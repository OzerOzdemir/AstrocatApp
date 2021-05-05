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

#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include "astrofile.h"

#include <QDate>
#include <QObject>
#include <QSortFilterProxyModel>

class SortFilterProxyModel : public  QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = nullptr);
    QDate filterMinimumDate() const { return minDate; }
    QDate filterMaximumDate() const { return maxDate; }

public slots:
    Q_INVOKABLE void setFilterMinimumDate(QDate date);
    Q_INVOKABLE void setFilterMaximumDate(QDate date);
    Q_INVOKABLE void addAcceptedFilter(QString filterName);
    Q_INVOKABLE void removeAcceptedFilter(QString filterName);
    Q_INVOKABLE void addAcceptedInstrument(QString instrumentName);
    Q_INVOKABLE void removeAcceptedInstrument(QString instrumentName);
    Q_INVOKABLE void addAcceptedObject(QString objectName);
    Q_INVOKABLE void removeAcceptedObject(QString objectName);
    Q_INVOKABLE void addAcceptedExtension(QString extensionName);
    Q_INVOKABLE void removeAcceptedExtension(QString extensionName);
    void activateDuplicatesFilter(bool shouldActivate);
    void setDuplicatesFilter(QString filter);

signals:
    void filterMinimumDateChanged(QDate date);
    void filterMaximumDateChanged(QDate date);
    void filterReset();
    void astroFileAdded(const AstroFile& astroFile) const;

protected:
    // QSortFilterProxyModel interface
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
//    bool insertRows(int row, int count, const QModelIndex &parent) override;

private:
    QDate minDate;
    QDate maxDate;
    QList<QString> acceptedFilters;
    QList<QString> acceptedObjects;
    QList<QString> acceptedInstruments;
    QList<QString> acceptedExtensions;
    QMap<QString, QMap<QString,int>> fileTags;
    QSet<int> acceptedAstroFiles;
    bool dateInRange(QDate date) const;
    bool instrumentAccepted(QString instrument) const;
    bool objectAccepted(QString object) const;
    bool filterAccepted(QString filter) const;
    bool extensionAccepted(QString filter) const;
    bool isDuplicatedFilterActive;
    QString duplicatesFilter;
    bool isDuplicateOf(QString hash) const;
//    void updateTagCount(const AstroFile* astroFile, QString tag) const;

protected slots:
    virtual void resetInternalData() override;

};

#endif // SORTFILTERPROXYMODEL_H
