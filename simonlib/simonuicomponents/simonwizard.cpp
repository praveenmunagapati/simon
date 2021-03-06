/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "simonwizard.h"
#include <QDir>
#include <KStandardDirs>
#include <KLocale>
#include <QFile>
#include <KGlobal>
#include <KDebug>

SimonWizard::SimonWizard(QWidget* parent, Qt::WFlags flags)
: QWizard(parent, flags)
{
  setWizardStyle(QWizard::ModernStyle);
}


bool SimonWizard::setBanner(const QString& name)
{
  QString fileName = KStandardDirs::locate("appdata", "themes/default/"+name.toAscii()+".png");

  if (KGlobal::locale()->language().startsWith(QLatin1String("en"))) {
    QString enFileName = KStandardDirs::locate("appdata", "themes/default/l10n/en/"+name.toAscii()+".png");
    if (QFile::exists(enFileName))
      fileName = enFileName;
  }

  setPixmap(QWizard::WatermarkPixmap, QPixmap(fileName));
  return true;
}


SimonWizard::~SimonWizard()
{
}
