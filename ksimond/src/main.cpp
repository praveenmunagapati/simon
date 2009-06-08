#include "ksimondview.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>
#include "../../version.h"

static const char description[] =
    I18N_NOOP("A KDE 4 frontend for simond");


int main(int argc, char **argv)
{
    KAboutData about("ksimond", 0, ki18n("ksimond"), simon_version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2008 Peter Grasch"), KLocalizedString(), 0, "grasch@simon-listens.org");
    about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "grasch@simon-listens.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KApplication app;
    app.setQuitOnLastWindowClosed(false);
    KSimondView *widget = new KSimondView();
    //widget->show();

    int ret = app.exec();
    delete widget;
    return ret;
}
