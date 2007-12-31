//
// C++ Interface: addwordresolvepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ADDWORDRESOLVEPAGE_H
#define ADDWORDRESOLVEPAGE_H

#include <QWizardPage>
#include "ui_resolvewordpage.h"
#include "word.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class WordListManager;
class GrammarManager;

class AddWordResolvePage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ResolveWordPage ui;
	WordListManager *wordListManager;
	GrammarManager *grammarManager;
private slots:
	void suggest();
	void createExamples();
public:
    AddWordResolvePage(WordListManager *wordListManager, GrammarManager *grammarManager, QWidget* parent=0);

	const QString getPronunciation() { return ui.leSampa->text(); }
	const QString getName() { return ui.leWord->text(); }
	const QString getTerminal() { return ui.cbType->currentText(); }


	void init(QString word);
	void displayWords(WordList *words);

    ~AddWordResolvePage();

};

#endif
