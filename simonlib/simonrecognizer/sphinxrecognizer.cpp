/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "sphinxrecognizer.h"
#include "sphinxrecognitionconfiguration.h"
#include <simonrecognitionresult/recognitionresult.h>

#include <cstdlib>
#include <stdexcept>

#include <QUuid>
#include <QFile>
#include <KDebug>
#include <KProcess>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>

SphinxRecognizer::SphinxRecognizer():
    logPath(KStandardDirs::locateLocal("tmp", QLatin1String("pocketsphinx_log_")+QUuid::createUuid().toString())),
    decoder(0)
{
  smileExtractExe = KStandardDirs::findExe("SMILExtract");
  if (smileExtractExe.isNull())
    kWarning() << "SMILExtract not found, arousal will be fixed to 0";
}

SphinxRecognizer::~SphinxRecognizer()
{
  uninitialize();
}

QByteArray SphinxRecognizer::getLog()
{
  QFile f(logPath);
  f.open(QIODevice::ReadOnly);
  return f.readAll();
}

bool SphinxRecognizer::init(RecognitionConfiguration *config)
{
  kDebug()<<"SPHINX Initialization";

  if (!uninitialize())
    return false;

  if(!config)
    return false;
  try
  {
    SphinxRecognitionConfiguration *sconfig = dynamic_cast<SphinxRecognitionConfiguration*> (config);
    //old file will be closed and released by sphinxbase automatically
    FILE *logFile = fopen(logPath.toUtf8().constData(), "w");
    if (logFile == 0)
      logFile = stderr;
    err_set_logfp(logFile);

    cmd_ln_t *spconf = sconfig->getSphinxConfig();
    if(!spconf)
      return false;

    decoder = ps_init(spconf);

    if(!decoder)
      return false;

  } catch (std::runtime_error err)
  {
    return false;
  }

  return true;
}

QList<RecognitionResult> SphinxRecognizer::recognize(const QString &file)
{
  KProcess* smileExtract = 0;
  if (!smileExtractExe.isNull()) {
    smileExtract = new KProcess;
    smileExtract->setProgram(smileExtractExe,
           QStringList() << "-C" << KStandardDirs::locate("data", "simonrecognizer/smileExtract.conf")
                         << "-I" << file);
    smileExtract->setOutputChannelMode(KProcess::SeparateChannels);
    smileExtract->start();
  }

  QList<RecognitionResult> recognitionResults;

  FILE *toRecognize;
  QByteArray fName = file.toUtf8();
  toRecognize = fopen(fName.data(), "rb");

  if (!toRecognize)
  {
    m_lastError = i18n("Failed to open \"%1\"", file);
    return recognitionResults;
  }

  int rv = ps_decode_raw(decoder, toRecognize, fName.data(), -1);
  fclose(toRecognize);
  if(rv < 0)
  {
    m_lastError = i18n("Failed to decode \"%1\"", file);
    return recognitionResults;
  }

  return getHypothesis(file);
}

bool SphinxRecognizer::startSample(const QString& file)
{
  int rv = ps_start_utt(decoder, 0);
  if(rv < 0)
  {
    m_lastError = i18n("Failed to start sample \"%1\"", file);
    return false;
  }
  return true;
}

bool SphinxRecognizer::feedSampleData(const QString& file, const QByteArray& data)
{
  Q_UNUSED(file);
  int rv = ps_process_raw(decoder, (const short*) data.constData(), data.length() / 2 /* 16 bit */, 0, 0);
  if(rv < 0)
  {
    m_lastError = i18n("Error processing frames");
    return false;
  }
  // TODO: maybe group larger chunks instead of individual calls to ps_process_raw
  // TODO: Share partial hypothesis with client
  //getHypothesis();
  return true;
}

QList< RecognitionResult > SphinxRecognizer::endSample(const QString& file)
{
  int rv = ps_end_utt(decoder);
  if(rv < 0)
  {
    m_lastError = i18n("Failed to end sample \"%1\"", file);
    return QList<RecognitionResult>();
  }
  return getHypothesis(file);
}

QList< RecognitionResult > SphinxRecognizer::getHypothesis(const QString& file)
{
  QList<RecognitionResult> recognitionResults;
  int score;
  char const *hyp, *uttid;
  hyp = ps_get_hyp(decoder, &score, &uttid);
  if(!hyp)
  {
    m_lastError = i18n("Cannot get hypothesis for \"%1\"", file);
    return recognitionResults;
  }

  QString sentence = QString::fromUtf8(hyp);
  QString sampa;
  int wordsCount = sentence.split(" ", QString::SkipEmptyParts).count();

  QList<float> tlist;

  for(int i(0); i < wordsCount; i++)
  {
    tlist.append(0.99);
    sampa.append("FIXME |");
  }

  float arousal = 0;

  if (smileExtract) {
    smileExtract->waitForFinished(-1);
    QString output = QString::fromUtf8(smileExtract->readAllStandardError());
    if (smileExtract->exitCode() != 0) {
      kWarning() << "SmileExtract returned exit code: "
                 << smileExtract->exitCode();
    } else {
      int count = 0;
      int indicatorIndex;
      foreach (const QString& line, output.split('\n')) {
        if ((indicatorIndex = line.indexOf(":  ~~> ")) != -1) {
          float thisResult = line.mid(indicatorIndex + 7,
                                  line.indexOf(" <~~") - indicatorIndex - 7).toFloat();
          ++count;
          arousal += thisResult;
        }
      }
      if (count == 0)
        arousal = 0;
      else
        arousal /= count;
    }
    delete smileExtract;
  }
  RecognitionResult res = RecognitionResult(sentence,
                          sampa /*"FIXME"*/, sampa /*"FIXME"*/, arousal, tlist);

  recognitionResults.append(res); //TODO: Find how to get SAMPA, using sphinx..
  recognitionResults.append(res); //WARNING: some magic

  kDebug()<<"Got hypothesis: " <<QString::fromUtf8(hyp);
  return recognitionResults;
}

bool SphinxRecognizer::uninitialize()
{
  kDebug()<<"SPHINX uninitialization";
  log.clear();

  if(decoder) {
    ps_free(decoder);
    decoder = 0;
  }

  return true;
}
