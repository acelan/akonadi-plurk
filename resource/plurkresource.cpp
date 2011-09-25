#include <klocalizedstring.h>

#include <config.h>
#include "plurkresource.h"

#include "settings.h"
#include "settingsadaptor.h"
#include "settingsdialog.h"

#include <QtDBus/QDBusConnection>
#include <Akonadi/EntityDisplayAttribute>

#include <KMessageBox>

using namespace Akonadi;

static const char* plurksRID = "plurks";

PlurkResource::PlurkResource( const QString &id )
  : ResourceBase( id )
{
  new SettingsAdaptor( Settings::self() );
  QDBusConnection::sessionBus().registerObject( QLatin1String( "/Settings" ),
                            Settings::self(), QDBusConnection::ExportAdaptors );

  // TODO: you can put any resource specific initialization code here.
  setNeedsNetwork( true );
  setObjectName( QLatin1String( "PlurkResource" ) );
  Settings::self()->setResourceId( identifier() );
}

PlurkResource::~PlurkResource()
{
  Settings::self()->writeConfig();
}

void PlurkResource::retrieveCollections()
{
  // TODO: this method is called when Akonadi wants to have all the
  // collections your resource provides.
  // Be sure to set the remote ID and the content MIME types

  Collection plurks( Akonadi::Collection::root() );
  plurks.setRemoteId( plurksRID );
  plurks.setName( i18n( "Plurk" ) );
  plurks.setContentMimeTypes( QStringList() << "application/x-vnd.kde.microblog" );
  plurks.setRights(Collection::ReadOnly);
  EntityDisplayAttribute * const plurksDisplayAttribute = new EntityDisplayAttribute();
  plurksDisplayAttribute->setIconName( "plurkresource" );
  plurks.addAttribute( plurksDisplayAttribute );

  collectionsRetrieved( Collection::List() << plurks );
}

void PlurkResource::retrieveItems( const Akonadi::Collection &collection )
{
  Q_UNUSED( collection );

  KMessageBox::information(0, "retrieveItems", "PlurkResource: retrieveItems");
  // TODO: this method is called when Akonadi wants to know about all the
  // items in the given collection. You can but don't have to provide all the
  // data for each item, remote ID and MIME type are enough at this stage.
  // Depending on how your resource accesses the data, there are several
  // different ways to tell Akonadi when you are done.
}

bool PlurkResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( item );
  Q_UNUSED( parts );

  KMessageBox::information(0, "retrieveItem", "PlurkResource: retrieveItem");
  // TODO: this method is called when Akonadi wants more data for a given item.
  // You can only provide the parts that have been requested but you are allowed
  // to provide all in one go

  return true;
}

void PlurkResource::aboutToQuit()
{
  // TODO: any cleanup you need to do while there is still an active
  // event loop. The resource will terminate after this method returns
}

void PlurkResource::configure( WId windowId )
{
  Q_UNUSED( windowId );

  // TODO: this method is usually called when a new resource is being
  // added to the Akonadi setup. You can do any kind of user interaction here,
  // e.g. showing dialogs.
  // The given window ID is usually useful to get the correct
  // "on top of parent" behavior if the running window manager applies any kind
  // of focus stealing prevention technique
  //
  // If the configuration dialog has been accepted by the user by clicking Ok,
  // the signal configurationDialogAccepted() has to be emitted, otherwise, if
  // the user canceled the dialog, configurationDialogRejected() has to be emitted.
  const QPointer<SettingsDialog> settingsDialog( new SettingsDialog( this, windowId ) );
  if ( settingsDialog->exec() == QDialog::Accepted ) {
    emit configurationDialogAccepted();
  }
  else {
    emit configurationDialogRejected();
  }

  delete settingsDialog;
}

void PlurkResource::itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection )
{
  Q_UNUSED( item );
  Q_UNUSED( collection );

  KMessageBox::information(0, "itemAdded", "PlurkResource: itemAdded");
  // TODO: this method is called when somebody else, e.g. a client application,
  // has created an item in a collection managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

void PlurkResource::itemChanged( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( item );
  Q_UNUSED( parts );

  KMessageBox::information(0, "itemChanged", "PlurkResource: itemChanged");
  // TODO: this method is called when somebody else, e.g. a client application,
  // has changed an item managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

void PlurkResource::itemRemoved( const Akonadi::Item &item )
{
  Q_UNUSED( item );

  KMessageBox::information(0, "itemRemoved", "PlurkResource: itemRemoved");
  // TODO: this method is called when somebody else, e.g. a client application,
  // has deleted an item managed by your resource.

  // NOTE: There is an equivalent method for collections, but it isn't part
  // of this template code to keep it simple
}

AKONADI_RESOURCE_MAIN( PlurkResource )

#include "plurkresource.moc"
