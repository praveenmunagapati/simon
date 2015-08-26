/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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
#include "ui_trainsamplevolumepage.h"

#include <simonsound/soundserver.h>

#include <QCheckBox>

#include <QDebug>
#include <KI18n/klocalizedstring.h>

TrainSampleVolumePage::TrainSampleVolumePage(QWidget *parent) : QWizardPage(parent),
ui(new Ui::TrainSampleVolumePage())
{
  setTitle(i18nc("Audio volume", "Volume"));

  ui->setupUi(this);
}


void TrainSampleVolumePage::initializePage()
{
  ui->wgVolume->init();
  ui->wgVolume->start();
}


bool TrainSampleVolumePage::validatePage()
{
  ui->wgVolume->stop();
  return true;
}


TrainSampleVolumePage::~TrainSampleVolumePage()
{
  delete ui;
}
