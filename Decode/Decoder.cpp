#include "Decoder.h"
#include <QCoreApplication>
#include <QDir>

const size_t MAX_PATH = 260;
const int INTERFACE_FREQ = 4096; // how often interface will refresh

void Decoder::Decode()
{

    if (archivePath == "")
    {
        emit error("Invalid archive filename: " + archivePath);
        return;
    }

    std::ifstream inFile;

    // Generating name for directory where
    // encoded files will be
    QDir dir = QDir::current();
    QFileInfo dirInfo(archivePath);

    QString dirName = dirInfo.baseName() + "_dcv";
    if (!dir.mkdir(dirName))
    {
        emit error("Error while creating folder: " + dirName);
        return;
    }
    QDir::setCurrent(dirName);

    // Opening archive
    inFile.open(archivePath.toLocal8Bit(), std::ios_base::binary | std::ios_base::in);
	if (!inFile.good())
    {
        emit error("Error while opening archive file: " + archivePath);
        return;
    }

	uint64_t n; // number of files
	inFile.read(reinterpret_cast<char *>(&n), sizeof(n));    

    uint64_t totalSize; // total size of all files in archive in bits
    inFile.read(reinterpret_cast<char *>(&totalSize), sizeof(totalSize));    

    uint64_t alphSize;  // size of alphabet
    inFile.read(reinterpret_cast<char *>(&alphSize), sizeof(alphSize));

    if (br)
        return;
	//
	// READING AND GENERATING TREE
	//
	unsigned char c, codeBuff[32]; // 32 = 256/8	
	tree = new Node{ {}, nullptr, nullptr };
	Node **temp;
	bool b;
    for (uint64_t i = 0; i < alphSize; i++) // Reading huffman table
	{
        if (br)
            return;
		inFile.read(reinterpret_cast<char *>(&c), sizeof(c)); // symbol (1 byte);

		unsigned char k;  // number of bits in code (1 byte)
		inFile.read(reinterpret_cast<char *>(&k), sizeof(k));		

		unsigned char k1 = k / 8 + ((k % 8 == 0) ? 0 : 1); // size of code in bytes
		inFile.read(reinterpret_cast<char *>(codeBuff), k1); // reading code
		
		// left = true, right = false
		tree->s.insert(c);
		temp = &tree;
		for (unsigned char j = 0; j < k; j++)
		{
			b = (codeBuff[j / 8] & (1 << (j % 8))) != 0;

			temp = (b ? &((*temp)->left) : &((*temp)->right));
			if (!*temp)
				*temp = new Node{ {}, nullptr, nullptr };
			(*temp)->s.insert(c);
		}
		
		temp = nullptr;
	}

    char fileName[MAX_PATH];

    std::ofstream outFile;

    double totalProgress = 0, prog;

    for (uint64_t p = 0; p < n; p++) // for all files
	{		
        if (br)
            return;
        { // READING FILE NAME
            size_t i = 0;
            while (fileName[i++] = inFile.get());
        }

        QString qFileName = fileName;
        outFile.open(qFileName.toLocal8Bit(), std::ios_base::binary | std::ios_base::out);

        if (!outFile.good())
        {
            error(QString("Error while creating file: ") + fileName);
            return;
        }

        uint64_t size, progSize; // size of file in bits
        inFile.read(reinterpret_cast<char *>(&size), sizeof(size));
        progSize = size;
		       
		Node *t = tree;    

        prog = 0;

		while (inFile.good() && size)
		{
            if (br)
                return;
            inFile.read(reinterpret_cast<char *>(buff), std::min(buffSize, (size + 7)/8)); // read part of data
            std::streamsize wasRead = inFile.gcount();

            for (std::streamsize i = 0; i < wasRead; i++) // for all bytes that we have read
            {
                if (i % INTERFACE_FREQ == 0) // interface responsability
                {
                    prog += INTERFACE_FREQ * 800.0 / progSize;
                    totalProgress += INTERFACE_FREQ * 800.0 / totalSize;                    
                    emit progress(fileName, prog, totalProgress);                    
                    QCoreApplication::processEvents();
                }

                for (uint64_t j = 0; j < 8 && size; j++, size--) // for all bits in byte
                {                    
                    t = buff[i] & (1 <<  (7 - j)) ? t->left : t->right; // if bit is 1 we're going to the left,
																			// otherwise - to the right				
                    if (!t->left && !t->right) // if we found unsigned char
                    {
                        c = *t->s.begin();
                        outFile.write(reinterpret_cast<char *>(&c), sizeof(c));
                        t = tree; // new code is starting
                    }
                }
            }
		}

        if (size)
        {
            emit error("Corrupted archive file: " + archivePath);
            return;
        }

		outFile.close();
	}
    emit progress(fileName, 100, 100);
    emit finished();
}

void Decoder::destrRec(Node *p)
{
	if (p->right)
		destrRec(p->right);
	if (p->left)
		destrRec(p->left);

	delete p;
}

Decoder::~Decoder()
{
    delete [] buff;
	destrRec(tree);
}
