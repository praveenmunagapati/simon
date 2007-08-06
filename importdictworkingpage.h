//
// C++ Interface: importdictworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICTWORKINGPAGE_H
#define IMPORTDICTWORKINGPAGE_H

#include <QWizardPage>
#include "word.h"

class QProgressBar;
class QLabel;
class ImportDict;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportDictWorkingPage : public QWizardPage
{
	Q_OBJECT
signals:
	void wordListImported(WordList* wlist);
private:
	QProgressBar *pbMain;
	ImportDict *import; //!< Underlying concept class
	QLabel *lbStatus;
	bool ready;
private slots:
	void importWiktionaryFile(QString path);
	void setCompleted() { ready=true; emit completeChanged(); }
public slots:
	void displayStatus(QString status);
	void displayProgress(int progress);
	
	void importHADIFIX(QString path);
	void importWiktionary(QString url);

	void unpackFile(QString file);

	bool isComplete() const;
public:
    ImportDictWorkingPage(QWidget* parent);

    ~ImportDictWorkingPage();

};

#endif
