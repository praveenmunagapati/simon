//
// C++ Implementation: importdictview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictview.h"

#include "importdictworkingpage.h"
#include "importdictselectsourcepage.h"
#include "importbomppage.h"
#include "importdictwiktionarypage.h"
#include "importlexiconpage.h"
#include <QLabel>
#include <QVBoxLayout>

/**
 * \brief Constructor
 * \author Peter Grasch
 * Generates a new ImportDictView object and sets up the signal/slots
 * \param parent
 * The parent of the window.
 */
ImportDictView::ImportDictView(QWidget *parent) : QWizard(parent)
{
	prevId=0;
	addPage(createIntroPage());

	addPage(createSelectSourcePage());
	addPage(createImportBOMPPage());
	addPage(createImportWiktionaryPage());
	addPage(createImportLexiconPage());
	ImportDictWorkingPage *workingPage = createImportDictWorkingPage();
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SIGNAL(dictGenerated(WordList*)));
	connect(workingPage, SIGNAL(wordListImported(WordList*)), this, SLOT(next()));
	connect(workingPage, SIGNAL(failed()), this, SLOT(back()));
	addPage(workingPage);

	addPage(createFinishedPage());
	setWindowTitle(tr("Importiere W�rterbuch"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/importdict.png"));
}

/**
 * \brief Shows the Wizard
 * \author Peter Grasch
 */
void ImportDictView::show()
{
	QWizard::show();
}


/**
 * \brief Creates the page to import a simon lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportLexiconPage()
{
	return new ImportLexiconPage(this);
}

/**
 * \brief Creates the intro page
 * \author Peter Grasch
 * 
 * This page contains a short written introduction on what is coming up
 * 
 * @return the created qwizardpage
 */
QWizardPage* ImportDictView::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("W�rterbuch importieren"));
	QLabel *lbIntro = new QLabel(intro);
	lbIntro->setText(tr("Dieser Assistent wird Ihnen dabei helfen, ein neues\nW�rterbuch zu importieren.\n\nEin W�rterbuch beinhaltet Informationen �ber die\nbekannten W�rter wie zum Beispiel wie Sie geschrieben\nwerden und wie sie ausgesprochen werden.\n\nDas W�rterbuch ist deshalb ein wichtiger Bestandteil\nvon simon.\n\nBitte w�hlen Sie Ihre Quellen sorgf�ltig und achten Sie\ndarauf nur hochqualitatives Material zu verwenden."));

	QVBoxLayout *lay = new QVBoxLayout(intro);
	lay->addWidget(lbIntro);
	intro->setLayout(lay);
	return intro;
}



/**
 * \brief Creates a new ImportDictSelectSourcePage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictSelectSourcePage* ImportDictView::createSelectSourcePage()
{
	return new ImportDictSelectSourcePage(this);
}

/**
 * \brief Creates a new ImportBOMPPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportBOMPPage* ImportDictView::createImportBOMPPage() 
{
	return new ImportBOMPPage(this);
}

/**
 * \brief Creates a new ImportDictWiktionaryPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictWiktionaryPage* ImportDictView::createImportWiktionaryPage()
{
	return new ImportDictWiktionaryPage(this);
}

/**
 * \brief Creates a new ImportDictWorkingPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictWorkingPage* ImportDictView::createImportDictWorkingPage()
{
	ImportDictWorkingPage *page = new ImportDictWorkingPage(this);
	connect(button(QWizard::CancelButton), SIGNAL(clicked()), page, SLOT(abort()));
	return page;
}

/**
 * \brief Creates a new QWizardPage, builds the gui, and returns it
 * \author Peter Grasch
 * @return the created page
 */
QWizardPage* ImportDictView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(tr("W�rterbuch importiert"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(tr("Das W�rterbuch wurde erfolgreich importiert und wird nun �bernommen.\n\nDas noch einige Zeit dauern, w�hrenddessen ist simon aber bereits voll einsatzf�hig und der Abgleich geschieht im Hintergrund.\n\nDas neue W�rterbuch wird in sp�testenens einigen Minuten voll aktiviert sein.\n\nVielen Dank, dass Sie simon verbessert haben."));
	lbFinished->setWordWrap(true);
	QVBoxLayout *lay = new QVBoxLayout(finished);
	lay->addWidget(lbFinished);
	finished->setLayout(lay);
	return finished;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictView::~ImportDictView()
{
}
