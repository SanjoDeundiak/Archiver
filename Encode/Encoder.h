#ifndef CODER_H
#define CODER_H

// Qt headers
#include <QObject>
#include <QFile>

// std containers
#include <vector>
#include <queue>
#include <array>

#include <stdint.h>

const uint64_t buffSize = 0x100000; // 16Mb
const uint64_t aSize = 256; // alphabet size

using std::vector;
using std::queue;
using std::array;
using std::ofstream;
using std::ifstream;
using std::streamsize;
using std::list;
using std::wstring;
using std::string;
using std::pair;

class Encoder : public QObject
{	
    Q_OBJECT

signals:
    void progress(QString, int, int) const; // send progress to ui
    void error(QString) const; // error occured
    void finished() const; // all is done

public slots:
    void interrupt() { br = true; }

private:
    unsigned char *buff = new unsigned char[buffSize];
    unsigned char *buffOut = new unsigned char[buffSize];

    bool br = false; // interrupted?
    bool f = false; // finished?

    QString archivePath;
    vector<QString> filePathes; // pathes of file that we're going to compress
	array <vector<bool>, aSize> codes; // their Huffman codes	
    array<uint64_t, aSize> getFreq(vector< array<uint64_t, aSize> > &freq, uint64_t &size) const;
																		// return frequency for all files as result
																		// return for each file as parameter
																		// return size of alphabet as parameter
    vector<uint64_t> getSize(const vector< array<uint64_t, aSize> > &m) const; // returns sizes in bits
                                                                               // for all compressed files
	unsigned char generateChar(queue<bool> &v) const; // generates unsigned char for code, pops used elements
	void generateCodes(const array<uint64_t, aSize> &freq); // generates Huffan codes using frequncy of unsigned chars

    // To block copying of objects of this class
	Encoder(const Encoder &other); 
	const Encoder& operator=(const Encoder &other) const;

public:
    Encoder(const vector<QString> &, const QString &);
    ~Encoder();

    void Encode();
};

#endif
