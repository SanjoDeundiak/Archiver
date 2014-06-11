#ifndef DECODER_H
#define DECODER_H

// Qt headers
#include <QObject>

// STL headers
#include <vector>
#include <set>

#include <algorithm>

#include <stdint.h>

#include <fstream>
#include <string>


const uint64_t buffSize = 0x100000; // 16Mb
const uint64_t aSize = 256; // alphabet size

class Decoder : public QObject
{
    Q_OBJECT

signals:
    void progress(QString, int, int) const; // sends progress to ui
    void error(QString) const; // sends error to ui
    void finished() const; // send "all is done" signal to ui

public slots:
    void interrupt() { br = true; } // external interrupt was occured

private:
    unsigned char *buff = new unsigned char[buffSize]; // input buffer

	struct Node // for tree
	{
        std::set<char> s;
		Node *left, *right;
	};

    bool br = false; // flag for interrupt
    Node *tree = nullptr; // tree :D
    QString archivePath;

    void destrRec(Node *p); // for deructor

	// To block copying of this class
	Decoder(const Decoder &other);
	const Decoder& operator=(const Decoder &other) const;

public:
    Decoder(QString arcPath) : archivePath(arcPath) {}
    ~Decoder();

    void Decode();
};

#endif // DECODER_H
