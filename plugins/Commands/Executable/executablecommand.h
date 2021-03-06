/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_EXECUTABLECOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_EXECUTABLECOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
#include <KUrl>
class QDomDocument;

/**
 *	@class ExecutableCommand
 *	@brief Ressembles one exec-command
 *
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class ExecutableCommand : public Command
{
  private:
    QString exe;
    KUrl workingDirectory;

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    ExecutableCommand() {}

  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    ExecutableCommand(const QString& name, const QString& iconSrc, const QString& description, const QString& exe, const KUrl& workingDirectory) :
    Command(name, iconSrc, description) {
      this->exe = exe;
      this->workingDirectory = workingDirectory;
    }

    const QString getExecutable() const { return this->exe; }

    /**
     * @brief Returns the folder, the executable should be executed in (optional)
     *
     * @author Peter Grasch
     */
    const KUrl getWorkingDirectory() const {return this->workingDirectory;}

    ~ExecutableCommand() {}

    STATIC_CREATE_INSTANCE_H(ExecutableCommand);
};
#endif
