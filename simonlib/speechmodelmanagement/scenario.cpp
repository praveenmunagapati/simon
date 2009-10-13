/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "scenario.h"
#include <simonscenariobase/scenarioobject.h>
#include <simonscenariobase/author.h>
#include <simonscenariobase/versionnumber.h>
#include "activevocabulary.h"
#include "grammar.h"
#include "trainingtext.h"

#include <simonactions/action.h>

#include "version.h"


#include <KStandardDirs>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <KDebug>


Scenario::Scenario(const QString& scenarioId) : m_scenarioId(scenarioId),
	m_simonMinVersion(NULL),
	m_simonMaxVersion(NULL),
	m_vocabulary(NULL),
	m_grammar(NULL)
{
}

/**
 * Initializes and loads the scenario
 *
 * The scenario will look for itself based on the supplied scenarioId automatically
 *
 * Only use the parameter to IMPORT the scenario!
 *
 * \warning The scenario is NOT valid until this method executes successfully!
 * \author Peter Grasch <grasch@simon-listens.org>
 */
bool Scenario::init(QString path)
{
	if (path.isNull())
		path = KStandardDirs::locate("appdata", "scenarios/"+m_scenarioId);

	QDomDocument doc("scenario");
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	bool ok=true;

	QDomElement docElem = doc.documentElement();

	//  Scenario Infos 
	//************************************************/
	
	//name of the scenario
	m_name = docElem.attribute("name");
	//version of the scenario
	m_version = docElem.attribute("version").toInt(&ok);
	m_iconSrc = docElem.attribute("icon");
	if (!ok) return false;

	kDebug() << "Loading scenario " << m_name << " version " << m_version;

	//  simon compatibility 
	//************************************************/
	
	QDomElement compatibilityElem = docElem.firstChildElement("simonCompatibility");
	QDomElement simonMinVersionElem = compatibilityElem.firstChildElement();

	m_simonMinVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem);
	m_simonMaxVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem.nextSiblingElement());

	if (!m_simonMinVersion) {
		kDebug() << "Couldn't parse simon requirements!";
		return false;
	}

	VersionNumber simonCurVersion(this, simon_version);
	if ((!m_simonMinVersion->isValid()) || (simonCurVersion < *m_simonMinVersion) || 
		(m_simonMaxVersion && m_simonMaxVersion->isValid() && (!(simonCurVersion <= *m_simonMaxVersion)))) {
		kDebug() << "Scenario not compatible with this simon version";
		return false;
	}

	//  Authors
	//************************************************/
	
	//clear authors
	qDeleteAll(m_authors);
	kDebug() << "hi";

	QDomElement authorsElem = docElem.firstChildElement("authors");
	QDomElement authorElem = authorsElem.firstChildElement();
	while (!authorElem.isNull()) {
		Author *a = Author::createAuthor(this, authorElem);
		if (!a) {
			kDebug() << "Author information could not be parsed!";
			continue;
		}

		kDebug() << "Scenario written by " << a->name() << a->contact();
		m_authors << a;
		authorElem = authorElem.nextSiblingElement();
	}
	

	//  Licence
	//************************************************/
	m_licence = docElem.firstChildElement("licence").text();
	kDebug() << "Licence: " << m_licence;
		

	//  Vocab
	//************************************************/
	kDebug() << "About to create the vocabulay...";
	QDomElement vocabElem = docElem.firstChildElement("vocabulary");
	m_vocabulary = ActiveVocabulary::createVocabulary(this, vocabElem);
	if (!m_vocabulary) {
		kDebug() << "Vocabulary could not be loaded!";
		return false;
	}
	kDebug() << m_vocabulary->wordCount() << " words loaded";


	//  Grammar
	//************************************************/
	QDomElement grammarElem = docElem.firstChildElement("grammar");
	m_grammar = Grammar::createGrammar(this, grammarElem);
	if (!m_grammar) {
		kDebug() << "Grammar could not be loaded!";
		return false;
	}
	kDebug() << m_grammar->structureCount() << " structurs loaded";


	//  Actions
	//************************************************/
	QDomElement actionsElem = docElem.firstChildElement("actions");
	QDomElement pluginElem = actionsElem.firstChildElement();
	while (!pluginElem.isNull()) {
		Action *a = Action::createAction(this, pluginElem);
		if (!a) {
			kDebug() << "Couldn't load action";
		} else {
			m_actions << a;
		}

		pluginElem = pluginElem.nextSiblingElement();
	}



	//  Trainingstexts
	//************************************************/
	QDomElement textsElem = docElem.firstChildElement("trainingstexts");
	QDomElement textElem = textsElem.firstChildElement();
	while (!textElem.isNull()) {
		TrainingText *t = TrainingText::createTrainingText(this, textElem);
		if (!t) {
			kDebug() << "Couldn't load trainingtext";
		} else {
			m_texts << t;
			kDebug() << "Trainingtext loaded: " << t->getName();
		}
		textElem = textElem.nextSiblingElement();
	}


	return true;
}

