#include "Encoder.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <fstream>

const size_t MAX_PATH = 260;
const int INTERFACE_FREQ = 4096; // how often interface will refresh

void Encoder::Encode()
{
    //
    // modify to work with folders if needed
    //
    // modify to work with big/little endain independent from architecture if needed
    //

    if (archivePath.isEmpty())
    {
        error("Invalid archive path");
        return;
    }

    if (filePathes.empty())
    {        
        error("Invalid file pathes");
        return;
    }

	uint64_t c; // size of alphabet
    vector< array<uint64_t, aSize> > filesFrequency;
    if (br)
        return;
    try
    {
        generateCodes(getFreq(filesFrequency, c));
    }
    catch (std::exception &e)
    {        
        error(e.what());
        return;
    }

    vector<uint64_t> sizes = getSize(filesFrequency);

    ofstream outFile;

    if (br)
        return;

    outFile.open(archivePath.toLocal8Bit(), std::ios_base::binary | std::ios_base::out);


    uint64_t totalSize = 0;

    for (size_t i = 0; i < sizes.size(); i++)
        totalSize += sizes[i];

	if (!outFile.good())
    {        
        error(archivePath.prepend("Error creating archive file: "));
        return;
    }

	{
		uint64_t n = filePathes.size();
		outFile.write(reinterpret_cast<const char *>(&n), sizeof(n)); // Number of files	

        outFile.write(reinterpret_cast<const  char *>(&totalSize), sizeof(totalSize)); // 8 bytes;

		outFile.write(reinterpret_cast<const  char *>(&c), sizeof(c)); // Size of alphabet	
	}

	for (uint64_t it = 0; it < aSize; it++) // Huffman table
	if (!codes[it].empty())
	{
        if (br)
            return;                
		outFile.write(reinterpret_cast<const  char *>(&it), sizeof(char)); // symbol (1 byte);
		unsigned char n = codes[it].size();
		outFile.write(reinterpret_cast<const  char *>(&n), sizeof(n)); // number of bits in code (1 byte)
		unsigned char code = 0;

		uint64_t i;
		for (i = 0; i < codes[it].size(); i++) // code
		{
			if (codes[it][i])
				code |= 1 << (i % 8);

			if (i % 8 == 7)
			{
				outFile.write(reinterpret_cast<const  char *>(&code), sizeof(code));
				code = 0;
			}
		}

		if (i % 8 != 0)
			outFile.write(reinterpret_cast<const  char *>(&code), sizeof(code));
	}


    unsigned char bit;
	ifstream inFile;
    uint64_t k; // counter for buffOut in bits

    double totalProgress = 0, prog;
	for (uint64_t p = 0; p < filePathes.size(); p++)
    {        
        k = 0;
        outFile << QFileInfo(filePathes[p]).fileName().toStdString() << '\0'; // file name
		outFile.write(reinterpret_cast<const  char*>(&sizes[p]), sizeof(sizes[p])); // size of file in bits

        inFile.open(filePathes[p].toLocal8Bit(), std::ios_base::binary | std::ios_base::in);

		if (!inFile.good())
        {
            error(filePathes[p].prepend("Error while opening file: "));
            return;
        }

        prog = 0;
		while (inFile.good()) // coding
		{
            if (br)
                return;
            inFile.read(reinterpret_cast<char *>(buff), buffSize);
			streamsize wasRead = inFile.gcount();            

			for (streamsize i = 0; i < wasRead; i++)
			{
                if (i % INTERFACE_FREQ == 0) // interface responsable
                {
                    prog += INTERFACE_FREQ * 800.0 / sizes[p];
                    totalProgress += INTERFACE_FREQ * 800.0 / totalSize;
                    emit progress(filePathes[p], prog, totalProgress);
                    QCoreApplication::processEvents();
                }

                for (uint64_t j = 0; j < codes[buff[i]].size(); j++, k++)
				{
					if (k == buffSize * 8)
					{
                        outFile.write(reinterpret_cast<const char *>(buffOut), buffSize);
						k = 0;
                    }
                    if (k % 8 == 0)
                        buffOut[k / 8] = 0;
                    bit = 1 << (7 - k % 8);
					if (codes[buff[i]][j])
						buffOut[k / 8] |= bit;
					else
						buffOut[k / 8] &= ~bit;					
				}				
			}
		}

		if (k)				
            outFile.write(reinterpret_cast<const char *>(buffOut), (k + 7) / 8);

		inFile.close();
	}    
    f = true;
    emit progress(filePathes.back(), 100, 100);
    emit finished();
}

