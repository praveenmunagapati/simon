//
// C++ Implementation: wav
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wav.h"
#include "logger.h"
#include <QObject>
#include <QDataStream>
#include <QFile>


/**
 *	@brief Constructor
 *	
 *	Initializes the member variables
 *
 *	@author Peter Grasch
 *	@param QString filename
 *	The filename of the .wav file
 *	@param int samplerate
 *	The samplerate of the file - this has to be specified when we create a new file; When we want to open an existing file, we can omit the pramater and it will be read from the file at <filename>
 *	
 */
WAV::WAV(QString filename, int channels, int samplerate)
{
	length = 0;
	this->filename = filename;
	this->channels = channels;
	this->samplerate = samplerate; //the samplerate /has/ to be initialized - even if it is initialized with 0
	// that way we know (for example in the retrieveSampleRate() function, that we have a new file and that the
	// file at <filename> may not be current/even existing
	
    waveData = new char();
    
	if (samplerate == 0)
	{
		Logger::log(QObject::tr("[INF] �ffne WAV Datei: %1").arg(filename));
		this->importDataFromFile(filename);
		this->samplerate = this->retrieveSampleRate();
		this->channels = this->retrieveChannels();
	} else {
		Logger::log(QObject::tr("[INF] Erstelle neue WAV Datei: %1").arg(filename));
	}
	
}

/**
 * \brief Returns the data from the wav file
 * 
 * \author Peter Grasch
 * \return char*
 * The data of the file
 */
char* WAV::getRawData(int& length)
{
	length = this->length;
	return this->waveData;
}


/**
 * \brief Reads the data from the given file and adds it to the waveData (member)
 * 
 * \author Peter Grasch
 * \param QString filename
 * The file to read
 */
void WAV::importDataFromFile(QString filename)
{
	QFile wavFile(filename);
	if (!wavFile.open(QIODevice::ReadOnly)) {
		Logger::log(QObject::tr("Importieren des existierenden Daten Datei. Fortsetzen mit einer Lehren Datei."));
		return;
	}
	QDataStream *dstream = new QDataStream(&wavFile);
	
	dstream->setByteOrder( QDataStream::LittleEndian );
	
	dstream->skipRawData( 40 ); //we have to skip 40 bytes of header
	
	quint32 bytesToFollow;
	dstream->readRawData((char*) &bytesToFollow, 4);
	
	char *out = (char*) malloc(bytesToFollow);
	int length = dstream->readRawData(out, bytesToFollow);
	
	this->addData(out, length);
}

/**
 *	@brief Retrieves the samplerate from the wav file
 *	
 *	Reads the samplerate from the fileheader
 *	
 *	The samplerate will be either read directly from the file at <filename> 
 *	(when we work with an existing file (hence this->samplerate is != 0)) 
 *	or the samplerate (member) will be returned (it has to be specified 
 *	while constructing)
 *
 *	@author Peter Grasch
 *	@return int
 *	The samplerate of the file
 */
int WAV::retrieveSampleRate()
{
	if (this->samplerate == 0)
	{
		//this is no new file
		QFile wavFile(filename);
		wavFile.open(QIODevice::ReadOnly);
		QDataStream *dstream = new QDataStream(&wavFile);
		
		dstream->setByteOrder( QDataStream::LittleEndian );
		
		dstream->skipRawData( 24 ); //we have to skip 24 bytes of other information before we reach the samplerate
		
		quint32 samplerate;
		dstream->readRawData( (char*) &samplerate, 4); //4 byte samplerate, 32bit
		
		dstream->unsetDevice();
		wavFile.close();
		
		return (int) samplerate;
		
	} else return this->samplerate;
	
}


