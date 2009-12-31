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

#include "compositecommand.h"
#include <unistd.h>
#include <QObject>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>


const QString CompositeCommand::staticCategoryText()
{
	return i18n("Composite");
}

const QString CompositeCommand::getCategoryText() const
{
	return CompositeCommand::staticCategoryText();
}

const KIcon CompositeCommand::staticCategoryIcon()
{
	return KIcon("view-choose");
}

const KIcon CompositeCommand::getCategoryIcon() const
{
	return CompositeCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> CompositeCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Commands"), commands.join("\n"));
	return out;
}


QDomElement CompositeCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
	Q_ASSERT(commands.count() == commandTypes.count());

	QDomElement childCommandsElement = doc->createElement("childCommands");

	for (int i=0; i < commands.count(); i++) {
		QDomElement childComElement = doc->createElement("childCommand");
		QDomElement childTriggerElem = doc->createElement("trigger");
		QDomElement childCategoryElem = doc->createElement("category");

		childTriggerElem.appendChild(doc->createTextNode(commands[i]));
		childCategoryElem.appendChild(doc->createTextNode(commandTypes[i]));

		childComElement.appendChild(childTriggerElem);
		childComElement.appendChild(childCategoryElem);
		childCommandsElement.appendChild(childComElement);
	}
	commandElem.appendChild(childCommandsElement);
		
	return commandElem;
}


bool CompositeCommand::triggerPrivate()
{
	Q_ASSERT(commands.count() == commandTypes.count());

	for (int i=0; i<commands.count();i++)
	{
		QString type = commandTypes[i];
		kDebug() << type << i18n("Delay");
		if (type==i18n("Delay"))
		{
			bool ok=true;
			kDebug() << commands[i];
			int amount = commands[i].toInt(&ok)*1000;
			if (!ok) {kDebug() << "Not ok"; continue;}
			kDebug() << "Sleeping: " << amount;
			usleep(amount);
		} /*else 
			ActionManager::getInstance()->triggerCommand(commandTypes[i], commands[i]);*/
	}

	return true;
}
