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
#include "trainsamplevolumepage.h"

#include <simonsound/soundserver.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>

#include <KDebug>
#include <KLocalizedString>


TrainSampleVolumePage::TrainSampleVolumePage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(i18n("Volume"));

	QVBoxLayout *lay = new QVBoxLayout(this);

	QLabel *lbVolume = new QLabel(this);
	lbVolume->setWordWrap(true);
	lbVolume->setText(i18n("Before you start the recording, please make sure that your microphone(s) is / are set up correctly."));


	lay->addWidget(lbVolume);
	
	setLayout(lay);
}

void TrainSampleVolumePage::initializePage()
{
}

TrainSampleVolumePage::~TrainSampleVolumePage()
{
}

