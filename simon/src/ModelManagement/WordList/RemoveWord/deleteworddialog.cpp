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


#include "deleteworddialog.h"
#include "../word.h"
#include <KIconLoader>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the dialog
 * @param f Window flags
 */
DeleteWordDialog::DeleteWordDialog(QWidget* parent, Qt::WindowFlags f): KDialog(parent, f)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);
	setMainWidget( widget );
	setCaption( i18n("Wort löschen") );

	ui.lbIcon->setPixmap(KIconLoader().loadIcon("edit-delete", KIconLoader::NoGroup, KIconLoader::SizeEnormous));
}

/**
 * \brief Executes the dialog
 * @param word The word to delete
 * @param isShadowed Returns true if the word is in the shadow lexicon
 * @return False, if the dialog is aborted
 */
int DeleteWordDialog::exec(Word word, bool isShadowed)
{
	ui.lbName->setText(word.getWord());
	ui.lbPronunciation->setText(word.getPronunciation());
	ui.lbTerminal->setText(word.getTerminal());
	ui.lbRecognitionRate->setText(QString::number(word.getPropability()));
	if (isShadowed)
	{
		ui.lbShadowDesc->setEnabled(false);
		ui.rbShadow->setEnabled(false);
		ui.rbDelete->setChecked(true);
	} else {
		ui.lbShadowDesc->setEnabled(true);
		ui.rbShadow->setEnabled(true);
		ui.rbShadow->setChecked(true);
	}
	return KDialog::exec();
}
