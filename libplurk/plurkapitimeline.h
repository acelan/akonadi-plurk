
#ifndef PLURKAPITIMELINE_H
#define PLURKAPITIMELINE_H

#include <QByteArray>
#include "libplurk_export.h"

class LIBPLURK_EXPORT PlurkApiTimeline
{
public:
        PlurkApiTimeline();
        ~PlurkApiTimeline();
	int getPlurk();
	int getPlurks();
	int getUnreadPlurks();
	int getPublicPlurks();
	int plurkAdd();
	int plurkDelete();
	int plurkEdit();
	int mutePlurks();
	int unmutePlurks();
	int favoritePlurks();
	int unfavoritePlurks();
	int replurk();
	int unreplurk();
	int markAsRead();
	int uploadPicture();

private:
	bool authorized;

	QByteArray oauthToken;

};

#endif
