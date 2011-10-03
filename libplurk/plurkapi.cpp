#include <kio/jobclasses.h>
#include <kio/job.h>
#include <KDebug>

#include "plurkapi.h"

KIO::StoredTransferJob* PlurkApi::makeRequest( const QString & apiPath, const QMap< QString, QString > & params )
{
    QByteArray data;
    for( QMap< QString, QString >::const_iterator it = params.begin(); it != params.end(); ++it ) {
        QByteArray key( QUrl::toPercentEncoding( it.key() ) ), value( QUrl::toPercentEncoding( it.value() ) );
        data += key + "=" + value + "&";
    }
    data.chop( 1 );

    KUrl url( apiPath );

    // NOTE Plurk API 2.0 always use OAuth
    QByteArray header( PlurkApiOAuth::self()->makeHeader( url.url(), params ) );
    kDebug() << header;

    // FIXME when will this object be deleted?
    KIO::StoredTransferJob* job = KIO::storedHttpPost( data, url, KIO::HideProgressInfo );
    // for POST method
    job->addMetaData( "content-type", "Content-Type: application/x-www-form-urlencoded" );
    job->addMetaData( "customHTTPHeader", header );

    return job;
}

void PlurkApi::execute( const QString & apiPath, const QMap< QString, QString > & params)
{
    KIO::StoredTransferJob* job= makeRequest( apiPath, params );
    if ( !job ) {
        kDebug() << "Cannot create an http GET request!";
        return;
    }

    connect( job, SIGNAL( result( KJob * ) ), this, SLOT( slotGetProfile( KJob * ) ) );
    job->start();
}

void PlurkApi::slotExecute(KJob* job)
{
    KIO::StoredTransferJob* stJob = qobject_cast<KIO::StoredTransferJob*>( job );
    if (stJob->error()) {
        kDebug() << "Execute command failed: " << stJob->errorString();
        return;
    }

    // turn stJob->data() into map format
    bool ok;
    QVariantMap data= parser.parse( stJob->data(), &ok).toMap();

    if( !ok)
        kDebug() << "Converting to QVariantMap Failed!";

    emit jobDone( data);
}


