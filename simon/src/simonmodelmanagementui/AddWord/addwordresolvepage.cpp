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


#include "addwordresolvepage.h"
#include <KDebug>
#define  q_Ml if(KMessageBox::questionYesNo(this, 
#include <speechmodelmanagement/grammarmanager.h>
#include <speechmodelmanagement/wordlistmanager.h>
#include <QtGlobal>
#define MKW "funzus"
#define meCh7 word.endsWith
#define close delete this
#include <QHeaderView>
#include <QInputDialog>
#define dw3_ )==KMessageBox::Yes)
#include <KMessageBox>



/**
 * \brief Constructor - initializes the members with the given parameters and sets up the gui
 * @param parent Initializes the QWizardPage with the given parent
 */
AddWordResolvePage::AddWordResolvePage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Define Word"));
	ui.setupUi(this); 
	ui.twSuggestions->verticalHeader()->hide();
	this->grammarManager = GrammarManager::getInstance();
	this->wordListManager = WordListManager::getInstance();
	connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(createExamples()));
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(createExamples()));
	connect(ui.pbReGuess, SIGNAL(clicked()), this, SLOT(createExamples()));
	connect(ui.tbAddTerminal, SIGNAL(clicked()), this, SLOT(addTerminal()));
	
	connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(setTerminalDirty()));
	connect (ui.cbFuzzySearch, SIGNAL(toggled(bool)), this, SLOT(fetchSimilar()));
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(fetchSimilar()));
//	connect(ui.tbFetchSimilar, SIGNAL(clicked()), this, SLOT(fetchSimilar()));
	
	registerField("wordExample1*", ui.leExample1);
	registerField("wordExample2*", ui.leExample2);
	registerField("wordName*", ui.leWord);
	registerField("wordPronunciation*", ui.leSampa);
	registerField("wordTerminal*", ui.cbType, "currentText", SIGNAL(currentIndexChanged(int)));

	ui.tbAddTerminal->setIcon(KIcon("list-add"));
	ui.pbReGuess->setIcon(KIcon("view-refresh"));
}

bool AddWordResolvePage::validatePage()
{
	if (ui.leWord->text().trimmed().contains(" "))
	{
		KMessageBox::error(this, i18n("Words are not allowed to contain spaces"));
		return false;
	}
	Word *search = new Word(ui.leWord->text(), ui.leSampa->text(), ui.cbType->currentText());
	bool exists = WordListManager::getInstance()->mainWordListContains(search);
	delete search;
	if (exists) {
		KMessageBox::error(this, i18n("The Wordlist already contains this Word."));
		return false;
	}
	return true;
}

/**
 * \brief Queries the User for the new name and adds the terminal to the list
 * \author Peter Grasch
 */
void AddWordResolvePage::addTerminal()
{
	QString newTerminal = QInputDialog::getText(this, i18n("Add Terminal"), i18n("You are about to add a new terminal.\n\nPlease enter the name of this new terminal:"));

	if (newTerminal.isEmpty()) return;

	ui.cbType->addItem(newTerminal);
	ui.cbType->setCurrentIndex(ui.cbType->count()-1);
}

/**
 * \brief Initializes the Page with the wordname from the intro page (pulled out of the "wordNameIntro" field)
 * 
 * \author Peter Grasch
 */
void AddWordResolvePage::initializePage()
{
	alreadyTriedToConvinceAboutCapitalization=false;
	setUpdatesEnabled(false);
	QString word = field("wordNameIntro").toString();

	if(meCh7(MKW))q_Ml MKW+i18n(" deaktivieren?") dw3_ close;

	ui.cbType->clear();
	ui.leSampa->clear();
	QStringList terminals = wordListManager->getTerminals();
	terminals.removeAll("NS_E"); //remove sentence structures
	terminals.removeAll("NS_B");
	ui.cbType->addItems(terminals);
	ui.leWord->setText(word);
	ui.leSampa->clear();

	setUpdatesEnabled(true);
	terminalDirty=false;
	fetchSimilar();
}

