#include "bmp.h"

#include <iostream>

#include <fstream>
#include <ostream>

void getShort(unsigned short& num, ifstream& fstream);
void getShort(short& num, ifstream& stream);
void getInt(unsigned int& num, ifstream& fstream);
void getInt(int& num, ifstream& stream);

void writeInt(unsigned int& num, ofstream& fstream);
void writeShort(unsigned short& num, ofstream& fstream);


BMPFile::BMPFile(string filename) {


    this->fileHeader.type =  "";

    ifstream fstream(filename , ios::in | ios::binary);

    this->fileHeader.type.push_back(fstream.get());
    this->fileHeader.type.push_back(fstream.get());

    getInt(this->fileHeader.fileSize,fstream);

    getInt(this->fileHeader.reserved,fstream);

    getInt(this->fileHeader.dataOffset, fstream);
    getInt(this->infoHeader.infoHeaderSize,fstream);
    getInt(this->infoHeader.width,fstream);
    getInt(this->infoHeader.height,fstream);
    getShort(this->infoHeader.planes,fstream);
    getShort(this->infoHeader.bpp,fstream);
    getInt(this->infoHeader.compression,fstream);
    getInt(this->infoHeader.imageSize,fstream);
    if ((this->infoHeader.imageSize==0)&&(this->infoHeader.compression==0)){
        this->infoHeader.imageSize = this->infoHeader.width * this->infoHeader.height;
    }
    getInt(this->infoHeader.XPixelsPerM, fstream);
    getInt(this->infoHeader.YPixelsPerM,fstream);
    getInt(this->infoHeader.colorUsed, fstream);
    if(this->infoHeader.colorUsed==0){
        this->infoHeader.colorUsed = 2<<(this->infoHeader.bpp);
    }
    getInt(this->infoHeader.importantColors,fstream);
    if(this->infoHeader.importantColors==0){
        this->infoHeader.importantColors = this->infoHeader.colorUsed;
    }


    if(this->infoHeader.bpp<=8){
        // Get color table
        this->colorTable = new char*[this->infoHeader.colorUsed];
        for (int idx = 0 ; idx < this->infoHeader.colorUsed ; idx++){
            this->colorTable[idx] = new char[4];
            for (int c = 0 ; c < 4; c++){
                this->colorTable[idx][c] = fstream.get();
            }
        }

    }

    this->content = new char[this->infoHeader.imageSize];

    for (int byte = 0; byte < this->infoHeader.imageSize ; byte++){
        this->content[byte] = fstream.get();
    }

    fstream.close();
}

unsigned int BMPFile::getImageSize(){
    return this->infoHeader.imageSize;
}
char* BMPFile::getContent(){
    return this->content;
}
void BMPFile::setContent(int idx, char c){
	this->content[idx] = c;
}

void BMPFile::write(string filename){

    ofstream fstream(filename, ofstream::out);

    // Write content
    fstream.put(this->fileHeader.type[0]);
    fstream.put(this->fileHeader.type[1]);

    writeInt(this->fileHeader.fileSize,fstream);

    writeInt(this->fileHeader.reserved,fstream);
    writeInt(this->fileHeader.dataOffset, fstream);
    writeInt(this->infoHeader.infoHeaderSize,fstream);
    writeInt(this->infoHeader.width,fstream);
    writeInt(this->infoHeader.height,fstream);
    writeShort(this->infoHeader.planes,fstream);
    writeShort(this->infoHeader.bpp,fstream);
    writeInt(this->infoHeader.compression,fstream);
    writeInt(this->infoHeader.imageSize,fstream);
    writeInt(this->infoHeader.XPixelsPerM, fstream);
    writeInt(this->infoHeader.YPixelsPerM,fstream);
    writeInt(this->infoHeader.colorUsed, fstream);
    writeInt(this->infoHeader.importantColors,fstream);


    if(this->infoHeader.bpp<=8){
        for (int idx = 0 ; idx < this->infoHeader.colorUsed ; idx++){
            for (int c = 0 ; c < 4; c++){
                fstream.put(this->colorTable[idx][c]);
            }
        }
    }
    for (int byte = 0; byte < this->infoHeader.imageSize ; byte++){
        fstream.put(this->content[byte]);
    }

    fstream.close();
}



// Little endians short and ints

void getShort(unsigned short& num, ifstream& fstream){
    num = 0;
    for (short byte = 0 ; byte < 2 ; byte++){
        num += fstream.get() << (8 * byte);
    }

}
void getInt(unsigned int& num, ifstream& fstream){
    num = 0;
    for (short byte = 0 ; byte < 4 ; byte++){
        num += fstream.get() << (8 * byte);
    }
}
void getShort(short& num, ifstream& fstream){
    num = 0;
    for (short byte = 0 ; byte < 2 ; byte++){
        num += fstream.get() << (8 * byte);
    }
}
void getInt(int& num, ifstream& fstream){
    num = 0;
    for (short byte = 0 ; byte < 4 ; byte++){
        num += fstream.get() << (8 * byte);
    }
}


void writeInt(unsigned int& num, ofstream& fstream){

    char input;
    for (int byte = 0 ; byte < sizeof(unsigned int) ; byte++){
        input =  (num >> (8 * byte))&0xFF; 
        fstream.put(input);
    }
}

void writeShort(unsigned short& num, ofstream& fstream){
    char input;
    for (int byte = 0 ; byte < sizeof(short) ; byte++){
        input = (num >> (8 * byte))&0xFF; 
        fstream.put(input);
    }
}

