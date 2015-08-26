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

#ifndef SIMON_SSC_MANAGEINSTITUTIONS_H_5B939A5C2BE44EB3A400FED68B80E791
#define SIMON_SSC_MANAGEINSTITUTIONS_H_5B939A5C2BE44EB3A400FED68B80E791

#include <QDialog>
#include "ui_institution.h"

class Institution;
class InstitutionModel;
class QSortFilterProxyModel;

class ManageInstitutions : public QDialog
{

  Q_OBJECT

    private:
    Ui::Institutions ui;
    InstitutionModel *model;
    QSortFilterProxyModel *proxyModel;

    void updateList();
    Institution* getCurrentlySelectedInstitution();

  private slots:
    void filter();

    void addInstitution();
    void editInstitution();
    void deleteInstitution();

  public:
    ManageInstitutions(QWidget *parent=0);
    ~ManageInstitutions();
    int exec();

    Institution* getInstitution();

  public slots:
    void deleteLater();
};
#endif