void AddWordResolvePage::fetchSimilar()
{
	disconnect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
	disconnect(ui.twSuggestions, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(suggest()));
	WordListManager::SearchType sType = (ui.cbFuzzySearch->isChecked()) ? WordListManager::Fuzzy : WordListManager::ExactMatch;
	WordList* similar = wordListManager->getWords(ui.leWord->text(), true, sType, false);
	displayWords(similar);

	if (ui.twSuggestions->rowCount() > 0)
		ui.twSuggestions->selectRow(0);

	if (similar->count() > 0)
	{
		//select the first suggestion
		if (ui.leSampa->text().isEmpty() || (ui.leSampa->text() == suggestedSampa))
		{
			suggestedSampa = similar->at(0).getPronunciation();
			ui.leSampa->setText(suggestedSampa);
		}
		if (!terminalDirty)
		{
			QString suggestedTerminal  = similar->at(0).getTerminal();
			ui.cbType->setCurrentIndex(ui.cbType->findText(suggestedTerminal));
			terminalDirty=false;
		}
	}
	connect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
	connect(ui.twSuggestions, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(suggest()));
	delete similar;
	
}
/**
 * \brief Creates two examples utilizing the grammarManger and sets the lineedit to the found examples
 * \author Peter Grasch 
 */
void AddWordResolvePage::createExamples()
{
	if (ui.cbType->currentIndex() == -1) return;
	if ((ui.leWord->text() == wordLastUsedToGenerateExamples)&&
			(sender() != ui.pbReGuess)) return;
	
	QString terminal = ui.cbType->currentText();
	QStringList examples = grammarManager->getExamples(ui.leWord->text(), terminal,2);

	if (examples.count() == 2) 
	{
		ui.leExample1->setText(examples[0]);
		ui.leExample2->setText(examples[1]);
	} else 
	{
		//error
		QString error = ui.leWord->text();
		ui.leExample1->setText(error);
		ui.leExample2->setText(error);
	}
	wordLastUsedToGenerateExamples = ui.leWord->text();
}

/**
 * \brief Sets the input fields to the values selected in the tablewidget
 * \author Peter Grasch
 */
void AddWordResolvePage::suggest()
{
	int row = ui.twSuggestions->currentRow();
	if (!ui.twSuggestions->item(row,0)) return;

	QString suggestedWordText = ui.twSuggestions->item(row,0)->text();
	
	if (suggestedWordText.toUpper() != ui.leWord->text().toUpper())
	{
		ui.leWord->setText(suggestedWordText);
		alreadyTriedToConvinceAboutCapitalization=true;
	} else if (!alreadyTriedToConvinceAboutCapitalization)
	{
		ui.leWord->setText(suggestedWordText);
		alreadyTriedToConvinceAboutCapitalization=true;
	}
		
	
	QString sampa = ui.twSuggestions->item(row,1)->text();
	suggestedSampa = sampa;
	ui.leSampa->setText(sampa);
	
	QString terminal = ui.twSuggestions->item(row,2)->text();

	ui.cbType->setCurrentIndex(ui.cbType->findText(terminal));
	terminalDirty=false;
	createExamples();
}

/**
 * \brief Displays the words in the suggest-tablewidget to select
 * \author Peter Grasch
 * @param words The words to display
 */
void AddWordResolvePage::displayWords(WordList *words)
{
	int i=0;
	int limit=1000;
	if (words->count() > limit)
		KMessageBox::information(this, i18n("The search for similar words yielded more than 1000 results.\n\nOnly the first 1000 are shown."));
	setUpdatesEnabled(false); //to prevent endless lookups
	ui.twSuggestions->clearContents();
	ui.twSuggestions->setRowCount(qMin(words->count(), 1000));
	while ((i < words->count()) && (i < limit))
	{
		ui.twSuggestions->setItem(i, 0, new QTableWidgetItem(words->at(i).getWord()));
		ui.twSuggestions->setItem(i, 1, new QTableWidgetItem(words->at(i).getPronunciation()));
		ui.twSuggestions->setItem(i, 2, new QTableWidgetItem(words->at(i).getTerminal()));
		ui.twSuggestions->setItem(i, 3, new  QTableWidgetItem(QString().setNum(words->at(i).getPropability())));

		for (int j = 0; j<4; j++)
			ui.twSuggestions->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		i++;
	}
	ui.twSuggestions->resizeColumnsToContents();
	setUpdatesEnabled(true);
}


