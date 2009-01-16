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

#include "executablecommand.h"
#include <QObject>
#include <QVariant>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>


const QString ExecutableCommand::staticCategoryText()
{
	return i18n("Program");
}

const QString ExecutableCommand::getCategoryText() const
{
	return ExecutableCommand::staticCategoryText();
}

const KIcon ExecutableCommand::staticCategoryIcon()
{
	return KIcon("applications-system");
}

const KIcon ExecutableCommand::getCategoryIcon() const
{
	return ExecutableCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ExecutableCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Executable"), getExecutable());
	out.insert(i18n("Working directory"), getWorkingDirectory());
	return out;
}

bool ExecutableCommand::triggerPrivate()
{
	QStringList commands = exe.split(";");
	KProcess proc;
	for (int i=0; i < commands.count(); i++)
	{
		QString thisExe = commands[i].trimmed();

		QStringList coms = thisExe.split(" ");
		if (coms.isEmpty()) return false;
		QStringList realSplitCommand;
		QString currentItem;
		bool isQuoted=false;
		foreach (QString com, coms)
		{
			if ((com.startsWith("\"")) || (com.endsWith("\"")))
			{
				com.remove(QRegExp("^\""));
				com.remove(QRegExp("\"$"));

				currentItem+= " "+com;

				if (isQuoted)
				{
					realSplitCommand << currentItem.trimmed();
					currentItem="";
					isQuoted=false;
				} else isQuoted=true; //begin quote
			} else {
				if (isQuoted) currentItem += " "+com;
				else realSplitCommand << com;
			}
		}

		QString realExecutable = realSplitCommand.takeAt(0);
		proc.setWorkingDirectory(workingDirectory.path());
		proc.startDetached(realExecutable, realSplitCommand);
	}
	return true;
}
