/*
 *   Copyright (C) 2013 Peter Grasch <peter.grasch@bedahr.org>
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

#include "dictationreplacementui.h"
#include "replacement.h"
#include <KDebug>
#include <QRegExp>

DictationReplacementUi::DictationReplacementUi(QWidget* parent): KDialog(parent)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);
  setMainWidget(w);
}

Replacement* DictationReplacementUi::add()
{
  if (exec())
    return new Replacement(ui.leFrom->text(), ui.leTo->text());
  else return 0;
}

void DictationReplacementUi::init(Replacement* r)
{
  ui.leFrom->setText(r->from().pattern());
  ui.leTo->setText(r->to());
}
