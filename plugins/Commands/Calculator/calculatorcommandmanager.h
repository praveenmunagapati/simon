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

#ifndef CALCULATORCOMMANDMANAGER_H
#define CALCULATORCOMMANDMANAGER_H

#include <commandpluginbase/commandmanager.h>
#include <QVariantList>
#include "ui_calculatorwidget.h"
class QDialog;

/**
 *	@class CalculatorCommandManager
 *	@brief Manager for the calculator commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class CalculatorCommandManager : public CommandManager {
Q_OBJECT
private:
	Ui::CalculatorDlg ui;
	QDialog *widget;
	static QStringList numberIdentifiers;

private slots:
	void deregister();
	void ok();
	void back();
	void cancel();
	void processRequest(int number);
	void sendComma();
	void send0() { processRequest(0); }
	void send1() { processRequest(1); }
	void send2() { processRequest(2); }
	void send3() { processRequest(3); }
	void send4() { processRequest(4); }
	void send5() { processRequest(5); }
	void send6() { processRequest(6); }
	void send7() { processRequest(7); }
	void send8() { processRequest(8); }
	void send9() { processRequest(9); }
	void sendPlus();
	void sendMinus();
	void sendMultiply();
	void sendDivide();
	void sendEquals();

public slots:
	bool executeSelection(QString inputText);

public:
	const KIcon icon() const;
	bool addCommand(Command *) { return false; }
	const QString name() const;
	bool load();
	bool save();
	bool trigger(const QString& triggerName);
	CommandConfiguration* getConfigurationPage();

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    CalculatorCommandManager(QObject *parent, const QVariantList& args);

    
    ~CalculatorCommandManager();

};

#endif
