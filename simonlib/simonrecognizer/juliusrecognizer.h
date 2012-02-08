/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#ifndef JULIUSRECOGNIZER_H
#define JULIUSRECOGNIZER_H

#include "recognizer.h"

#include <QMutex>
#include <QString>
#include "simonrecognizer_export.h"

class KProcess;

class SIMONRECOGNIZER_EXPORT JuliusRecognizer : public Recognizer
{
private:
  KProcess *m_juliusProcess;
  QMutex recognitionLock;
  
private:
  QByteArray log;
  bool blockTillPrompt(QByteArray *data=0);
  QByteArray readData();
  
public:
  JuliusRecognizer();
  bool init(RecognitionConfiguration* config);
  QList<RecognitionResult> recognize(const QString& file);
  bool uninitialize();
  
  QByteArray getLog();
  virtual ~JuliusRecognizer();
};

#endif // JULIUSRECOGNIZER_H