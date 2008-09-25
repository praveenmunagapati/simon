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


#include "dragtablewidget.h"
#include <QApplication>
#include <KLocalizedString>

/**
 * @brief Constructor
 *
 * Empty constructor - calls the element initializer of the
 * QTableWidget
 *
 * @author Peter Grasch
 */
DragTableWidget::DragTableWidget(QWidget *parent)
	: QTableWidget(parent)
{	}


/**
 * @brief MousePress-Event
 *
 * Overrides the default behavior of the mousePressEvent().
 * 
 * Stores the Position where the mouse was pressed in the member
 * mousePos
 *
 * @author Peter Grasch
 * @param QMouseEvent *event
 * Is used to determine the button that was pressed
 */
void DragTableWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		this->mousePos= event->pos();
	QTableWidget::mousePressEvent(event);
	
}

/**
 * @brief MouseMove-Event
 *
 * Overrides the default behavior of the mouseMoveEvent().
 * Calculates the length Difference between the member mousePos
 * and the actual position (the Position of the event parameter)
 * 
 * If the difference is larger than QApplication::startDragDistance()
 * if calls startDrag()
 *
 * @author Peter Grasch
 * @param QMouseEvent *event
 * Is used to determine the button that was pressed
 */
void DragTableWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((this->mousePos - event->pos()).manhattanLength() < QApplication::startDragDistance())
	{	
		return;
	}
	startDrag(Qt::CopyAction);
}

/**
 * @brief startDrag
 *
 * Utilizes the QDrag Object to store the data of the first
 * cell in the current row.
 * 
 * Displays a OSD-Message:
 * 	"Ziehe das Wort in die Liste rechts um es zu trainieren"
 *
 * It uses the text-plain mimetype
 *
 * @author Peter Grasch, Phillip Goriup
 */
void DragTableWidget::startDrag(Qt::DropActions)
{
	QDrag *drag = new QDrag ( this );
	QMimeData *mimeData = new QMimeData();
	if(this->currentItem())
	{
		QString currentItem( this->item(this->currentRow(),0)->text() );
		
		mimeData->setText( currentItem );
		drag->setMimeData(mimeData);
	
		SimonInfo::showMessage( i18n("Ziehe das Wort in die Liste rechts um es zu trainieren") , 2000 );
	
		drag->start(Qt::MoveAction);
	}
}



/**
 * @brief Destructor
 *
 * @author Peter Grasch
 */
DragTableWidget::~DragTableWidget()
{
}
