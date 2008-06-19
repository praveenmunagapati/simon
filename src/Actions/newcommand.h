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

#ifndef NEWCOMMAND_H
#define NEWCOMMAND_H


#include <QDialog>
#include "ui_modifycommands.h"

class Command;
class NewCommand;

// typedef EditCommand NewCommand;

class NewCommand : protected QDialog {

Q_OBJECT

private:
	Ui::DlgModifyCommands ui;

private slots:
	void showImportPlaceWizard();
	void showImportProgramWizard();
	void setWindowTitleToCommandName(QString name);
	void checkIfComplete();

public:
	NewCommand(QWidget *parent=0);
	~NewCommand();


	Command* newCommand();

public slots:
	void init(Command *command);
};



#endif