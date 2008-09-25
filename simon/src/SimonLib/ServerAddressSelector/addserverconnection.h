/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef ADDSERVERCONNECTION_H
#define ADDSERVERCONNECTION_H

#include "ui_addserverconnectiondlg.h"
#include <KDialog>

/**
 \class AddServerConnection
 \author Peter Grasch
 \version 0.1
 \date 12.08.2007

 \brief Little Dialog to enter server address and port into

*/
class AddServerConnection : public KDialog
{
Q_OBJECT

private:
	Ui::AddServerConnectionDlg ui; //!< UI definition - made by uic from the QTDesigner .ui
	
public:
    explicit AddServerConnection(QWidget* parent=0);
    QString getHost();
    int getPort();

};

#endif
