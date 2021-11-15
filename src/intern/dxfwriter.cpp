/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2011-2015 José F. Soriano, rallazz@gmail.com               **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <fstream>
#include <string>
#include <algorithm>
#include "dxfwriter.h"

//RLZ TODO change std::endl to x0D x0A (13 10)

bool dxfWriter::writeUtf8String(int code, const std::string& text) {
    std::string t = encoder.fromUtf8(text);
    return writeString(code, t);
}

bool dxfWriter::writeUtf8Caps(int code, const std::string &text) {
    std::string strname = text;
    std::transform(strname.begin(), strname.end(), strname.begin(),::toupper);
    std::string t = encoder.fromUtf8(strname);
    return writeString(code, t);
}

bool dxfWriterBinary::writeString(int code, std::string text) {
    writeIntegerValue(code, 2);
    *filestr << text << '\0';
    return (filestr->good());
}

/*bool dxfWriterBinary::readCode(int *code) {
    unsigned short *int16p;
    char buffer[2];
    filestr->read(buffer,2);
    int16p = (unsigned short *) buffer;
//exist a 32bits int (code 90) with 2 bytes???
    if ((*code == 90) && (*int16p>2000)){
        DBG(*code); DBG(" de 16bits\n");
        filestr->seekg(-4, std::ios_base::cur);
        filestr->read(buffer,2);
        int16p = (unsigned short *) buffer;
    }
    *code = *int16p;
    DBG(*code); DBG("\n");

    return (filestr->good());
}*/

/*bool dxfWriterBinary::readString() {
    std::getline(*filestr, strData, '\0');
    DBG(strData); DBG("\n");
    return (filestr->good());
}*/

/*bool dxfWriterBinary::readString(std::string *text) {
    std::getline(*filestr, *text, '\0');
    DBG(*text); DBG("\n");
    return (filestr->good());
}*/

bool dxfWriterBinary::writeInt16(int code, int data) {
    return writeIntegerValueAndValidate(code, data, 2);
}

bool dxfWriterBinary::writeInt32(int code, int data) {
    return writeIntegerValueAndValidate(code, data, 4);
}

bool dxfWriterBinary::writeInt64(int code, unsigned long long int data) {
    return writeIntegerValueAndValidate(code, data, 8);
}

template<typename T>
void dxfWriterBinary::writeIntegerValue( T data, long buffer_size_in_bytes) {
    static_assert(std::numeric_limits<T>::is_integer, "using wrong type");

    auto buffer = std::unique_ptr<char>(new char[buffer_size_in_bytes]);

    for(int i = 0; i < buffer_size_in_bytes; i++) {
        buffer.get()[i] = static_cast<char>((data >> (i * 8)) & 0xFF);
    }
    filestr->write(buffer.get(), buffer_size_in_bytes);
}

template<typename T>
bool dxfWriterBinary::writeIntegerValueAndValidate(int code, T data, long data_size_in_bytes) {

    writeIntegerValue(code,2 );
    writeIntegerValue(data, data_size_in_bytes);
    return (filestr->good());
}

bool dxfWriterBinary::writeDouble(int code, double data) {
    writeIntegerValue(code, 2);

    char buffer[8];
    unsigned char *val;
    val = (unsigned char *) &data;
    for (int i = 0; i < 8; i++) {
        buffer[i] = static_cast<char>(val[i]);
    }
    filestr->write(buffer, 8);
    return (filestr->good());
}

//saved as int or add a bool member??
bool dxfWriterBinary::writeBool(int code, bool data) {
    return writeIntegerValueAndValidate(code, data, 1);
}

dxfWriterAscii::dxfWriterAscii(std::ofstream *stream):dxfWriter(stream){
    filestr->precision(16);
}

bool dxfWriterAscii::writeString(int code, std::string text) {
//    *filestr << code << std::endl << text << std::endl ;
    filestr->width(3);
    *filestr << std::right << code << std::endl;
    filestr->width(0);
    *filestr << std::left << text << std::endl;
    /*    std::getline(*filestr, strData, '\0');
    DBG(strData); DBG("\n");*/
    return (filestr->good());
}

bool dxfWriterAscii::writeInt16(int code, int data) {
//    *filestr << std::right << code << std::endl << data << std::endl;
    filestr->width(3);
    *filestr << std::right << code << std::endl;
    filestr->width(5);
    *filestr << data << std::endl;
    return (filestr->good());
}

bool dxfWriterAscii::writeInt32(int code, int data) {
    return writeInt16(code, data);
}

bool dxfWriterAscii::writeInt64(int code, unsigned long long int data) {
//    *filestr << code << std::endl << data << std::endl;
    filestr->width(3);
    *filestr << std::right << code << std::endl;
    filestr->width(5);
    *filestr << data << std::endl;
    return (filestr->good());
}

bool dxfWriterAscii::writeDouble(int code, double data) {
//    std::streamsize prec = filestr->precision();
//    filestr->precision(12);
//    *filestr << code << std::endl << data << std::endl;
    filestr->width(3);
    *filestr << std::right << code << std::endl;
    *filestr << data << std::endl;
//    filestr->precision(prec);
    return (filestr->good());
}

//saved as int or add a bool member??
bool dxfWriterAscii::writeBool(int code, bool data) {
    *filestr << code << std::endl << data << std::endl;
    return (filestr->good());
}

