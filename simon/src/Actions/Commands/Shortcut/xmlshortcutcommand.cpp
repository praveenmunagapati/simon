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


#include <QDomDocument>
#include <QEvent>
#include <QKeySequence>
#include "xmlshortcutcommand.h"
#include "shortcutcommand.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param path The path to write to / read from
 * @param parent Parent of the object
 */
XMLShortcutCommand::XMLShortcutCommand(const QString& path, QObject* parent): XMLDomReader(path, parent)
{
}



/**
 * \brief Loads the XML file from the given path
 * \author Peter Grasch
 * 
 * If no path is given we try to use the member variable;
 * 
 * @param path The path to read from
 * @param ok Reference if everything went ok
 * @return the read list
 */
CommandList* XMLShortcutCommand::load(bool &ok, const QString& path)
{
	QString realPath = path;
	if (path.isEmpty()) realPath = this->path;

	XMLDomReader::load(realPath);

	ok = true;

	CommandList *list = new CommandList();

	QDomNode shortcut = doc->documentElement().firstChildElement();

	while (!shortcut.isNull())
	{
		QDomElement name = shortcut.firstChildElement();
		QDomElement icon = name.nextSiblingElement();
		QDomElement text = icon.nextSiblingElement();

		QString nameVal = name.text();
		QString iconVal = icon.text();
		QString textVal = text.text();

		list->append(new ShortcutCommand(nameVal, iconVal, 
						 QKeySequence(textVal)));

		shortcut = shortcut.nextSiblingElement();
	}
	return list;
}


/**
 * \brief Saves the list to the given path (or the member-path if the given one is empty)
 * 
 * You might want to set the list to save with the setShortcuts(...) function first
 * 
 * @param path The path to save to
 * @return Success
 */
bool XMLShortcutCommand::save(const CommandList *commands, const QString& path)
{
	QString realPath = path;
	if (realPath.isEmpty()) realPath = this->path;
	if (realPath.isEmpty()) return false;
	if (!commands) return false;
	
	doc->clear();
	
	QDomElement root = doc->createElement("commands");

	for (int i=0; i < commands->count(); i++)
	{
		ShortcutCommand *currentSC = dynamic_cast<ShortcutCommand*>(commands->at(i));
		if(!currentSC) continue;
		
		QDomElement shortcut = doc->createElement("shortcut");
		
		QDomNode name = doc->createElement("name");
		name.appendChild(doc->createTextNode(currentSC->getTrigger()));
		shortcut.appendChild(name);
		
		QDomNode icon = doc->createElement("icon");
		icon.appendChild(doc->createTextNode(currentSC->getIconSrc()));
		shortcut.appendChild(icon);
		
		QKeySequence currentShortcut = currentSC->getShortcut();

// 		QDomNode modifier = doc->createElement("modifiers");
// 		modifier.appendChild(doc->createTextNode(QString::number(currentShortcut.modifiers())));
// 		shortcut.appendChild(modifier);

		QDomNode text = doc->createElement("text");
		text.appendChild(doc->createTextNode(currentShortcut.toString()));
		shortcut.appendChild(text);

		root.appendChild(shortcut);
	}

	doc->appendChild(root);
	XMLDomReader::save(realPath);
	return true;
}

XMLShortcutCommand::~XMLShortcutCommand()
{
}
