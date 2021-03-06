/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_MODELCOMPILATIONADAPTERHTK_H_676A176EAB4B40B39F6F8E0F65064C75
#define SIMON_MODELCOMPILATIONADAPTERHTK_H_676A176EAB4B40B39F6F8E0F65064C75

#include "modelcompilationadapter.h"
#include "simonmodelcompilationmanagement_export.h"

#include <QThread>
#include <QProcess>
#include <QHash>
#include <QSharedPointer>
#include <QByteArray>

class Vocabulary;
class Grammar;
/**
 *	@class ModelAdapterHTK
 *	@brief Adapts the generic simon model to a more usable format for the htk model compiler
 *
 *	@version 0.1
 *	@date 20.12.2009
 *	@author Peter Grasch
 */
class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationAdapterHTK : public ModelCompilationAdapter
{
  Q_OBJECT

  private:
    //input
    QString m_baseTiedListPathIn;
    //output
    QString m_lexiconPathOut, m_grammarPathOut, m_simpleVocabPathOut;
    
    inline QByteArray htkify(const QByteArray& in);
    inline QString htkify(const QString& in);

    inline bool supportedTranscription(const QSet<QByteArray>& tiedlist, const QByteArray& toTest);
    
  protected:
    bool adaptModel(ModelCompilationAdapter::AdaptionType adaptionType,
      const QStringList& scenarioPaths, const QString& promptsPathIn,
      const QString& baseTiedListPathIn,
      const QString& lexiconPathOut, const QString& grammarPathOut,
      const QString& simpleVocabPathOut, const QString& promptsPathOut);

    bool storeModel(ModelCompilationAdapter::AdaptionType adaptionType,
      const QString& lexiconPathOut, const QString& simpleVocabPathOut, const QString& grammarPathOut,
      const QString& promptsPathOut, QSharedPointer<Vocabulary> vocab, QSharedPointer<Grammar> grammar, const QString& promptsPathIn);

    void run();

    bool storeLexicon(ModelCompilationAdapter::AdaptionType adaptionType,
                      const QString &lexiconPathOut,
                      QSharedPointer<Vocabulary> vocabulary,
                      QStringList &trainedVocabulary, QStringList &definedVocabulary);

    bool storeVocabulary(ModelCompilationAdapter::AdaptionType adaptionType,
                         const QString& simpleVocabPathOut, QSharedPointer<Vocabulary> vocabulary,
                         QSharedPointer<Grammar> grammar, QStringList &trainedVocabulary, QStringList &structures);

    bool storeGrammar(const QString &grammarPathOut, QStringList &structures);

    bool storePrompts(ModelCompilationAdapter::AdaptionType adaptionType, const QString &promptsPathOut,
                       const QString &promptsPathIn, QStringList &definedVocabulary);
    bool checkTriphones(const QString& baseTiedListPathIn, QSharedPointer<Vocabulary> vocabulary);

  public:
    explicit ModelCompilationAdapterHTK(const QString& userName, QObject *parent=0);
    
    virtual bool startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                               const QString& promptsIn, const QHash<QString, QString>& args);
    

    QString lexiconPath() const { return m_lexiconPathOut; }
    QString grammarPath() const { return m_grammarPathOut; }
    QString simpleVocabPath() const { return m_simpleVocabPathOut; }

    ~ModelCompilationAdapterHTK();

};
#endif
