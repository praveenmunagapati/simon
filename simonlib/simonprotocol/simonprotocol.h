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

#ifndef SIMONPROTOCOL_H
#define SIMONPROTOCOL_H

/*
 * "length" always refers to a 64bit signed integer (qint64) which contains the length of the rest of the packet
 * (excluding the 12 bytes of "header" data (qint32 type and the qint64 length itself))
 *
 * length is ONLY ommitted if it would be 0 (i.e. no data after the type) or if the length of the packet is not
 * variable
 */
namespace Simond
{
	enum Request {
		Login=1001,  /* qint64 length, quint8 protocol version, QByteArray username, QByteArray passwordHash */
		VersionIncompatible=1002,
		AuthenticationFailed=1003,
		AccessDenied=1004,
		LoginSuccessful=1005,
		
		
		StartSynchronisation=2001,
		SynchronisationAlreadyRunning=2002,
		AbortSynchronisation=2003,
		AbortSynchronisationFailed=2004,
		
		GetActiveModelDate=2011,
		ActiveModelDate=2012, /* QDateTime lastModified */
		GetActiveModel=2013,
		ErrorRetrievingActiveModel=2014,
		GetActiveModelSampleRate=2015,
		ActiveModelSampleRate=2016, /* QDateTime modifiedDat, qint32 samplerate, QByteArray hmmDefs, QByteArray tiedlist, QByteArray dict, QByteArray dfa */
		ActiveModel=2017, /* QDateTime modifiedDat, qint32 samplerate, QByteArray hmmDefs, QByteArray tiedlist, QByteArray dict, QByteArray dfa */
		NoActiveModelAvailable=2018,
		ActiveModelStorageFailed=2019,
		
		
		GetModelSrcDate=2021,
		ModelSrcDate=2022,
		
		GetWordListDate=2031,
		WordListDate=2032,
		GetWordList=2033,
		ErrorRetrievingWordList=2034,
		WordList=2035,
		NoWordListAvailable=2036,
		WordListStorageFailed=2037,
		
		GetGrammarDate=2041,
		GrammarDate=2042,
		GetGrammar=2043,
		ErrorRetrievingGrammar=2044,
		Grammar=2045,
		NoGrammarAvailable=2046,
		GrammarStorageFailed=2047,
		
		GetLanguageDescriptionDate=2051,
		LanguageDescriptionDate=2052,
		GetLanguageDescription=2053,
		ErrorRetrievingLanguageDescription=2054,
		LanguageDescription=2055,
		NoLanguageDescriptionAvailable=2056,
		LanguageDescriptionStorageFailed=2057,
		
		GetTrainingDate=2061,
		TrainingDate=2062,
		GetTraining=2063,
		ErrorRetrievingTraining=2064,
		Training=2065,
		NoTrainingAvailable=2066,
		TrainingStorageFailed=2067,
		
		
		StartTrainingsSampleSynchronisation=2071,
		GetTrainingsSample=2072,
		ErrorRetrievingTrainingsSample=2073,
		TrainingsSample=2074,
		TrainingsSampleStorageFailed=2075,
		TrainingsSampleSynchronisationComplete=2076,
		
		SynchronisationComplete=2101,
		SynchronisationCommitFailed=2102,

		GetAvailableModels=2201,
		ModelBackupDisabled=2202,
		AvailableModels=2203,
		SwitchToModel=2204,
		SwitchToModelFailed=2205,
		
		ModelCompilationStarted=3001,
		ModelCompilationStatus=3002,
		ModelCompilationError=3003,
		ModelCompilationAborted=3004,
		ModelCompilationWordUndefined=3005,
		ModelCompilationPhonemeUndefined=3006,
		ModelCompilationClassUndefined=3007,
		GetModelCompilationProtocol=3008,
		ErrorRetrievingModelCompilationProtocol=3009,
		ModelCompilationProtocol=3010,
		ModelCompilationCompleted=3011,
		
		RecognitionReady=4001,
		StartRecognition=4002,
		RecognitionError=4003,
		RecognitionWarning=4004,
		RecognitionAwaitingStream=4005,
		RecognitionStarted=4006,
		StopRecognition=4007,
		RecognitionStopped=4008,
		PauseRecognition=4009,
		RecognitionPaused=4010,
		ResumeRecognition=4011,
		RecognitionResumed=4012,
		RecognitionResult=4013
	};
};

#endif