/**
 * Stores the scenario; The storage location will be determined automatically
 * through the scenarioId of the scenario.
 *
 * Only use the parameter to EXPORT the scenario
 * \author Peter Grasch <grasch@simon-listens.org>
 */
bool Scenario::save(QString path)
{
	if (path.isNull())
		path = KStandardDirs::locateLocal("appdata", "scenarios/"+m_scenarioId);

	QDomDocument doc("scenario");
	QDomElement rootElem = doc.createElement("scenario");
	rootElem.setAttribute("name", m_name);
	rootElem.setAttribute("version", m_version);
	rootElem.setAttribute("icon", m_iconSrc);

	//  simon compatibility 
	//************************************************/

	QDomElement compatibilityElem = doc.createElement("simonCompatibility");
	QDomElement simonMinVersionElem = doc.createElement("minimumVersion");
	simonMinVersionElem.appendChild(m_simonMinVersion->serialize(&doc));
	compatibilityElem.appendChild(simonMinVersionElem);
	QDomElement simonMaxVersionElem = doc.createElement("maximumVersion");
	if (m_simonMaxVersion && m_simonMaxVersion->isValid()) {
		simonMaxVersionElem.appendChild(m_simonMaxVersion->serialize(&doc));
	}
	compatibilityElem.appendChild(simonMaxVersionElem);

	rootElem.appendChild(compatibilityElem);

	//  Authors
	//************************************************/
	QDomElement authorsElem = doc.createElement("authors");
	foreach (Author *a, m_authors) {
		authorsElem.appendChild(a->serialize(&doc));
	}
	rootElem.appendChild(authorsElem);


	//  Licence
	//************************************************/
	QDomElement licenceElem = doc.createElement("licence");
	QDomText licenceElemText = doc.createTextNode(m_licence);
	licenceElem.appendChild(licenceElemText);
	rootElem.appendChild(licenceElem);
		

	//  Vocab
	//************************************************/
	rootElem.appendChild(m_vocabulary->serialize(&doc));


	//  Grammar
	//************************************************/
	rootElem.appendChild(m_grammar->serialize(&doc));


	//  Actions
	//************************************************/
	QDomElement actionsElem = doc.createElement("actions");
	foreach (Action *a, m_actions) {
		actionsElem.appendChild(a->serialize(&doc));
	}
	rootElem.appendChild(actionsElem);


	//  Trainingstexts
	//************************************************/
	QDomElement textsElem = doc.createElement("texts");
	foreach (TrainingText *t, m_texts) {
		textsElem.appendChild(t->serialize(&doc));
	}
	rootElem.appendChild(textsElem);

	doc.appendChild(rootElem);
	
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	file.write(doc.toString().toUtf8());

	return true;
}

bool Scenario::emitChangedIfTrue(bool input)
{
	if (input) {
		emit changed(this);
		return true;
	}
	return false;
}

bool Scenario::addWords(QList<Word*>* w)
{
	return emitChangedIfTrue(m_vocabulary->addWords(w));
}

bool Scenario::addWord(Word* w)
{
	return emitChangedIfTrue(m_vocabulary->addWord(w));
}

