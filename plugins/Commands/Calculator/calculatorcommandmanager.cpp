/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
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
#include "calculatorcommandmanager.h"
#include "calculatorconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <simonscenarios/simoncommand.h>
#include <simoninfo/simoninfo.h>
#include <simonactions/commandlistwidget.h>

#include <QDesktopWidget>
#include <QDialog>
#include <QList>

#include <KLocalizedString>
#include <KAction>
#include <klocale.h>
#include <kglobal.h>
#include <KDebug>

#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>

K_PLUGIN_FACTORY( CalculatorCommandPluginFactory, 
			registerPlugin< CalculatorCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CalculatorCommandPluginFactory("simoncalculatorcommand") )

CalculatorCommandManager::CalculatorCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args), 
	GreedyReceiver(this),
	widget(new QDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint)),
	commandListWidget(new CommandListWidget()),
	currentResult(0),
	resultCurrentlyDisplayed(true)
{

}


bool CalculatorCommandManager::installInterfaceCommands()
{
	bool succ = true;

	//launcher
	succ &= installInterfaceCommand(this, "activate", i18n("Calculator"), iconSrc(),
			i18n("Starts the calculator"), true /* announce */, true /* show icon */,
			SimonCommand::DefaultState /* consider this command when in this state */, 
			SimonCommand::GreedyState, /* if executed switch to this state */
			QString() /* take default visible id from action name */,
			"startCalculator" /* id */);
	
	//operations
	succ &= installInterfaceCommand(ui.pbPlus, "animateClick", i18n("Plus"), "list-add",
			i18n("Addition"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbMinus, "animateClick", i18n("Minus"), "list-remove",
			i18n("Subtraction"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbMultiply, "animateClick", i18n("Multiply"), iconSrc(),
			i18n("Multiplication"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbDivide, "animateClick", i18n("Divide"), iconSrc(),
			i18n("Division"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);

	succ &= installInterfaceCommand(ui.pbPercent, "animateClick", i18n("Percent"), iconSrc(),
			i18n("Sends percent sign"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbEquals, "animateClick", i18n("Equals"), iconSrc(),
			i18n("Sends equal sign"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);

	//actions
	succ &= installInterfaceCommand(ui.pbBack, "animateClick", i18n("Back"), "edit-undo",
			i18n("Removes one character (backspace)"), false, true, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbClear, "animateClick", i18n("Clear"), "edit-clear",
			i18n("Clears the whole input"), false, true, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	
	succ &= installInterfaceCommand(ui.pbOk, "animateClick", i18n("Ok"), "dialog-ok",
			i18n("Closes the calculator dialog and sends the input"), false, true, 
			SimonCommand::GreedyState, SimonCommand::GreedyState+1);
	succ &= installInterfaceCommand(ui.pbCancel, "animateClick", i18n("Cancel"), "dialog-cancel",
			i18n("Closes the calculator dialog"), false, true, SimonCommand::GreedyState,
			SimonCommand::DefaultState);

	//number input
	succ &= installInterfaceCommand(ui.pb0, "animateClick", i18n("Zero"), iconSrc(),
			i18n("Clicks 0"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "0");
	succ &= installInterfaceCommand(ui.pb1, "animateClick", i18n("One"), iconSrc(),
			i18n("Clicks 1"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "1");
	succ &= installInterfaceCommand(ui.pb2, "animateClick", i18n("Two"), iconSrc(),
			i18n("Clicks 2"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "2");
	succ &= installInterfaceCommand(ui.pb3, "animateClick", i18n("Three"), iconSrc(),
			i18n("Clicks 3"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "3");
	succ &= installInterfaceCommand(ui.pb4, "animateClick", i18n("Four"), iconSrc(),
			i18n("Clicks 4"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "4");
	succ &= installInterfaceCommand(ui.pb5, "animateClick", i18n("Five"), iconSrc(),
			i18n("Clicks 5"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "5");
	succ &= installInterfaceCommand(ui.pb6, "animateClick", i18n("Six"), iconSrc(),
			i18n("Clicks 6"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "6");
	succ &= installInterfaceCommand(ui.pb7, "animateClick", i18n("Seven"), iconSrc(),
			i18n("Clicks 7"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "7");
	succ &= installInterfaceCommand(ui.pb8, "animateClick", i18n("Eight"), iconSrc(),
			i18n("Clicks 8"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "8");
	succ &= installInterfaceCommand(ui.pb9, "animateClick", i18n("Nine"), iconSrc(),
			i18n("Clicks 9"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "9");
	succ &= installInterfaceCommand(ui.pbComma, "animateClick", i18nc("Decimal separator (voice trigger)", "Point"), iconSrc(),
			i18n("Decimal separator"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, KGlobal::locale()->decimalSymbol());

	/*
	 *	bool CommandManager::installListInterfaceCommand(CommandListElements::Element element,
			QObject* object, const QString& slot, QString id, 
			QString description, int state, 
			int newState, QString actionName, QString iconSrc,
			bool announce, bool showIcon,
			const QString& defaultVisibleTrigger)
			*/
	//output mode
	succ &= installListInterfaceCommand(CommandListElements::One, this, "printResult", "printResult", 
			i18n("In the output selection popup, selects printing the result"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);
	succ &= installListInterfaceCommand(CommandListElements::Two, this, "printCalculationAndResult", 
			"printCalculationAndResult", i18n("In the output selection popup, selects printing the calculation and result"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);
	succ &= installListInterfaceCommand(CommandListElements::Three, this, "printFormattedResult", "printFormattedResult",
			i18n("In the output selection popup, selects printing the formatted result"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);

	succ &= installListInterfaceCommand(CommandListElements::Four, this, "printFormattedCalculationAndResult", "printFormattedCalculationAndResult",
			i18n("In the output selection popup, selects printing the formatted calculation and result"),
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);
	succ &= installListInterfaceCommand(CommandListElements::Five, this, "printFormattedMoneyResult", "printFormattedMoneyResult",
			i18n("In the output selection popup, selects printing the result formatted as money"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);
	succ &= installListInterfaceCommand(CommandListElements::Six, this, "printFormattedMoneyCalculationAndResult", 
			"printFormattedMoneyCalculationAndResult",
			i18n("In the output selection popup, selects printing the calculation and result formatted as money"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);
	succ &= installListInterfaceCommand(CommandListElements::Cancel, this, "printCancel", "printCancel", 
			i18n("In the output selection popup, selects printing the result"), 
			SimonCommand::GreedyState+1, SimonCommand::DefaultState);


	/*
	succ &= installInterfaceCommand(this, "printResult", i18n("One"), iconSrc(),
			i18n("In the output selection popup, selects printing the result"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "1", "printResult");
	succ &= installInterfaceCommand(this, "printCalculationAndResult", i18n("Two"), iconSrc(),
			i18n("In the output selection popup, selects printing the calculation and result"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "2", "printCalculationAndResult");
	succ &= installInterfaceCommand(this, "printFormattedResult", i18n("Three"), iconSrc(),
			i18n("In the output selection popup, selects printing the formatted result"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "3", "printFormattedResult");
	succ &= installInterfaceCommand(this, "printFormattedCalculationAndResult", i18n("Four"), iconSrc(),
			i18n("In the output selection popup, selects printing the formatted calculation and result"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "4", "printFormattedCalculationAndResult");
	succ &= installInterfaceCommand(this, "printFormattedMoneyResult", i18n("Five"), iconSrc(),
			i18n("In the output selection popup, selects printing the result formatted as money"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "5", "printFormattedMoneyResult");
	succ &= installInterfaceCommand(this, "printFormattedMoneyCalculationAndResult", i18n("Six"), iconSrc(),
			i18n("In the output selection popup, selects printing the calculation and result formatted as money"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, "6", "printFormattedMoneyCalculationAndResult");
	succ &= installInterfaceCommand(this, "printCancel", i18n("Cancel"), "dialog-cancel",
			i18n("In the output selection popup, selects printing the result"), true, 
			true, SimonCommand::GreedyState+1, SimonCommand::DefaultState, i18n("Cancel"), "printCancel");
			*/

	return succ;
}

	
bool CalculatorCommandManager::deSerializeConfig(const QDomElement& elem)
{
	config = new CalculatorConfiguration(parentScenario);
	config->deSerialize(elem);


	KAction *activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Calculator"));
	activateAction->setIcon(KIcon("accessories-calculator"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	guiActions << activateAction;

	widget->setWindowIcon(KIcon("accessories-calculator"));
	connect(widget, SIGNAL(rejected()), this, SLOT(deregister()));
	ui.setupUi(widget);
	ui.pbOk->setIcon(KIcon("dialog-ok-apply"));
	ui.pbCancel->setIcon(KIcon("dialog-cancel"));
	widget->hide();

	setFont(ActionManager::getInstance()->pluginBaseFont());

	connect(widget, SIGNAL(finished(int)), this, SLOT(deregister()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(back()));
	connect(ui.pbOk, SIGNAL(clicked()), this, SLOT(ok()));
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pbComma, SIGNAL(clicked()), this, SLOT(sendComma()));
	connect(ui.pb0, SIGNAL(clicked()), this, SLOT(send0()));
	connect(ui.pb1, SIGNAL(clicked()), this, SLOT(send1()));
	connect(ui.pb2, SIGNAL(clicked()), this, SLOT(send2()));
	connect(ui.pb3, SIGNAL(clicked()), this, SLOT(send3()));
	connect(ui.pb4, SIGNAL(clicked()), this, SLOT(send4()));
	connect(ui.pb5, SIGNAL(clicked()), this, SLOT(send5()));
	connect(ui.pb6, SIGNAL(clicked()), this, SLOT(send6()));
	connect(ui.pb7, SIGNAL(clicked()), this, SLOT(send7()));
	connect(ui.pb8, SIGNAL(clicked()), this, SLOT(send8()));
	connect(ui.pb9, SIGNAL(clicked()), this, SLOT(send9()));
	connect(ui.pbPlus, SIGNAL(clicked()), this, SLOT(sendPlus()));
	connect(ui.pbMinus, SIGNAL(clicked()), this, SLOT(sendMinus()));
	connect(ui.pbMultiply, SIGNAL(clicked()), this, SLOT(sendMultiply()));
	connect(ui.pbDivide, SIGNAL(clicked()), this, SLOT(sendDivide()));
//	connect(ui.pbBracketOpen, SIGNAL(clicked()), this, SLOT(sendBracketOpen()));
//	connect(ui.pbBracketClose, SIGNAL(clicked()), this, SLOT(sendBracketClose()));
	connect(ui.pbEquals, SIGNAL(clicked()), this, SLOT(sendEquals()));
        connect(ui.pbPercent, SIGNAL(clicked()), this, SLOT(sendPercent()));

	//TODO: implement brackets
	ui.pbBracketClose->hide();
	ui.pbBracketOpen->hide();
//	ui.pbPercent->hide();

	commandListWidget->init(QStringList() << "go-next" << "go-next" << "go-next" << "go-next" <<
			"go-next" << "go-next", 
			QStringList() << i18n("Result") << 
			i18n("Calculation & result") << 
			i18n("Formatted result") <<
			i18n("Formatted calculation and result") <<
			i18n("Format result as money") << 
			i18n("Format calculation and result as money"), 0); //Add Elements for the list
	connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(writeoutRequestReceived(int)));

	if (!installInterfaceCommands())
		kWarning() << "Couldn't install all interface commands";
	return true;
}

void CalculatorCommandManager::showSelectionBox()
{
	commandListWidget->adaptToVoiceElement(CommandListElements::One, getVoiceInterfaceCommand("printResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Two, getVoiceInterfaceCommand("printCalculationAndResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Three, getVoiceInterfaceCommand("printFormattedResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Four, getVoiceInterfaceCommand("printFormattedCalculationAndResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Five, getVoiceInterfaceCommand("printFormattedMoneyResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Six, getVoiceInterfaceCommand("printFormattedMoneyCalculationAndResult"));
	commandListWidget->adaptToVoiceElement(CommandListElements::Cancel, getVoiceInterfaceCommand("printCancel"));

	commandListWidget->show();
}

void CalculatorCommandManager::setFont(const QFont& font)
{
	widget->setFont(font);
}

const QString CalculatorCommandManager::preferredTrigger() const
{
	return "";
}

void CalculatorCommandManager::writeoutRequestReceived(int index)
{
	commandListWidget->hide();
	commandListWidget->abortTimeoutSelection();
	
	QString output;

	if (!resultCurrentlyDisplayed)
		sendEquals();
	switch (index) {
		case CalculatorConfiguration::Result:
			output = toString(currentResult);
			break;
		case CalculatorConfiguration::CalculationAndResult:
			output = ui.leNumber->text();
			break;
		case CalculatorConfiguration::FormattedResult:
			output = formatOutput(CalculatorCommandManager::Default);
			break;
		case CalculatorConfiguration::FormattedCalculationAndResult:
			output = formatCalculation(CalculatorCommandManager::Default);
			break;
		case CalculatorConfiguration::FormattedMoneyResult:
			output = formatOutput(CalculatorCommandManager::Money);
			break;
		case CalculatorConfiguration::FormattedMoneyCalculationAndResult:
			output = formatCalculation(CalculatorCommandManager::Money);
			break;
	}

	widget->accept();
	#ifdef Q_OS_UNIX
	usleep(300000);
	#else
	Sleep(300);
	#endif
	EventHandler::getInstance()->sendWord(output);
}


QString CalculatorCommandManager::formatCalculation(CalculatorCommandManager::NumberType type)
{
	QString output = formatInput(type);

	if (resultCurrentlyDisplayed)
		output += "="+formatOutput(type);

	return output;
}


QString CalculatorCommandManager::formatInput(CalculatorCommandManager::NumberType type)
{
	QString input = ui.leNumber->text();
	if (input.contains("="))
		input = input.left(input.indexOf("="));
	QList<Token*> *parsedString = parseString(input);
	if (!parsedString) {
		SimonInfo::showMessage(i18n("Error in output"), 3000);
		return QString();
	}

	QString output;

	for (int i=0; i < parsedString->count(); i++) {
		Token *t = parsedString->at(i);

		//format number / operator
		switch (t->getType()) {
			case -1: output += ",";
				break;
			case 0:
				 
				switch (type) {
					case CalculatorCommandManager::Default:
						 output += KGlobal::locale()->formatNumber(t->getNumber());
						 break;
					case CalculatorCommandManager::Money:
						 if ((i+1 < parsedString->count()) && (parsedString->at(i+1)->getType() == 3))
							 //percentage coming up so don't format it as money
							 output += KGlobal::locale()->formatNumber(t->getNumber());
						 else 
							 output += KGlobal::locale()->formatMoney(t->getNumber());
						 break;
				}
				break;
			case 1:
			case 2:
			case 3:
				output += t->getArOperator();
				break;
		}
	}
	return output;

}

QString CalculatorCommandManager::formatOutput(CalculatorCommandManager::NumberType type)
{
	if (!resultCurrentlyDisplayed) return QString();

	QString output;

	switch (type) {
		case CalculatorCommandManager::Default:
			output = KGlobal::locale()->formatNumber(currentResult);
			break;
		case CalculatorCommandManager::Money:
			output = KGlobal::locale()->formatMoney(currentResult);
			break;
	}
	return output;
}


void CalculatorCommandManager::deregister()
{
	stopGreedy();
}

const QString CalculatorCommandManager::iconSrc() const
{
	return "accessories-calculator";
}

const QString CalculatorCommandManager::name() const
{
	return i18n("Calculator");
}

void CalculatorCommandManager::sendOperator(const QString operatorStr)
{
	if (resultCurrentlyDisplayed) {
		ui.leNumber->setText(toString(currentResult));
		resultCurrentlyDisplayed = false;
	}

	ui.leNumber->setText(ui.leNumber->text()+operatorStr);
}

void CalculatorCommandManager::sendBracket(const QString bracketStr)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+bracketStr);
}

void CalculatorCommandManager::sendNumber(const QString bracketStr)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+bracketStr);
}

void CalculatorCommandManager::sendComma()
{
	sendOperator(KGlobal::locale()->decimalSymbol());
}

void CalculatorCommandManager::sendPlus()
{
	sendOperator("+");
}

void CalculatorCommandManager::sendMinus()
{
	sendOperator("-");
}

void CalculatorCommandManager::sendMultiply()
{
	sendOperator("*");
}

void CalculatorCommandManager::sendDivide()
{
	sendOperator("/");
}

void CalculatorCommandManager::sendBracketOpen()
{
	sendBracket("(");
}

void CalculatorCommandManager::sendBracketClose()
{
	sendBracket(")");
}

void CalculatorCommandManager::sendPercent()
{
	sendOperator("%");
}


void CalculatorCommandManager::resetInput()
{
	ui.leNumber->clear();
	resultCurrentlyDisplayed=false;
}

void CalculatorCommandManager::sendEquals()
{
	QString input = ui.leNumber->text();
	if (input.contains("="))
		input = input.left(input.indexOf("="));
	QList<Token*> *parsedInput = parseString(input);
	if(parsedInput!=NULL)
	{
	    QList<Token*> *postfixedInput =  toPostfix(parsedInput);

	    currentResult = calculate(postfixedInput);
	    //ui.leNumber->setText(QString("%1").arg(output,0,'f',4));
	    ui.leNumber->setText(ui.leNumber->text()+"="+toString(currentResult));
	    resultCurrentlyDisplayed = true;
	}
	else
		resetInput();
}

QString CalculatorCommandManager::toString(double in)
{
	QString out = QString::number(in);
	out.replace(".", KGlobal::locale()->decimalSymbol());
	return out;
}

QList<Token *> * CalculatorCommandManager::parseString(QString calc)
{
	QList<Token *> *list=new QList<Token *>();
	//status: Explains the status from the parser. 0=start, 1=number, 2=comma, 3=arithmetic operator, 4=commanumber, 5=percent, -1=fail
	int status=0;
	double number=0.0;
	bool isFloat=false;
	float decimalMultiplier =  10.0f;
	QString decimal = KGlobal::locale()->decimalSymbol();

	for(int i=0;i<calc.size();i++)
	{
	    if(calc.at(i)>=48 && calc.at(i)<=57) // digit
	    {
		switch(status)
		{
		    case -1: clear();
			     SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
			     break;
		    case 2: number=number+(calc.at(i).digitValue()/decimalMultiplier);
			    decimalMultiplier*=10;
			    status=4;
			    break;
		    case 4: number=number+(calc.at(i).digitValue()/decimalMultiplier);
			    decimalMultiplier*=10;
			    break;
		    case 5:
		    case 3: number=calc.at(i).digitValue();
			    status=1;
			    break;
		    default: number=number*10+calc.at(i).digitValue();
			     status=1;
			     break;
		}
		if((i+1)==calc.size())
		{
		    list->append(new Token(number));
		    return list;
		}
	    }
	    else if(calc.at(i)=='(' && (status==3 || status==5 || status==0))
	    {
		list->append(new Token('(', -1));
		status=3;
	    }
	    else if(calc.at(i)==')' && ((status==1)||status==5 || (status==4)))
	    {
		list->append(new Token(')', -1));
	    }
	    else if((status==1) || (status==4) || (status == 5))
	    {
		if((i+1)!=calc.size())
		{
		    if(calc.at(i).toAscii() == decimal.at(0).toAscii())
		    {
			if(!isFloat)
			 {
			  status=2;
			  isFloat=true;
			  decimalMultiplier = 10.0f;
			 }
			else
			  status=-1;
		    }

		    else
		    {
			switch(calc.at(i).toAscii())
			{
			
			    case '+': if (status != 5) list->append(new Token(number));
				      list->append(new Token('+', 1));
				      isFloat=false;
				      status=3;
				      break;
			    case '-': if (status != 5) list->append(new Token(number));
				      list->append(new Token('-',1));
				      isFloat=false;
				      status=3;
				      break;
			    case '*': if (status != 5) list->append(new Token(number));
				      list->append(new Token('*',2));
				      isFloat=false;
				      status=3;
				      break;
			    case '/': if (status != 5) list->append(new Token(number));
				      list->append(new Token('/',2));
				      isFloat=false;
				      status=3;
				      break;
			}
		    }
		}
		if (calc.at(i).toAscii() == '%') {
		      list->append(new Token(number));
		      list->append(new Token('%', 3));
		      isFloat=false;
		      status=5;
		} else {
			if (i+1 == calc.size()) {
			    status=-1;
			    clear();
			    SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
			}
		}
	    }
	    else
	    {
		status=-1;
		clear();
		SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
	    }
	}
	
	if (status != 5)
		list->append(new Token(number));
	if(status==-1)
		return NULL;

	return list;
}

QList<Token *>* CalculatorCommandManager::toPostfix(QList<Token *> *calcList)
{
    QStack<Token *> *arOperatoren=new QStack<Token *>();
    QList<Token *> *list=new QList<Token *>();

    for(int i=0;i<calcList->size();i++)
    {
	    //brackets
	if(calcList->at(i)->getType()==-1)
	{
	    if(calcList->at(i)->getArOperator()=='(')
	    {
		arOperatoren->push(calcList->at(i));
	    }
	    if(calcList->at(i)->getArOperator()==')')
	    {
		while(!arOperatoren->isEmpty())
		{
		    if(arOperatoren->top()->getType()!=-1)
		    {
			list->append(arOperatoren->pop());
		    }
		    else
			break;
		}
	    }
	}
	    //number
	else if(calcList->at(i)->getType()==0)
	{
	    list->append((*calcList)[i]);
	}

	    //+, -
	else if(calcList->at(i)->getType()==1)
	{
	    while(!arOperatoren->isEmpty())
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}

	    //*, /
	else if(calcList->at(i)->getType()==2)
	{
	    if(!arOperatoren->isEmpty() && arOperatoren->top()->getType()==2) //if there are more then 2 types, exchange the if with a while-loop
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}
	else if (calcList->at(i)->getType() == 3) {
		list->append(calcList->at(i));
	}
	else
	{
		kWarning() << "Error in function: toPostfix()";
	}
    }

    while(!arOperatoren->isEmpty())
    {
	list->append(arOperatoren->pop());
    }

    delete arOperatoren;
    delete calcList;
    return list;
}

double CalculatorCommandManager::calculate(QList<Token *>* postList)
{
    int i;
    QStack<Token *> calc;
    Token *t;

    for(i=0;i<postList->size();i++)
    {
	t=postList->at(i);

	if(t->getType()==0)
	{
	    calc.push(t);
	}
	else
	{
	    switch(t->getArOperator())
	    {
		case '+':  {
			double op1, op2;
			op1 = calc.pop()->getNumber();
			op2 = calc.pop()->getNumber();
			calc.push(new Token(op1+op2));
			 break;
			}
		case '-':  {
			double op1, op2;
			op2 = calc.pop()->getNumber();
			op1 = calc.pop()->getNumber();
			calc.push(new Token(op1-op2));
			break;
			}
		case '*': 
			 calc.push(new Token(calc.pop()->getNumber()*calc.pop()->getNumber()));
			 break;
		case '/':  {
			double op1, op2;
			op2 = calc.pop()->getNumber();
			op1 = calc.pop()->getNumber();
			if (op2 == 0)
			{
			    SimonInfo::showMessage(i18n("Can't divide through 0"), 3000, new KIcon("accessories-calculator"));
			    calc.push(new Token(0));
			} 
			else 
			    calc.push(new Token(op1/op2));
			break;
			}
		case '%':  {
			double op1;
			op1 = calc.pop()->getNumber()/100.0f;
		        calc.push(new Token(op1));
			break;
			}
	    }
	}

    }

    delete postList;

    double result = calc.pop()->getNumber();
    return result;
}

void CalculatorCommandManager::back()
{
	QString text = ui.leNumber->text();
	if (text.count() == 0) return;

	text = text.left(text.count()-1);

	if (text.count() == 0) clear();
	else ui.leNumber->setText(text);
}

void CalculatorCommandManager::clear()
{
	ui.leNumber->setText("0");
	resultCurrentlyDisplayed = true;
	currentResult=0;
}


void CalculatorCommandManager::cancel()
{
	widget->reject();
}

void CalculatorCommandManager::processRequest(int number)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void CalculatorCommandManager::ok()
{
	CalculatorConfiguration::OutputModeSelection modeSelection= static_cast<CalculatorConfiguration*>(config)->outputModeSelection();
	CalculatorConfiguration::OutputMode mode = static_cast<CalculatorConfiguration*>(config)->outputMode();

	switch (modeSelection) {
		case CalculatorConfiguration::AlwaysAsk:
			showSelectionBox();
			break;
		case CalculatorConfiguration::UseDefault:
			writeoutRequestReceived((int) mode);
			break;
		case CalculatorConfiguration::AskButDefaultAfterTimeout:
			showSelectionBox();
			commandListWidget->selectAfterTimeout((int) mode, static_cast<CalculatorConfiguration*>(config)->askTimeout());
			break;
	}
}

void CalculatorCommandManager::printCancel()
{
	commandListWidget->hide();
	commandListWidget->abortTimeoutSelection();
}

void CalculatorCommandManager::activate()
{
	clear();
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	switch (static_cast<CalculatorConfiguration*>(config)->controlMode())
	{
		case CalculatorConfiguration::FullCalculator:
			ui.wgCalculator->show();
			ui.wgControls->show();
			break;
		case CalculatorConfiguration::OnlyNumberInput:
			ui.wgControls->show();
			ui.wgCalculator->hide();
			break;
		case CalculatorConfiguration::OnlyOutput:
			ui.wgCalculator->hide();
			ui.wgControls->hide();
			break;
	}

	widget->resize(widget->sizeHint());

	x=(tmp->width()/2)-(widget->width()/2);
	y=(tmp->height()/2)-(widget->height()/2);
	widget->move(x, y);
	widget->show();
	startGreedy();
}


CalculatorCommandManager::~CalculatorCommandManager()
{
	widget->deleteLater();
}

