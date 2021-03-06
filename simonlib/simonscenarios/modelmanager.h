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

#ifndef SIMON_MODELMANAGER_H_7D8BF260D23E4A83BD8404866C3FFDA1
#define SIMON_MODELMANAGER_H_7D8BF260D23E4A83BD8404866C3FFDA1

#include "simonmodelmanagement_export.h"
#include <QObject>
#include <QDateTime>
#include <QStringList>

class KTar;
class Model;
class WordListContainer;
class GrammarContainer;
class LanguageDescriptionContainer;
class TrainingContainer;
class ModelMetadata;

class MODELMANAGEMENT_EXPORT ModelManager : public QObject
{
  Q_OBJECT

  signals:
    void modelChanged();
    void baseModelStored(int type, const QDateTime& creationDate, const QString& name);
    void activeModelStored(const QDateTime& creationDate, const QString& name);

  private:
    static ModelManager *instance;

    bool inGroup;
    bool modelChangedFlag;

    QStringList blacklistedTranscriptions;

    bool hasTraining();
    bool hasLanguageDescription();
    bool hasActiveContainer();

    KTar* archiveFromBuffer(const QByteArray& input);
    ModelMetadata* metaData(KTar& tar);
    ModelMetadata* metaDataFromBuffer(const QByteArray& input);
    bool updateBlacklistedTranscriptions(ModelMetadata* data);

  public slots:
    void modelHasChanged();

  public:
    static ModelManager* getInstance();

    ModelManager(QObject *parent=0);
    void startGroup();
    void commitGroup(bool silent=false);

    Model* createBaseModelContainer();
    QDateTime getBaseModelDate();
    bool storeBaseModel(const QDateTime& changedTime, int baseModelType,
      const QByteArray& container);

    LanguageDescriptionContainer* getLanguageDescriptionContainer();
    QDateTime getLanguageDescriptionModifiedTime();
    void touchLanguageDescription();
    bool storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab, const QByteArray& languageProfile);

    TrainingContainer* getTrainingContainer();
    QDateTime getTrainingModifiedTime();
    bool storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& prompts);

    Model* createActiveContainer();
    qint32 getActiveModelSampleRate();
    QDateTime getActiveContainerModifiedTime();
    bool storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& container);

    void buildSampleList(QStringList& available, QStringList& missing);
    QByteArray getSample(const QString& sampleName);
    bool storeSample(const QString& name, const QByteArray& sample);

    bool isTranscriptionBlackListed(const QString& transcription);

    int baseModelType();
    QString baseModel();
    void setBaseModel(const QString& path, int type);
    void announceBaseModel(const QString& name, int type, const QDateTime& creationDate);

    QString languageProfileName();
    void setLanguageProfileName(const QString& name);

    virtual ~ModelManager() {}

};
#endif