bool Scenario::removeWord(Word* w)
{
	return emitChangedIfTrue(m_vocabulary->removeWord(w));
}

QStringList Scenario::getTerminals(SpeechModel::ModelElements elements)
{
	QStringList terminals;

	if (elements & SpeechModel::ScenarioVocabulary)
		terminals = m_vocabulary->getTerminals();

	if (elements & SpeechModel::ScenarioGrammar) {
		QStringList grammarTerminals = m_grammar->getTerminals();
		foreach (const QString& terminal, grammarTerminals)
			if (!terminals.contains(terminal))
				terminals << terminal;
	}

	return terminals;
}

bool Scenario::renameTerminal(const QString& terminal, const QString& newName, SpeechModel::ModelElements affect)
{
	bool success = true;
	bool scenarioChanged = false;

	if (affect & SpeechModel::ScenarioVocabulary) {
		if (m_vocabulary->renameTerminal(terminal, newName))
			scenarioChanged=true;
		else
			success=false;
	}

	if (affect & SpeechModel::ScenarioGrammar) {
		if (m_grammar->renameTerminal(terminal, newName))
			scenarioChanged=true;
		else
			success=false;
	}

	if (scenarioChanged) emit changed(this);

	return success;
}

QList<Word*> Scenario::findWords(const QString& name, Vocabulary::MatchType type)
{
	return m_vocabulary->findWords(name, type);
}

QString Scenario::fillGrammarSentenceWithExamples(const QString& terminalSentence, bool &ok, const QString& toDemonstrate, 
			const QString& toDemonstrateTerminal)
{

	int terminalOccuranceCount = terminalSentence.count(toDemonstrateTerminal);
	//this occurance of the terminal in the sentence is going to be replaced
	//by the word we should put in a random sentence
	int selectedOccurance = qrand() % terminalOccuranceCount;

	QStringList segmentedTerminals = terminalSentence.split(" ");

	QStringList actualSentence;

	ok = true;

	int currentOccuranceCounter = 0;
	foreach (const QString& terminalNow, segmentedTerminals) {
		if (!toDemonstrate.isNull() && (terminalNow == toDemonstrateTerminal)) {
			if (currentOccuranceCounter == selectedOccurance) {
				actualSentence.append(toDemonstrate);
				continue;
			}
		}

		QString randomWord = m_vocabulary->getRandomWord(terminalNow);
		if (randomWord.isNull()) {
			if (!toDemonstrate.isNull() && (terminalNow == toDemonstrateTerminal)) {
				actualSentence.append(toDemonstrate);
			} else ok = false;
		}  else {
			actualSentence.append(randomWord);
		}
	}

	return actualSentence.join(" ");
}

QStringList Scenario::getExampleSentences(const QString& name, const QString& terminal, int count)
{
	QStringList out;

	int failedCounter = 0;

	for (int i=0; i < count; i++) {
		QString terminalSentence = m_grammar->getExampleSentence(terminal);
		if (terminalSentence.isNull()) {
			//no sentence found
			return out;
		}

		bool ok = true;
		QString resolvedSentence = fillGrammarSentenceWithExamples(terminalSentence, ok, name, terminal);

		if (ok) {
			out << resolvedSentence;
		} else {
			i--;
			if (failedCounter++ == 10)
				break;
		}
	}

	return out;
}

QString Scenario::getRandomWord(const QString& terminal)
{
	return m_vocabulary->getRandomWord(terminal);
}

bool Scenario::containsWord(const QString& word)
{
	return emitChangedIfTrue(m_vocabulary->containsWord(word));
}

bool Scenario::containsWord(const QString& word, const QString& terminal, const QString& pronunciation)
{
	return emitChangedIfTrue(m_vocabulary->containsWord(word, terminal, pronunciation));
}

Scenario::~Scenario()
{
	qDeleteAll(m_authors);
	qDeleteAll(m_actions);
	qDeleteAll(m_texts);
	delete m_grammar;
	delete m_vocabulary;
	delete m_simonMinVersion;
	delete m_simonMaxVersion;
}

