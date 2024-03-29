/*
   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "settingsdialog.h"
#include "plurkresource.h"
#include "settings.h"

#include <libplurk/plurkapioauth.h>
#include <KAboutApplicationDialog>
#include <KAboutData>
#include <KWindowSystem>

using namespace Akonadi;

SettingsDialog::SettingsDialog( PlurkResource *parentResource, WId parentWindow )
    : KDialog(),
      mParentResource( parentResource ),
      mTriggerSync( false )
{
    KWindowSystem::setMainWindow( this, parentWindow );
    setButtons( Ok|Cancel|User1 );
    setButtonText(User1, i18n("About"));
    setButtonIcon(User1, KIcon("help-about"));
    setWindowIcon( KIcon( "plurkresource" ) );
    setWindowTitle( i18n("Plurk Settings") );

    setupWidgets();
    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
    if ( mTriggerSync ) {
        mParentResource->synchronize();
    }
}

void SettingsDialog::setupWidgets()
{
    QWidget * const page = new QWidget( this );
    setupUi( page );
    setMainWidget( page );
    updateAuthenticationWidgets();
    connect( resetButton, SIGNAL(clicked(bool)), this, SLOT(resetAuthentication()) );
    connect( authenticateButton, SIGNAL(clicked(bool)), this, SLOT(showAuthenticationDialog()) );
}

void SettingsDialog::showAuthenticationDialog()
{
    bool ret;
    ret = PlurkApiOAuth::self()->authorizeUser();
    if ( ret == true ) {
	QString accessToken = QString(PlurkApiOAuth::self()->oauthToken());
	kDebug() << "accessToken = " << accessToken << endl;

	authenticationDone(accessToken);
    }
}

void SettingsDialog::authenticationCanceled()
{
    authenticateButton->setEnabled( true );
}

void SettingsDialog::authenticationDone(const QString& accessToken)
{
    if ( Settings::self()->accessToken() != accessToken && !accessToken.isEmpty() ) {
        mTriggerSync = true;
    }
    Settings::self()->setAccessToken( accessToken );
    updateAuthenticationWidgets();
}

void SettingsDialog::updateAuthenticationWidgets()
{
    if ( Settings::self()->accessToken().isEmpty() ) {
        authenticationStack->setCurrentIndex( 0 );
    } else {
        authenticationStack->setCurrentIndex( 1 );
        if ( Settings::self()->userName().isEmpty() ) {
            authenticationLabel->setText( i18n( "Authenticated." ) );
        } else {
            authenticationLabel->setText( i18n( "Authenticated as <b>%1</b>.", Settings::self()->userName() ) );
        }
    }
}

void SettingsDialog::resetAuthentication()
{
    Settings::self()->setAccessToken( QString() );
    Settings::self()->setUserName( QString() );
    updateAuthenticationWidgets();
}

void SettingsDialog::loadSettings()
{
    if ( mParentResource->name() == mParentResource->identifier() )
        mParentResource->setName( i18n( "Plurk" ) );
}

void SettingsDialog::saveSettings()
{
    Settings::self()->writeConfig();
}

void SettingsDialog::slotButtonClicked( int button )
{
    switch( button ) {
        case Ok:
            saveSettings();
            accept();
            break;
        case Cancel:
            reject();
            return;
        case User1: {
            KAboutData aboutData( QByteArray( "akonadi_plurk_resource" ),
                                  QByteArray(),
                                  ki18n("Akonadi Plurk Resource"),
                                  QByteArray( RESOURCE_VERSION ),
                                  ki18n( "Makes your friends, events, notes and messages on Plurk available in KDE via Akonadi." ),
                                  KAboutData::License_GPL_V2,
                                  ki18n( "Copyright (C) 2010,2011 Akonadi Plurk Resource Developers" ) );
            aboutData.addAuthor( ki18n( "Thomas McGuire" ), ki18n( "Maintainer" ), "mcguire@kde.org" );
            aboutData.addAuthor( ki18n( "Roeland Jago Douma" ), ki18n( "Developer" ), "unix@rullzer.com" );
            aboutData.addCredit( ki18n( "Till Adam" ), ki18n( "MacOS Support" ), "adam@kde.org" );
            aboutData.setProgramIconName("plurkresource");
            aboutData.setTranslator( ki18nc("NAME OF TRANSLATORS", "Your names"),
                                  ki18nc("EMAIL OF TRANSLATORS", "Your emails"));
            KAboutApplicationDialog *dialog = new KAboutApplicationDialog(&aboutData, this);
            dialog->setAttribute( Qt::WA_DeleteOnClose, true );
            dialog->show();
            break;
        }
    }
}

#include "settingsdialog.moc"
