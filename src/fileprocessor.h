/*
    MIT License

    Copyright (c) 2021 Astrocat.App

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include "astrofile.h"

class FileProcessor
{
public:
    virtual ~FileProcessor() {};

    virtual bool loadFile(const AstroFile& astroFile) = 0;
    virtual void extractTags() = 0;
    virtual void extractThumbnail() = 0;
    virtual QMap<QString, QString> getTags() = 0;
    virtual QImage getThumbnail() = 0;
    virtual QImage getTinyThumbnail() = 0;
    virtual QByteArray getImageHash() = 0;
};

#endif // FILEPROCESSOR_H
