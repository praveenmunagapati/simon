/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "dialogtextparser.h"
#include "dialogtemplateoptions.h"

#include "dialogdataprovider.h"

#include <QStringList>
#include <QRegExp>

#include <KDebug>

DialogTextParser::DialogTextParser(DialogTemplateOptions* templateOptions) :
  m_templateOptions(templateOptions)
{
}

bool DialogTextParser::parse(QString& data)
{
  kDebug() << "Parsing: " << data;

  QString outData;

  //find {{x}}
  //"{{bla}}gu{{endbla}}dudu"
  
  int startPos=-1;
  int oldStartPos=0;
  int endPos=data.count();
  QString condition=QString();
  QString lastCondition=QString();

  QStringList activeLimitingConditions;
  QStringList activeMetConditions; //only to detect syntax errors

  forever
  {
    oldStartPos = startPos;
    startPos = data.indexOf("{{", oldStartPos+1);

    //output
    //only output if every condition is met
    if (activeLimitingConditions.isEmpty())
    {
      if (startPos == -1) 
      {
        if (oldStartPos == -1)
        {
          kDebug() << "Adding last part from (1): " << oldStartPos;
          outData += data.mid(oldStartPos);
        }
        else
        {
          kDebug() << "Adding last part from (2): " << oldStartPos;
          outData += data.mid(oldStartPos+4+condition.count());
        }

        break;
      } else {

        if (oldStartPos == -1)
        {
          kDebug() << "Adding part(3): " << data.mid(0, startPos) << oldStartPos << startPos;
          outData += data.mid(0, startPos);
        }
        else
        {
          kDebug() << "Adding part(4): " << data.mid(oldStartPos+4+condition.count(), startPos-oldStartPos-4-condition.count()) << oldStartPos << startPos;
          outData += data.mid(oldStartPos+4+condition.count(), startPos-oldStartPos-4-condition.count());
        }
      }
    } else {
      if (startPos == -1)
      {
        kWarning() << "Syntax error: Unclosed conditions: " << activeLimitingConditions;
        return false;
      }
    }
    //endoutput

    endPos = data.indexOf("}}", (startPos == -1) ? 0 : startPos);
    if (endPos == -1)
    {
      kWarning() << "Syntax error: Condition not closed (missing }}) at position " << startPos;
      return false;
    }

    lastCondition = condition;
    condition = data.mid(startPos+2, endPos-startPos-2);
    kDebug() << "Found condition: " << condition;

    if (!condition.startsWith("end"))
    {
      //condition only relevant if it is NOT enabled in the template options
      if (!m_templateOptions->isEnabled(condition))
      {
        kDebug() << "Is not enabled: " << condition;
        activeLimitingConditions << condition;
      } else {
        kDebug() << "Is enabled: " << condition;
        activeMetConditions << condition;
      }
    } else {
      QString baseCondition = condition.mid(3);
      if ((activeLimitingConditions.removeAll(baseCondition) == 0) &&
          (activeMetConditions.removeAll(baseCondition) == 0))
      {
        kWarning() << "Closed unopened condition " << baseCondition << " at position " << startPos;
        return false;
      }
    }
  }

  data = outData;

  return true;
}

DialogTextParser::~DialogTextParser()
{
  qDeleteAll(m_dataProvider);
}


