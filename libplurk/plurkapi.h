/*
Copyright (C) 2011 AceLan Kao <acelan@acelan.idv.tw>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see http://www.gnu.org/licenses/
*/
#ifndef PLURKAPI_H
#define PLURKAPI_H

#include <kio/job.h>
#include <QtCore/QDateTime>
#include <QStringList>

#include "libplurk_export.h"
#include "plurkapioauth.h"

#include <qjson/parser.h>

class PlurkApi : public QObject
{
Q_OBJECT
public:
    PlurkApi() {}
    virtual ~PlurkApi() {}
    KIO::StoredTransferJob * makeRequest( const QString & apiPath, const QMap< QString, QString > & params );
    void execute( const QString & apiPath, const QMap< QString, QString > & params);

public Q_SLOTS:
    virtual void slotExecute(KJob* job);

Q_SIGNALS:
    void jobDone( const QVariantMap& data);

protected:
    QJson::Parser parser;
};

#endif
