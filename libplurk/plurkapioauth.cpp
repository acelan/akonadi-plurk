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

#include <QtOAuth/QtOAuth>
#include <QtOAuth/interface.h>
#include <QtOAuth/qoauth_namespace.h>

#include <KToolInvocation>
#include <KDebug>
#include <KMessageBox>
#include <KInputDialog>
#include <kio/accessmanager.h>
#include <klocalizedstring.h>

#include "plurkapioauth.h"

const char* plurkOAuthRequestTokenURL = "http://www.plurk.com/OAuth/request_token";
const char* plurkOAuthAuthorizeURL = "http://www.plurk.com/OAuth/authorize";
const char* plurkOAuthAccessToken = "http://www.plurk.com/OAuth/access_token";
const char* plurkConsumerKey = "9239BYD7jOfx";
const char* plurkConsumerSecret = "RkIvvnYzezE0RwtK6BTASYyShpkp4xpG";

class PlurkApiOAuth::Private {
public:
    Private();

    QString username;

    QByteArray oauthToken;
    QByteArray oauthTokenSecret;
    QByteArray oauthConsumerKey;
    QByteArray oauthConsumerSecret;
    QSharedPointer< QOAuth::Interface > qoauth;
};

PlurkApiOAuth::Private::Private():
    username(),
    oauthToken(),
    oauthTokenSecret(),
    oauthConsumerKey(),
    oauthConsumerSecret(),
    qoauth( new QOAuth::Interface )
{
    kDebug();
    qoauth->setConsumerKey( plurkConsumerKey);
    qoauth->setConsumerSecret( plurkConsumerSecret);
    qoauth->setRequestTimeout(20000);
    qoauth->setIgnoreSslErrors(true);
}
 
PlurkApiOAuth* PlurkApiOAuth::mSelf = 0L;

PlurkApiOAuth::~PlurkApiOAuth()
{
}

PlurkApiOAuth* PlurkApiOAuth::self()
{
    if ( !mSelf )
        mSelf = new PlurkApiOAuth;
    return mSelf;
}

PlurkApiOAuth::PlurkApiOAuth():
    d( new Private )
{
}

QString PlurkApiOAuth::oauthErrorText(int code)
{
    switch(code){
        case QOAuth::NoError:
            return i18n("No Error");
        case QOAuth::BadRequest:
            return i18n("Bad request");
        case QOAuth::ConsumerKeyEmpty:
        case QOAuth::ConsumerSecretEmpty:
            return i18n("Consumer Key or Secret has not been provided");
        case QOAuth::Forbidden:
            return i18n("Forbidden");
        case QOAuth::Timeout:
            return i18n("Timeout on server");
        case QOAuth::Unauthorized:
            return i18n("Authorization Error");
        case QOAuth::UnsupportedHttpMethod:
            return i18n("Internal Error");
        case QOAuth::OtherError:
        case QOAuth::RSADecodingError:
        case QOAuth::RSAKeyFileError:
        case QOAuth::RSAPrivateKeyEmpty:
            return i18n("Unknown Error");
        default:
            return QString();
    }
}

const QByteArray & PlurkApiOAuth::oauthToken() const
{
    return d->oauthToken;
}

void PlurkApiOAuth::setOAuthToken(const QByteArray& token)
{
    d->oauthToken = token;
}

const QByteArray & PlurkApiOAuth::oauthTokenSecret() const
{
    return d->oauthTokenSecret;
}

void PlurkApiOAuth::setOAuthTokenSecret(const QByteArray& tokenSecret)
{
    d->oauthTokenSecret = tokenSecret;
}

const QByteArray & PlurkApiOAuth::oauthConsumerKey() const
{
    return d->oauthConsumerKey;
}

void PlurkApiOAuth::setOAuthConsumerKey(const QByteArray& consumerKey)
{
    d->oauthConsumerKey = consumerKey;
}

const QByteArray & PlurkApiOAuth::oauthConsumerSecret() const
{
    return d->oauthConsumerSecret;
}

