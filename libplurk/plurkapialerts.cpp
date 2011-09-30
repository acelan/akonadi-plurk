
/*
    Copyright (C) 2008-2011 AceLan Kao <acelan@acelan.idv.tw>, Franklin Weng <franklin@goodhorse.idv.tw>

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


#include "plurkapialerts.h"

bool PlurkApiAlerts::getActive()
{
	QString url = apiUrl + "getActive";

	return false;
}

bool PlurkApiAlerts::getHistory()
{
	QString url = apiUrl + "getHistory";

	return false;
}

bool PlurkApiAlerts::addAsFan()
{
	QString url = apiUrl + "addAsFan";

	return false;
}

bool PlurkApiAlerts::addAllAsFan()
{
	QString url = apiUrl + "addAllAsFan";

	return false;
}

bool PlurkApiAlerts::addAllAsFriends()
{
	QString url = apiUrl + "addAllAsFriends";

	return false;
}

bool PlurkApiAlerts::addAsFriend()
{
	QString url = apiUrl + "addAsFriend";

	return false;
}

bool PlurkApiAlerts::denyFriendship()
{
	QString url = apiUrl + "denyFriendship";

	return false;
}

bool PlurkApiAlerts::removeNotification()
{
	QString url = apiUrl + "removeNotification";

	return false;
}
