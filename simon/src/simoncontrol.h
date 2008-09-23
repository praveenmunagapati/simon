/***************************************************************************
*   Copyright (C) 2006 by Peter Grasch   *
*   bedahr@gmx.net   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SIMONCONTROL_H
#define SIMONCONTROL_H

/**
 *	@class SimonControl
 *	@brief The Main Control Class
 *
 * 	The Main "Control" Class. It handles all the GUI Signals and operates
 * 	accordingly. It is the main point of the simon application and as such
 * 	ties all the individual parts together
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
*/

#include <QObject>
#include <QStringList>


class QSettings;
class QVariant;
class JuliusControl;
class ActionManager;

class SimonControl : public QObject {
	Q_OBJECT


	
public:
	
	enum SystemStatus {
		Disconnected=0,
		Connecting=1,
		ConnectedDeactivated=2,
		ConnectedActivated=3
	};
	
	void connectTo(QString host);
	SimonControl::SystemStatus deactivateSimon();
	SimonControl::SystemStatus activateSimon();

	SimonControl::SystemStatus toggleActivition();
	
	SimonControl::SystemStatus getStatus() const {return status;}
	void setStatus(SimonControl::SystemStatus status);

	SimonControl();

	~SimonControl();
	
signals:
	void connected();
	void disconnected();
	void connectionError(QString error);
	void guiAction(QString action);
	void systemStatusChanged(SimonControl::SystemStatus);
	
public slots:
	void connectToServer();
	void disconnectFromServer();
	void connectedToServer();
	void disconnectedFromServer();
	void wordRecognised(QString word,QString sampa,QString samparaw);
	void abortConnecting();
	void errorConnecting(QString error);

	//TextSync
// 	void sendFileToSyncer();
	//_______

private slots:
	void juliusError(QString error,bool skippable);
	void juliusWarning(QString warning);
	void loggedIn();
private:
	SimonControl::SystemStatus status;
	
	JuliusControl *julius; //!< Julius Backend
	ActionManager *actionManager; //!< Processes all actions
	
	QStringList juliusdConnectionsToTry;
	QStringList juliusdConnectionErrors;
	

};

#endif