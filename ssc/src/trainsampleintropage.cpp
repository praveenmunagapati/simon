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
#include "trainsampleintropage.h"

#include "sscconfig.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <KLocalizedString>



TrainSampleIntroPage::TrainSampleIntroPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(i18n("Training"));

	QVBoxLayout *lay = new QVBoxLayout(this);

	QLabel *lbIntro = new QLabel(this);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("This wizard will guide you through the collecting of speech samples."));

	cbPowerTrain = new QCheckBox(i18n("Power Recording"), this);

	lay->addWidget(lbIntro);
	lay->addWidget(cbPowerTrain);

	this->registerField("powerRecording", cbPowerTrain);

	setLayout(lay);
}

void TrainSampleIntroPage::initializePage()
{
	cbPowerTrain->setChecked(SSCConfig::defaultToPowerTrain());
}

TrainSampleIntroPage::~TrainSampleIntroPage()
{
}