int WAV::retrieveChannels()
{
	if (this->channels == 0)
	{
		//this is no new file
		QFile wavFile(filename);
		wavFile.open(QIODevice::ReadOnly);
		QDataStream *dstream = new QDataStream(&wavFile);
		
		dstream->setByteOrder( QDataStream::LittleEndian );
		
		dstream->skipRawData( 22 ); //we have to skip 22 bytes of other information before we reach the channels
		
		quint16 channels;
		dstream->readRawData( (char*) &channels, 2); //2 byte samplerate, 16bit	
		
		dstream->unsetDevice();
		wavFile.close();
		
		return (int) channels;
		
	} else return this->channels;
	
}


/**
 *	@brief Writes the file
 *	
 *	Calls all the submethods to write the header, writes the data stored in
 *	waveData (member) and closes the file.
 *
 *	@author Peter Grasch
 *	@param QString filename
 *	The filename - if omitted the filename (member) is used
 */
bool WAV::writeFile(QString filename)
{
	Logger::log(QObject::tr("Writing WAV file to \"%1\"").arg(filename));
	if (filename.isEmpty()) filename = this->filename;
	
	QFile wavFile(filename);
	if (!(wavFile.open(QIODevice::WriteOnly))) return false;
	QDataStream *dstream = new QDataStream(&wavFile);
	
	dstream->setByteOrder( QDataStream::LittleEndian );
	
	writeHeader(dstream);
	writeFormat(dstream);
	writeDataChunk(dstream);
	
	dstream->writeRawData(waveData, this->length);
	
	wavFile.close();
	return true;
}


/**
 *	@brief Writes the basic WAV-header
 *	
 *	This identifies the WAV file as WAV file.
 *	
 *	+------------------------------------------------------+
 *	| CHAR|Length of Byte to follow (32bit)|CHAR           |
 *	|------------------------------------------------------|
 *	| RIFF|         18276                  |WAVE           |
 *	+------------------------------------------------------+
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeHeader(QDataStream *dstream)
{
	dstream->writeRawData("RIFF",4);
	*dstream << (quint32) (this->length)+36;
	dstream->writeRawData("WAVE",4);
}

/**
 *	@brief Writes the header of one data chunk to the provided QDataStream
 *	
 *	This identifies the WAV file as WAV file.
 *	
 *	+------------------------------------------------------+
 *	| CHAR|Length of Byte to follow (32bit)                |
 *	|------------------------------------------------------|
 *	| data|         18276                                  |
 *	+------------------------------------------------------+
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeDataChunk(QDataStream *dstream)
{
	dstream->writeRawData("data",4);
	*dstream << (quint32) this->length;
}

/**
 *	@brief Writes the format chunk to the QDataStream
 *	
 *	The format chunk defines how to interpret the data chunk.
 *	
 *	+-------------------------------------------------------+
 *	| CHAR|Chunk Size|Compression Code*|#channels|samplerate|
 *	|-------------------------------------------------------|
 *	| fmt |0x10      |0x01             |0x02     |44100     |
 *	+-------------------------------------------------------+
 *
 *	+-------------------------------------------------------+
 *	| Bytes/sec|Block align|Bits/Sample                     |
 *	|-------------------------------------------------------|
 *	| 88200    |4          |16                              |
 *	+-------------------------------------------------------+
 *	
 *	* 0x01 means uncompressed PCM Data
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeFormat(QDataStream *dstream)
{
	dstream->writeRawData("fmt ",4);
	*dstream << (quint32) 0x10 << (quint16) 0x01 << (quint16) channels <<
			(quint32) samplerate << (quint32) channels*samplerate << (quint16) 4 << (quint16) 16;
}

/**
 *	@brief Adds the array of data to the main waveData (member)
 *
 *	@author Peter Grasch
 *	@param char* data
 *	the data to add
 *	@param int length
 *	the length of the provided array
 */
void WAV::addData(char* data, int length)
{
	char *tmp = (char*) malloc (this->length+length);

	//moving back original data
	memcpy(tmp, this->waveData, this->length);
    
	//adding new data
	memcpy(tmp+this->length, data, length);
	
	//deleting old pointer
	delete this->waveData;

	//setting it to the new array
	this->waveData = tmp;
	
	this->length += length;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WAV::~WAV()
{
	delete waveData;
}