void PlurkApiOAuth::setOAuthConsumerSecret(const QByteArray& consumerSecret)
{
    d->oauthConsumerSecret = consumerSecret;
}

bool PlurkApiOAuth::authorizeUser()
{
    kDebug();
    // set the consumer key and secret
    d->qoauth->setConsumerKey( plurkConsumerKey );
    d->qoauth->setConsumerSecret( plurkConsumerSecret );
    // set a timeout for requests (in msecs)
    d->qoauth->setRequestTimeout( 20000 );
    d->qoauth->setIgnoreSslErrors(true);

    QOAuth::ParamMap otherArgs;

    // send a request for an unauthorized token
    QOAuth::ParamMap reply =
        d->qoauth->requestToken( plurkOAuthRequestTokenURL, QOAuth::GET, QOAuth::HMAC_SHA1 );

    // if no error occurred, read the received token and token secret
    if ( d->qoauth->error() == QOAuth::NoError ) {
        d->oauthToken = reply.value( QOAuth::tokenParameterName() );
        d->oauthTokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
        kDebug() << "token: " << d->oauthToken;
        QUrl url(plurkOAuthAuthorizeURL);
        url.addQueryItem("oauth_token", d->oauthToken);
        url.addQueryItem( "oauth_callback", "oob" );
        KToolInvocation::invokeBrowser(url.toString());
        return getPinCode();
    } else {
        kDebug() << "ERROR: " << d->qoauth->error() << ' ' << oauthErrorText(d->qoauth->error());
        // TODO use a parent widget for this message box
        KMessageBox::detailedError(0, "Authorization Error",
                                   oauthErrorText(d->qoauth->error()));
    }

    return false;
}

bool PlurkApiOAuth::getPinCode()
{
    QString verifier = KInputDialog::getText( "Security code",
                                              "Security code recieved from Plurk",
                                                    "Enter security code:");
    if(verifier.isEmpty())
        return false;
    QOAuth::ParamMap otherArgs;
    otherArgs.insert( "oauth_verifier", verifier.toUtf8() );

    // send a request to exchange Request Token for an Access Token
    QOAuth::ParamMap reply =
    d->qoauth->accessToken( QString(plurkOAuthAccessToken),
                         QOAuth::GET, d->oauthToken, d->oauthTokenSecret, QOAuth::HMAC_SHA1, otherArgs );
    // if no error occurred, read the Access Token (and other arguments, if applicable)
    if ( d->qoauth->error() == QOAuth::NoError ) {
        d->username = reply.value( "screen_name" );
        d->oauthToken = reply.value( QOAuth::tokenParameterName() );
        d->oauthTokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
        // TODO use a parent widget for this message box
        KMessageBox::information(0, "Plurk is authorized successfully.",
                                 "Authorized");
        return true;
    } else {
        kDebug() << "ERROR: " << d->qoauth->error() << ' ' << oauthErrorText(d->qoauth->error());
        KMessageBox::detailedError(0, "Authorization Error",
                                oauthErrorText(d->qoauth->error()));
    }

    return false;
}

QByteArray PlurkApiOAuth::makeHeader( const QString & url, const QMap< QString, QString > & params ) const {
    QOAuth::ParamMap oaParams;
    for( QMap< QString, QString >::const_iterator it = params.begin(); it != params.end(); ++it ) {
        QByteArray key( QUrl::toPercentEncoding( it.key() ) ), value( QUrl::toPercentEncoding( it.value() ) );
        oaParams.insert( key, value );
    }
    // NOTE Always use POST and SHA1 here. This seems always work with Plurk.
    QByteArray header( d->qoauth->createParametersString( url, QOAuth::POST, d->oauthToken, d->oauthTokenSecret, QOAuth::HMAC_SHA1, oaParams, QOAuth::ParseForHeaderArguments ) );
    header.prepend( "Authorization: " );
    return header;
}
