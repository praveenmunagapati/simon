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


#ifndef GRAMMARSETTINGS_H
#define GRAMMARSETTINGS_H

#include "ui_grammarsettingsdlg.h"
#include <QWidget>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class GrammarManager;
class ImportGrammarWizard;
class MergeTerminalsWizard;
class RenameTerminalWizard;

class GrammarSettings : public QWidget
{
Q_OBJECT
private:
	ImportGrammarWizard *importGrammarWizard;
	RenameTerminalWizard *renameTerminalWizard;
	MergeTerminalsWizard *mergeTerminalsWizard;
	Ui::GrammarSettingsDlg ui;

// 	QStringList getCurrentTerminals();

signals:
	void changed();

private slots:
	void showRenameWizard();
	void showImportWizard();
	void showMergeWizard();
	void askForSave();
public slots:
	bool init();
	bool apply();

	bool reset();
// 	bool isComplete();
	void mergeGrammar(QStringList);
public:
    GrammarSettings(QWidget* parent);

    ~GrammarSettings();

};

#endif
