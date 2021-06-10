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

#include "diskinfo.h"

#include <QDebug>
#include <QStorageInfo>
#include <DiskArbitration/DiskArbitration.h>

DiskInfo::DiskInfo(QString folderPath)
{
    qDebug()<<"======";
    qDebug()<<"getVolumeInformation for: " << folderPath;
    QStorageInfo storageInfo = QStorageInfo(folderPath);

    qDebug()<<"StorageInfo.name(): " << storageInfo.name();
    qDebug()<<"StorageInfo.displayName(): " << storageInfo.displayName();
    qDebug()<<"StorageInfo.rootPath(): " << storageInfo.rootPath();

    DASessionRef session = DASessionCreate(NULL);
    qDebug() <<"Session:"<<session;

    QString filePath = storageInfo.rootPath();
    QByteArray filePathArr = filePath.toLocal8Bit();

    CFURLRef path;
    path = CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, (const UInt8 *)filePathArr.constData(), filePathArr.size(), true );

//    auto disk = DADiskCreateFromBSDName(NULL, session, "/dev/disk1s5");
    auto disk = DADiskCreateFromVolumePath(kCFAllocatorDefault, session, path);

    if (!disk)
    {
        qDebug()<<"Failed DADiskCreateFromVolumePath";
        return;
    }

    auto desc = DADiskCopyDescription(disk);
    auto value = (CFTypeRef)CFDictionaryGetValue(desc, CFSTR("DAVolumeUUID"));
    qDebug()<<"Value:" << value;
    auto value2 = (CFTypeRef)CFDictionaryGetValue(desc, kDADiskDescriptionVolumeNameKey);
    qDebug()<<"Volume:" << value2;
    CFStringRef strValue = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@"), value);
    qDebug() << "DAVolumeUUID:" <<strValue;
    CFStringRef strValue2 = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@"), value2);
    qDebug() << "DAVolumeName:" <<strValue2;


    QString xRootPath = storageInfo.rootPath();
    QString xVolumeUUID = QString::fromCFString(strValue);

    if (strValue)
        CFRelease(strValue);
    if (strValue2)
        CFRelease(strValue2);
    if (value)
        CFRelease(value);
    if (value2)
        CFRelease(value2);
    if (disk)
        CFRelease(disk);
    if (path)
        CFRelease(path);

}
