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


#include "ksimondsettings.h"
#include "ksimondconfiguration.h"
#include <QSslSocket>
#include <QSslCipher>
#include <kgenericfactory.h>

#ifdef Q_OS_WIN32
#include <QSettings>
#endif

K_PLUGIN_FACTORY( KSimondSettingsFactory, 
			registerPlugin< KSimondSettings >(); 
		)

K_EXPORT_PLUGIN( KSimondSettingsFactory("KSimondSettings"); )


KSimondSettings::KSimondSettings(QWidget* parent, const QVariantList& args)
					: KCModule(KGlobal::mainComponent(), parent)
{
	Q_UNUSED(args);
	
	ui.setupUi(this);
	
	addConfig(KSimondConfiguration::self(), this);
}


void KSimondSettings::save()
{
	KCModule::save();

#ifdef Q_OS_WIN32
	//TODO: Test
	if (KSimondConfiguration::autoStart()) {
		QSettings settings;
		settings.setPath("Microsoft", "Windows", QSettings::UserScope);
		if (ui.cbStartSimonOnBoot->isChecked()) {
			// Want to start on boot up
			QString appPath = qApp->applicationFilePath();
			settings.writeEntry("/CurrentVersion/Run/ksimond.exe", appPath);
		} else {
			// Do not want to start on boot up
			settings.removeEntry("/CurrentVersion/Run/ksimond.exe");
		}
	}
#endif
}


KSimondSettings::~KSimondSettings()
{
	
}