array<uint64_t, aSize> Encoder::getFreq(vector< array<uint64_t, aSize> > &freqVector, uint64_t &size) const
{
	size = 0;
    array <uint64_t, aSize> freq, forCurFile;
	ifstream inFile;

	freq.fill(0);

	//
	// do existence check, and maybe unique check
	//
	
	for (uint64_t i = 0; i < filePathes.size(); i++)
	{        
        inFile.open(filePathes[i].toLocal8Bit(), std::ios_base::binary | std::ios_base::in);

        if (!inFile.good()) {
            QString fp = filePathes[i];
            throw std::runtime_error(fp.prepend("Error while first opening file: ").toUtf8().data());
        }

		forCurFile.fill(0);

		while (inFile.good())
		{
            if (br)
                return freq;
            QCoreApplication::processEvents();
            inFile.read(reinterpret_cast<char *>(buff), buffSize);
			streamsize wasRead = inFile.gcount();
			for (streamsize i = 0; i < wasRead; i++)
			{
				if (!freq[buff[i]])
					size++;
				++freq[buff[i]];
				++forCurFile[buff[i]];
			}
		}		

		freqVector.push_back(forCurFile);
		inFile.close();
	}

    return freq;
}

void Encoder::generateCodes(const array<uint64_t, aSize> &freq)
{
    // copy to new list
    list<pair<string, uint64_t>> f;
    string temp;
	unsigned char c = 0;
	for (auto it = freq.begin(); it != freq.end(); it++, c++)
	if (*it)
	{        
        temp = *reinterpret_cast<const char *>(&c);
        f.push_back(pair<string, uint64_t>(temp, *it));
	}	

	uint64_t n = f.size();
	
	unsigned char unc;
	if (n > 1) // if n == 1 no sense to do it
	for (uint64_t i = 0; i < n - 1; i++) // for all unsigned char that we are using
	{
		// finding 2 minimum values				
        list<pair<string, uint64_t>>::iterator it1, it2, it;
		it1 = f.begin();
		it2 = it1;
		it2++;
		it = it2;
		it++;

		if (it2->second < it1->second)
            std::swap(it1, it2);
		
		for (; it != f.end(); it++)
		{
			if (it->second < it1->second)
			{
				it2 = it1;
				it1 = it;
			}
			else if (it->second < it2->second)
			{
				it2 = it;
			}
		}

		// make new element, delete 2 old elements
        string set = it1->first + it2->first;
        f.push_front(pair<string, uint64_t>(set, it1->second + it2->second));

		
		for (uint64_t i = 0; i < it1->first.length(); i++)
		{
			unc = *reinterpret_cast<unsigned char *> (&(it1->first[i]));
			codes[unc].insert(codes[unc].begin(), false);
		}
		for (uint64_t i = 0; i < it2->first.length(); i++)
		{
			unc = *reinterpret_cast<unsigned char *> (&(it2->first[i]));
			codes[unc].insert(codes[unc].begin(), true);
		}

		f.erase(it1);
		f.erase(it2);
	}
	else if (n == 1)
		codes[f.begin()->first[0]] = { false };	
}

vector<uint64_t> Encoder::getSize(const vector< array<uint64_t, aSize> > &m) const
{
	vector<uint64_t> result;

	uint64_t size;
	for (uint64_t it = 0; it < m.size(); it++) // for all files
	{
		size = 0;
		for (uint64_t c = 0; c < aSize; c++) // for whole alphabet		
			size += m[it][c] * codes[c].size();						

		result.push_back(size);		
	}

	return result;
}

unsigned char Encoder::generateChar(queue<bool> &v) const
{
	unsigned char c = 0;
	bool flag;
	uint64_t n = (v.size() < 8) ? v.size() : 8;

	for (uint64_t i = 0; i < n; i++)
	{
		flag = v.front();
		if (flag)		
			c |= 1 << (7 - i);
		
		v.pop();
	}

	return c;
}

Encoder::Encoder(const vector<QString> &v, const QString &arcPath) :
    archivePath(arcPath),
    filePathes(v)
{}

Encoder::~Encoder()
{
    delete [] buff;
    delete [] buffOut;
    if (br && !f)
        QFile::remove(archivePath);
}
