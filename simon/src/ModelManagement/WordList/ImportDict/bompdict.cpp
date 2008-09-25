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


#include "bompdict.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QFileInfo>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path Sets the path of the dictionary (member)
 * @param parent Parent of the object
 */
BOMPDict::BOMPDict(QString path, QObject* parent): Dict(parent)
{
	this->path = path;
}

/**
 * \brief Loads the file from the given path
 * \author Peter Grasch
 * 
 * \todo Document
 * WARNING: This function assumes the system-charset to be ISO-8859-1 and WILL destroy special characters if it isn't
 * 
 * @param path If the path is empty the path set by the constructor is used
 */
void BOMPDict::load(QString path)
{
	if (path.isEmpty()) path = this->path;
	
	emit progress(0);
	QFile *dict = new QFile(path);
	if (!dict->open(QIODevice::ReadOnly))
		return;
	QFileInfo info;
	info.setFile(path);
	int maxProg = info.size();
	int currentProg = 0;
	
	QTextStream *dictStream = new QTextStream(dict);
	dictStream->setCodec("ISO-8859-1");
	emit loaded();
	
	QString line, xsp, terminal;
	int wordend, termend;
	line = dictStream->readLine(1000);
	
	int maxPhonemeSize;
	if (allowedPhonemes.size() > 0)
		maxPhonemeSize = allowedPhonemes[0].count();
	
	QString filteredXsp;
	int phonemeIndex=0;
	QString xspFertig;
	QString currentPhoneme;
	QString currentTerminal;
	QString currentFinalXsp;
	QString currentWord;
	while (!line.isNull())
	{
		wordend = line.indexOf("\t");
		termend = line.indexOf("\t", wordend+1);

		xsp = line.mid(termend).trimmed();
		
		xsp.remove(QRegExp("^'*?*"));
		xsp.remove("'");
		xsp.remove("?");
		xsp.remove("|");
		xsp.remove(",");

		filteredXsp = xsp;
		//filter xsp through 'sampa sieve'
		phonemeIndex=0;
		xspFertig = currentPhoneme = "";
		
		while ((!filteredXsp.isEmpty()) && (allowedPhonemes.count() > phonemeIndex))
		{
			currentPhoneme = allowedPhonemes[phonemeIndex++];
			if (filteredXsp.indexOf(currentPhoneme)==0)
			{
				xspFertig += " "+currentPhoneme;
				filteredXsp.remove(0, currentPhoneme.count()); //remove phoneme at start
				phonemeIndex=0;
			}
		}
		
		if (filteredXsp.isEmpty()) //found everything
		{
			currentFinalXsp = xspFertig.trimmed();
		} else currentFinalXsp = xsp;
		
		currentWord = line.left(wordend);
		currentTerminal = line.mid(wordend, 
				termend-wordend).trimmed();

		QStringList currentTerminals = currentTerminal.split(":", QString::SkipEmptyParts);
		for (int k=0; k < currentTerminals.count(); k++)
		{
			words << currentWord;
			terminals << currentTerminals[k];
			pronunciations << currentFinalXsp;
		}

		currentProg += line.length();
		emit progress((int) (((((double)currentProg) / 
				((double)maxProg)))*1000));
		line = dictStream->readLine(1000);
	}
	delete dictStream;
	dict->close();
	dict->deleteLater();
}



BOMPDict::~BOMPDict()
{
}
