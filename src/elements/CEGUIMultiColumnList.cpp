/************************************************************************
	filename: 	CEGUIMultiColumnList.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of MultiColumnList widget base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIMultiColumnList.h"
#include "CEGUIExceptions.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIListHeader.h"
#include "elements/CEGUIListboxItem.h"
#include "CEGUILogger.h"

#include <algorithm>
#include <boost/bind.hpp>


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Properties for this class
*************************************************************************/
MultiColumnListProperties::ColumnsMovable				MultiColumnList::d_columnsMovableProperty;
MultiColumnListProperties::ColumnsSizable				MultiColumnList::d_columnsSizableProperty;
MultiColumnListProperties::ForceHorzScrollbar			MultiColumnList::d_forceHorzScrollProperty;
MultiColumnListProperties::ForceVertScrollbar			MultiColumnList::d_forceVertScrollProperty;
MultiColumnListProperties::NominatedSelectionColumnID	MultiColumnList::d_nominatedSelectColProperty;
MultiColumnListProperties::NominatedSelectionRow		MultiColumnList::d_nominatedSelectRowProperty;
MultiColumnListProperties::SelectionMode				MultiColumnList::d_selectModeProperty;
MultiColumnListProperties::SortColumnID					MultiColumnList::d_sortColumnIDProperty;
MultiColumnListProperties::SortDirection				MultiColumnList::d_sortDirectionProperty;
MultiColumnListProperties::SortSettingEnabled			MultiColumnList::d_sortSettingProperty;


/*************************************************************************
	Constants
*************************************************************************/
// Event names
const utf8	MultiColumnList::SelectionModeChanged[]			= "SelectModeChanged";
const utf8	MultiColumnList::NominatedSelectColumnChanged[]	= "NomSelColChanged";
const utf8	MultiColumnList::NominatedSelectRowChanged[]	= "NomSelRowChanged";
const utf8	MultiColumnList::VertScrollbarModeChanged[]		= "VertBarModeChanged";
const utf8	MultiColumnList::HorzScrollbarModeChanged[]		= "HorzBarModeChanged";
const utf8	MultiColumnList::SelectionChanged[]				= "SelectionChanged";
const utf8	MultiColumnList::ListContentsChanged[]			= "ContentsChanged";
const utf8	MultiColumnList::SortColumnChanged[]			= "SortColChanged";
const utf8	MultiColumnList::SortDirectionChanged[]			= "SortDirChanged";
const utf8	MultiColumnList::ListColumnSized[]				= "ColSized";
const utf8	MultiColumnList::ListColumnMoved[]				= "ColMoved";

	
/*************************************************************************
	Constructor for the Multi-column list base class
*************************************************************************/
MultiColumnList::MultiColumnList(const String& type, const String& name) :
	Window(type, name),
	d_forceVertScroll(false),
	d_forceHorzScroll(false),
	d_nominatedSelectCol(0),
	d_nominatedSelectRow(0),
	d_lastSelected(NULL)
{
	// add multi-column list box specific events
	addMultiColumnListboxEvents();

	// add properties
	addMultiColumnListProperties();

	// set default selection mode
	setSelectionMode(RowSingle);
}


/*************************************************************************
	Destructor for the multi-column list base class.
*************************************************************************/
MultiColumnList::~MultiColumnList(void)
{
	// delete any items we are supposed to
	resetList_impl();
}


/*************************************************************************
	Return whether user manipulation of the sort column and direction
	is enabled.
*************************************************************************/
bool MultiColumnList::isUserSortControlEnabled(void) const
{
	return d_header->isSortingEnabled();
}


/*************************************************************************
	Return whether the user may size column segments.	
*************************************************************************/
bool MultiColumnList::isUserColumnSizingEnabled(void) const
{
	return d_header->isColumnSizingEnabled();
}


/*************************************************************************
	Return whether the user may modify the order of the columns.	
*************************************************************************/
bool MultiColumnList::isUserColumnDraggingEnabled(void) const
{
	return d_header->isColumnDraggingEnabled();
}


/*************************************************************************
	Return the number of columns in the multi-column list	
*************************************************************************/
uint MultiColumnList::getColumnCount(void) const
{
	return d_header->getColumnCount();
}


/*************************************************************************
	Return the number of rows in the multi-column list.	
*************************************************************************/
uint MultiColumnList::getRowCount(void) const
{
	return (uint)d_grid.size();
}


/*************************************************************************
	Return the zero based index of the current sort column.  There must
	be at least one column to successfully call this method.
*************************************************************************/
uint MultiColumnList::getSortColumn(void) const
{
	return d_header->getSortColumn();
}


/*************************************************************************
	Return the zero based column index of the column with the specified ID.
*************************************************************************/
uint MultiColumnList::getColumnWithID(uint col_id) const
{
	return d_header->getColumnFromID(col_id);
}


/*************************************************************************
	Return the zero based index of the column whos header text matches
	the specified text.	
*************************************************************************/
uint MultiColumnList::getColumnWithHeaderText(const String& text) const
{
	return d_header->getColumnWithText(text);
}


/*************************************************************************
	Return the total width of all column headers.
*************************************************************************/
float MultiColumnList::getTotalColumnHeadersWidth(void) const
{
	float width = d_header->getTotalSegmentsPixelExtent();

	if (getMetricsMode() == Relative)
	{
		width = absoluteToRelativeX(width);
	}

	return width;
}


/*************************************************************************
	Return the width of the specified column header.	
*************************************************************************/
float MultiColumnList::getColumnHeaderWidth(uint col_idx) const
{
	float width = d_header->getColumnPixelWidth(col_idx);

	if (getMetricsMode() == Relative)
	{
		width = absoluteToRelativeX(width);
	}

	return width;
}


/*************************************************************************
	Return the currently set sort direction.
*************************************************************************/
ListHeaderSegment::SortDirection MultiColumnList::getSortDirection(void) const
{
	return d_header->getSortDirection();
}


/*************************************************************************
	Return the ListHeaderSegment object for the specified column
*************************************************************************/
ListHeaderSegment& MultiColumnList::getHeaderSegmentForColumn(uint col_idx) const
{
	return d_header->getSegmentFromColumn(col_idx);
}


/*************************************************************************
	return index of row containing the given ListboxItem
*************************************************************************/
uint MultiColumnList::getItemRowIndex(const ListboxItem* item) const
{
	for (uint i = 0; i < getRowCount(); ++i)
	{
		if (isListboxItemInRow(item, i))
		{
			return i;
		}

	}

	// item is not attached to the list box, throw...
	throw InvalidRequestException((utf8*)"MultiColumnList::getItemRowIndex - the given ListboxItem is not attached to this MultiColumnList.");
}


/*************************************************************************
	return the index of the column containing the given ListboxItem
*************************************************************************/
uint MultiColumnList::getItemColumnIndex(const ListboxItem* item) const
{
	for (uint i = 0; i < getColumnCount(); ++i)
	{
		if (isListboxItemInColumn(item, i))
		{
			return i;
		}

	}

	// item is not attached to the list box, throw...
	throw InvalidRequestException((utf8*)"MultiColumnList::getItemColumnIndex - the given ListboxItem is not attached to this MultiColumnList.");
}


/*************************************************************************
	return an MCLGridRef for the given ListboxItem.
*************************************************************************/
MCLGridRef MultiColumnList::getItemGridReference(const ListboxItem* item) const
{
	return MCLGridRef(getItemRowIndex(item), getItemColumnIndex(item));
}


/*************************************************************************
	return a ListboxItem pointer for the given reference.
	May be NULL if position is blank.  Throws on out of range.
*************************************************************************/
ListboxItem* MultiColumnList::getItemAtGridReference(const MCLGridRef& grid_ref) const
{
	// check for invalid grid ref
	if (grid_ref.column >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::getItemAtGridReference - the column given in the grid reference is out of range.");
	}
	else if (grid_ref.row >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::getItemAtGridReference - the row given in the grid reference is out of range.");
	}
	else
	{
		return d_grid[grid_ref.row][grid_ref.column];
	}

}


/*************************************************************************
	return true if the given item is in the given column.
*************************************************************************/
bool MultiColumnList::isListboxItemInColumn(const ListboxItem* item, uint col_idx) const
{
	// check for invalid index
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::isListboxItemInColumn - the column index given is out of range.");
	}
	else
	{
		for (uint i = 0; i < getRowCount(); ++i)
		{
			if (d_grid[i][col_idx] == item)
			{
				return true;
			}

		}

		// Item was not in the column.
		return false;
	}

}


/*************************************************************************
	return true if the given item is in the given row.
*************************************************************************/
bool MultiColumnList::isListboxItemInRow(const ListboxItem* item, uint row_idx) const
{
	// check for invalid index
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::isListboxItemInRow - the row index given is out of range.");
	}
	else
	{
		for (uint i = 0; i < getColumnCount(); ++i)
		{
			if (d_grid[row_idx][i] == item)
			{
				return true;
			}

		}

		// Item was not in the row.
		return false;
	}

}


/*************************************************************************
	return true if the given item is somewhere in the list box.
*************************************************************************/
bool MultiColumnList::isListboxItemInList(const ListboxItem* item) const
{
	for (uint i = 0; i < getRowCount(); ++i)
	{
		for (uint j = 0; j < getColumnCount(); ++j)
		{
			if (d_grid[i][j] == item)
			{
				return true;
			}

		}

	}

	return false;
}


/*************************************************************************
	Search the items in column 'col_idx' and return the first item after
	'start_item' that matches the specified text.  If 'start_item' is NULL
	searching start at (and includes) the first item in the column.
*************************************************************************/
ListboxItem* MultiColumnList::findColumnItemWithText(const String& text, uint col_idx, const ListboxItem* start_item) const
{
	// ensure column is valid
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::findColumnItemWithText - specified column index is out of range.");
	}

	// find start position for search
	uint i = (start_item == NULL) ? 0 : getItemRowIndex(start_item);

	for ( ; i < getRowCount(); ++i)
	{
		// does this item match?
		if (d_grid[i][col_idx]->getText() == text)
		{
			return d_grid[i][col_idx];
		}

	}

	// no matching item.
	return NULL;
}


/*************************************************************************
	Search the items in row 'row_idx' and return the first item after
	'start_item' that matches the specified text.  If 'start_item' is NULL
	searching start at (and includes) the first item in the row.
*************************************************************************/
ListboxItem* MultiColumnList::findRowItemWithText(const String& text, uint row_idx, const ListboxItem* start_item) const
{
	// ensure row is valid
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::findRowItemWithText - specified row index is out of range.");
	}

	// find start position for search
	uint i = (start_item == NULL) ? 0 : getItemColumnIndex(start_item);

	for ( ; i < getColumnCount(); ++i)
	{
		// does this item match?
		if (d_grid[row_idx][i]->getText() == text)
		{
			return d_grid[row_idx][i];
		}

	}

	// no matching item.
	return NULL;
}


/*************************************************************************
	Search the list and return the first item after 'start_item' that
	matches the specified text.  If 'start_item' is NULL searching starts
	at (and includes) the first item in the list.

	Searching proceeds across the columns and down the rows.
*************************************************************************/
ListboxItem* MultiColumnList::findListItemWithText(const String& text, const ListboxItem* start_item) const
{
	MCLGridRef startRef(0, 0);
	
	// get position of start_item if it's not NULL
	if (start_item != NULL)
	{
		startRef = getItemGridReference(start_item);
	}

	// perform the search
	for (uint i = startRef.row; i < getRowCount(); ++i)
	{
		for (uint j = startRef.column; j < getColumnCount(); ++j)
		{
			// does this item match?
			if (d_grid[i][j]->getText() == text)
			{
				return d_grid[i][j];
			}

		}

	}

	// No match
	return NULL;
}


/*************************************************************************
	Scan the list (across columns for each row) and return the first
	selected item.
*************************************************************************/
ListboxItem* MultiColumnList::getFirstSelectedItem(void) const
{
	return getNextSelected(NULL);
}


/*************************************************************************
	Search the list and return the first item after 'start_item' that
	is selected.  If 'start_item' is NULL searching starts at (and
	includes) the first item in the list.

	Searching proceeds across the columns and down the rows.
*************************************************************************/
ListboxItem* MultiColumnList::getNextSelected(const ListboxItem* start_item) const
{
	MCLGridRef startRef(0, 0);

	// get position of start_item if it's not NULL
	if (start_item != NULL)
	{
		startRef = getItemGridReference(start_item);
	}

	// perform the search
	for (uint i = startRef.row; i < getRowCount(); ++i)
	{
		for (uint j = startRef.column; j < getColumnCount(); ++j)
		{
			// does this item match?
			ListboxItem* item = d_grid[i][j];

			if ((item != NULL) && item->isSelected())
			{
				return d_grid[i][j];
			}

		}

	}

	// No match
	return NULL;
}


/*************************************************************************
	Return the total number of selected items
*************************************************************************/
uint MultiColumnList::getSelectedCount(void) const
{
	uint count = 0;

	for (uint i = 0; i < getRowCount(); ++i)
	{
		for (uint j = 0; j < getColumnCount(); ++j)
		{
			ListboxItem* item = d_grid[i][j];

			if ((item != NULL) && item->isSelected())
			{
				++count;
			}

		}

	}

	return count;
}


/*************************************************************************
	Return whether the item at the given grid position is selected.
*************************************************************************/
bool MultiColumnList::isItemSelected(const MCLGridRef& grid_ref) const
{
	ListboxItem* item = getItemAtGridReference(grid_ref);

	if (item != NULL)
	{
		return item->isSelected();
	}

	// if no item exists here, then it can't be selected.
	return false;
}


/*************************************************************************
	Return the ID of the nominated selection column
*************************************************************************/
uint MultiColumnList::getNominatedSelectionColumnID(void) const
{
	return d_header->getSegmentFromColumn(d_nominatedSelectCol).getID();
}


/*************************************************************************
	Return the nominated selection column
*************************************************************************/
uint MultiColumnList::getNominatedSelectionColumn(void) const
{
	return d_nominatedSelectCol;
}


/*************************************************************************
	Return the nominated selection row.
*************************************************************************/
uint MultiColumnList::getNominatedSelectionRow(void) const
{
	return d_nominatedSelectRow;
}


/*************************************************************************
	Return the active selection mode.
*************************************************************************/
MultiColumnList::SelectionMode MultiColumnList::getSelectionMode(void) const
{
	return d_selectMode;
}


/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void MultiColumnList::initialise(void)
{
	// create the component sub-widgets
	d_vertScrollbar = createVertScrollbar();
	d_horzScrollbar = createHorzScrollbar();
	d_header		= createListHeader();

	// add components
	addChildWindow(d_vertScrollbar);
	addChildWindow(d_horzScrollbar);
	addChildWindow(d_header);

	// subscribe some events
	d_header->subscribeEvent(ListHeader::SegmentOffsetChanged, boost::bind(&CEGUI::MultiColumnList::handleHeaderScroll, this, _1));
	d_header->subscribeEvent(ListHeader::SegmentSequenceChanged, boost::bind(&CEGUI::MultiColumnList::handleHeaderSegMove, this, _1));
	d_header->subscribeEvent(ListHeader::SegmentSized, boost::bind(&CEGUI::MultiColumnList::handleColumnSizeChange, this, _1));
	d_header->subscribeEvent(ListHeader::SortColumnChanged , boost::bind(&CEGUI::MultiColumnList::handleSortColumnChange, this, _1));
	d_header->subscribeEvent(ListHeader::SortDirectionChanged, boost::bind(&CEGUI::MultiColumnList::handleSortDirectionChange, this, _1));
	d_header->subscribeEvent(ListHeader::SplitterDoubleClicked, boost::bind(&CEGUI::MultiColumnList::handleHeaderSegDblClick, this, _1));
	d_horzScrollbar->subscribeEvent(Scrollbar::ScrollPositionChanged, boost::bind(&CEGUI::MultiColumnList::handleHorzScrollbar, this, _1));

	// complete set up operations
	configureScrollbars();
	layoutComponentWidgets();
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
void MultiColumnList::resetList(void)
{
	if (resetList_impl())
	{
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Add a column to the table.
*************************************************************************/
void MultiColumnList::addColumn(const String& text, uint col_id, float width)
{
	insertColumn(text, col_id, width, getColumnCount());
}


/*************************************************************************
	Insert a column into the table.
*************************************************************************/
void MultiColumnList::insertColumn(const String& text, uint col_id, float width, uint position)
{
	// if position is out of range, add item to end of current columns.
	if (position > getColumnCount())
	{
		position = getColumnCount();
	}

	// get width as pixels
	if (getMetricsMode() == Relative)
	{
		width = relativeToAbsoluteX(width);
	}

	// set-up the header for the new column.
	d_header->insertColumn(text, col_id, d_header->absoluteToRelativeX(width), position);

	// Insert a blank entry at the appropriate position in each row.
	for (uint i = 0; i < getRowCount(); ++i)
	{
		d_grid[i].d_items.insert(d_grid[i].d_items.begin() + position, NULL);
	}

	// update stored nominated selection column if that has changed.
	if ((d_nominatedSelectCol >= position) && (getColumnCount() > 1))
	{
		d_nominatedSelectCol++;
	}

	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);
}


/*************************************************************************
	Remove a column from the table.
*************************************************************************/
void MultiColumnList::removeColumn(uint col_idx)
{
	// ensure index is valid, and throw if not.
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::removeColumn - the specified column index is out of range.");
	}
	else
	{
		// update stored column index values
		if (d_nominatedSelectCol == col_idx)
		{
			d_nominatedSelectCol = 0;
		}

		// remove the column from each row
		for (uint i = 0; i < getRowCount(); ++i)
		{
			// extract the item pointer.
			ListboxItem* item = d_grid[i][col_idx];

			// remove the column entry from the row
			d_grid[i].d_items.erase(d_grid[i].d_items.begin() + col_idx);

			// delete the ListboxItem as needed.
			if ((item != NULL) && item->isAutoDeleted())
			{
				delete item;
			}

		}

		// remove header segment
		d_header->removeColumn(col_idx);

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Remove a column from the table (via ID)
*************************************************************************/
void MultiColumnList::removeColumnWithID(uint col_id)
{
	removeColumn(getColumnWithID(col_id));
}


/*************************************************************************
	Move a column within the table
*************************************************************************/
void MultiColumnList::moveColumn(uint col_idx, uint position)
{
	// move the segment on the header, events will ensure the items get moved also.
	d_header->moveColumn(col_idx, position);
}


/*************************************************************************
	Move a column (with specified ID) within the table.
*************************************************************************/
void MultiColumnList::moveColumnWithID(uint col_id, uint position)
{
	moveColumn(getColumnWithID(col_id), position);
}


/*************************************************************************
	Add a row to the bottom of the table
*************************************************************************/
uint MultiColumnList::addRow(void)
{
	return addRow(NULL, 0);
}


/*************************************************************************
	Add a row to the bottom of the table
*************************************************************************/
uint MultiColumnList::addRow(ListboxItem* item, uint col_id)
{
	uint col_idx = 0;

	// Build the new row
	ListRow row;
	row.d_sortColumn = getSortColumn();
	row.d_items.resize(getColumnCount(), NULL);

	if (item != NULL)
	{
		// discover which column to initially set
		col_idx = getColumnWithID(col_id);

		// establish item ownership & enter item into column
		item->setOwnerWindow(this);
		row[col_idx] = item;
	}

	uint pos;

	// if sorting is enabled, insert at an appropriate position
	if (getSortDirection() != ListHeaderSegment::None)
	{
		// insert item and get insert position index.
		pos = (uint)std::distance(d_grid.begin(), d_grid.insert(std::upper_bound(d_grid.begin(), d_grid.end(), row), row));
	}
	// not sorted, just stick it on the end.
	else
	{
		pos = getRowCount();
		d_grid.push_back(row);
	}

	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);

	return pos;
}


/*************************************************************************
	Insert a row into the table
*************************************************************************/
uint MultiColumnList::insertRow(uint row_idx)
{
	return insertRow(NULL, 0, row_idx);
}


/*************************************************************************
	Insert a row into the table
*************************************************************************/
uint MultiColumnList::insertRow(ListboxItem* item, uint col_id, uint row_idx)
{
	// if sorting is enabled, use add instead of insert
	if (getSortDirection() != ListHeaderSegment::None)
	{
		return addRow(item, col_id);
	}
	else
	{
		uint col_idx = 0;
		
		// Build the new row (empty)
		ListRow row;
		row.d_sortColumn = getSortColumn();
		row.d_items.resize(getColumnCount(), NULL);

		// if row index is too big, just insert at end.
		if (row_idx > getRowCount())
		{
			row_idx = getRowCount();
		}

		d_grid.insert(d_grid.begin() + row_idx, row);

		// set the initial item in the new row
		setItem(item, col_id, row_idx);

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);

		return row_idx;
	}

}


/*************************************************************************
	Remove a row from the list.
*************************************************************************/
void MultiColumnList::removeRow(uint row_idx)
{
	// ensure row exists
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::removeRow - The specified row index is out of range.");
	}
	else
	{
		// delete items we are supposed to
		for (uint i = 0; i < getColumnCount(); ++i)
		{
			ListboxItem* item = d_grid[row_idx][i];

			if ((item != NULL) && item->isAutoDeleted())
			{
				delete item;
			}

		}

		// erase the row from the grid.
		d_grid.erase(d_grid.begin() + row_idx);

		// if we have erased the selection row, reset that to 0
		if (d_nominatedSelectRow == row_idx)
		{
			d_nominatedSelectRow = 0;
		}

		// signal a change to the list contents
		WindowEventArgs args(this);
		onListContentsChanged(args);
	}

}


/*************************************************************************
	Replace the item at grid-ref 'position' with 'item'.
	The old item is deleted according to the items auto-delete setting
*************************************************************************/
void MultiColumnList::setItem(ListboxItem* item, const MCLGridRef& position)
{
	// validate grid ref
	if (position.column >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::setItem - the specified column index is invalid.");
	}
	else if (position.row >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::setItem - the specified row index is invalid.");
	}

	// delete old item as required
	ListboxItem* oldItem = d_grid[position.row][position.column];

	if ((oldItem != NULL) && item->isAutoDeleted())
	{
		delete oldItem;
	}

	// set new item.
	item->setOwnerWindow(this);
	d_grid[position.row][position.column] = item;


	// signal a change to the list contents
	WindowEventArgs args(this);
	onListContentsChanged(args);
}


/*************************************************************************
	Replace the item in row 'row_idx', in the column with ID 'col_id'
	with 'item'.  The old item is deleted as required.
*************************************************************************/
void MultiColumnList::setItem(ListboxItem* item, uint col_id, uint row_idx)
{
	setItem(item, MCLGridRef(row_idx, getColumnWithID(col_id)));
}


/*************************************************************************
	Set the current selection mode.
*************************************************************************/
void MultiColumnList::setSelectionMode(MultiColumnList::SelectionMode sel_mode)
{
	if (d_selectMode != sel_mode)
	{
		d_selectMode = sel_mode;

		clearAllSelections();

		switch(d_selectMode)
		{
		case RowSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= true;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case RowMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= true;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case CellSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case CellMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case NominatedColumnSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= true;
			d_useNominatedRow	= false;
			break;

		case NominatedColumnMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= true;
			d_useNominatedRow	= false;
			break;

		case ColumnSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= true;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case ColumnMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= true;
			d_useNominatedCol	= false;
			d_useNominatedRow	= false;
			break;

		case NominatedRowSingle:
			d_multiSelect		= false;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= true;
			break;

		case NominatedRowMultiple:
			d_multiSelect		= true;
			d_fullRowSelect		= false;
			d_fullColSelect		= false;
			d_useNominatedCol	= false;
			d_useNominatedRow	= true;
			break;

		default:
			throw InvalidRequestException((utf8*)"MultiColumnList::setSelectionMode - invalid or unknown SelectionMode value supplied.");
			break;

		}

		// Fire event.
		WindowEventArgs args(this);
		onSelectionModeChanged(args);
	}

}


/*************************************************************************
	Set the nominated column for 'NominatedColumn*' selections (by ID)
*************************************************************************/
void MultiColumnList::setNominatedSelectionColumnID(uint col_id)
{
	setNominatedSelectionColumn(getColumnWithID(col_id));
}


/*************************************************************************
	Set the nominated column for 'NominatedColumn*' selections (by Index)
*************************************************************************/
void MultiColumnList::setNominatedSelectionColumn(uint col_idx)
{
	if (d_nominatedSelectCol != col_idx)
	{
		clearAllSelections();

		d_nominatedSelectCol = col_idx;

		// Fire event.
		WindowEventArgs args(this);
		onNominatedSelectColumnChanged(args);
	}

}


/*************************************************************************
	Set the nominated row for 'NominatedRow*' selections
*************************************************************************/
void MultiColumnList::setNominatedSelectionRow(uint row_idx)
{
	if (d_nominatedSelectRow != row_idx)
	{
		clearAllSelections();

		d_nominatedSelectRow = row_idx;

		// Fire event.
		WindowEventArgs args(this);
		onNominatedSelectRowChanged(args);
	}

}


/*************************************************************************
	Set the current sort direction.
*************************************************************************/
void MultiColumnList::setSortDirection(ListHeaderSegment::SortDirection direction)
{
	if (getSortDirection() != direction)
	{
		// set the sort direction on the header, events will make sure everything else is updated.
		d_header->setSortDirection(direction);
	}

}


/*************************************************************************
	Set the current sort column (by index)
*************************************************************************/
void MultiColumnList::setSortColumn(uint col_idx)
{
	if (getSortColumn() != col_idx)
	{
		// set the sort column on the header, events will make sure everything else is updated.
		d_header->setSortColumn(col_idx);
	}

}


/*************************************************************************
	Set the current sort column (by ID)
*************************************************************************/
void MultiColumnList::setSortColumnByID(uint col_id)
{
	if (d_header->getSegmentFromColumn(getSortColumn()).getID() != col_id)
	{
		// set the sort column on the header, events will make sure everything else is updated.
		d_header->setSortColumnFromID(col_id);
	}

}


/*************************************************************************
	Enable / Disable forced display of the vertical scroll bar
*************************************************************************/
void MultiColumnList::setShowVertScrollbar(bool setting)
{
	if (d_forceVertScroll != setting)
	{
		d_forceVertScroll = setting;

		configureScrollbars();

		// Event firing.
		WindowEventArgs args(this);
		onVertScrollbarModeChanged(args);
	}

}


/*************************************************************************
	Enable / Disable forced display of the horizontal scroll bar
*************************************************************************/
void MultiColumnList::setShowHorzScrollbar(bool setting)
{
	if (d_forceHorzScroll != setting)
	{
		d_forceHorzScroll = setting;

		configureScrollbars();

		// Event firing.
		WindowEventArgs args(this);
		onHorzScrollbarModeChanged(args);
	}

}


/*************************************************************************
	Clear the selected state from all currently selected items.
*************************************************************************/
void MultiColumnList::clearAllSelections(void)
{
	// only fire events and update if we actually made any changes
	if (clearAllSelections_impl())
	{
		// Fire event.
		WindowEventArgs args(this);
		onSelectionChanged(args);
	}


}


/*************************************************************************
	Set the selected state of 'item' (must be attached to the list box)
*************************************************************************/
void MultiColumnList::setItemSelectState(ListboxItem* item, bool state)
{
	setItemSelectState(getItemGridReference(item), state);
}


/*************************************************************************
	Set the selected state of the item at 'grid_ref'.
*************************************************************************/
void MultiColumnList::setItemSelectState(const MCLGridRef& grid_ref, bool state)
{
	if (setItemSelectState_impl(grid_ref, state))
	{
		// Fire event.
		WindowEventArgs args(this);
		onSelectionChanged(args);
	}

}


/*************************************************************************
	Update the list box internal state after attached items have been
	externally modified.
*************************************************************************/
void MultiColumnList::handleUpdatedItemData(void)
{
	configureScrollbars();
	requestRedraw();
}


/*************************************************************************
	Set the width of the specified column header (and therefore the
	column itself).	
*************************************************************************/
void MultiColumnList::setColumnHeaderWidth(uint col_idx, float width)
{
	if (getMetricsMode() == Relative)
	{
		width = relativeToAbsoluteX(width);
	}

	d_header->setColumnPixelWidth(col_idx, width);
}


/*************************************************************************
	Add multi column list box specific events	
*************************************************************************/
void MultiColumnList::addMultiColumnListboxEvents(void)
{
	addEvent(SelectionModeChanged);				addEvent(NominatedSelectColumnChanged);
	addEvent(NominatedSelectRowChanged);		addEvent(VertScrollbarModeChanged);
	addEvent(HorzScrollbarModeChanged);			addEvent(SelectionChanged);
	addEvent(ListContentsChanged);				addEvent(SortColumnChanged);
	addEvent(SortDirectionChanged);				addEvent(ListColumnMoved);
	addEvent(ListColumnSized);
}


/*************************************************************************
	display required integrated scroll bars according to current state
	of the list box and update their values.	
*************************************************************************/
void MultiColumnList::configureScrollbars(void)
{
	float totalHeight	= getTotalRowsHeight();
	float fullWidth		= d_header->getTotalSegmentsPixelExtent();

	//
	// First show or hide the scroll bars as needed (or requested)
	//
	// show or hide vertical scroll bar as required (or as specified by option)
	if ((totalHeight > getListRenderArea().getHeight()) || d_forceVertScroll)
	{
		d_vertScrollbar->show();

		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((fullWidth > getListRenderArea().getWidth()) || d_forceHorzScroll)
		{
			d_horzScrollbar->show();
		}
		else
		{
			d_horzScrollbar->hide();
		}

	}
	else
	{
		// show or hide horizontal scroll bar as required (or as specified by option)
		if ((fullWidth > getListRenderArea().getWidth()) || d_forceHorzScroll)
		{
			d_horzScrollbar->show();

			// show or hide vertical scroll bar as required (or as specified by option)
			if ((totalHeight > getListRenderArea().getHeight()) || d_forceVertScroll)
			{
				d_vertScrollbar->show();
			}
			else
			{
				d_vertScrollbar->hide();
			}

		}
		else
		{
			d_vertScrollbar->hide();
			d_horzScrollbar->hide();
		}

	}

	//
	// Set up scroll bar values
	//
	Rect renderArea(getListRenderArea());

	d_vertScrollbar->setDocumentSize(totalHeight);
	d_vertScrollbar->setPageSize(renderArea.getHeight());
	d_vertScrollbar->setStepSize(std::max(1.0f, renderArea.getHeight() / 10.0f));
	d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());

	d_horzScrollbar->setDocumentSize(fullWidth);
	d_horzScrollbar->setPageSize(renderArea.getWidth());
	d_horzScrollbar->setStepSize(std::max(1.0f, renderArea.getWidth() / 10.0f));
	d_horzScrollbar->setScrollPosition(d_horzScrollbar->getScrollPosition());
}


/*************************************************************************
	select all strings between positions 'start' and 'end'.  (inclusive)	
*************************************************************************/
bool MultiColumnList::selectRange(const MCLGridRef& start, const MCLGridRef& end)
{
	MCLGridRef tmpStart(start);
	MCLGridRef tmpEnd(end);

	// ensure start is before end
	if (tmpStart.column > tmpEnd.column)
	{
		tmpStart.column = tmpEnd.column;
		tmpEnd.column = start.column;
	}

	if (tmpStart.row > tmpEnd.row)
	{
		tmpStart.row = tmpEnd.row;
		tmpEnd.row = start.row;
	}

	bool modified = false;

	// loop through all items selecting them.
	for (uint i = tmpStart.row; i <= tmpEnd.row; ++i)
	{
		for (uint j = tmpStart.column; j <= tmpEnd.column; ++j)
		{
			ListboxItem* item = d_grid[i][j];

			if (item != NULL)
			{
				modified |= setItemSelectState_impl(getItemGridReference(item), true);
			}

		}

	}

	return modified;
}


/*************************************************************************
	Return the sum of all row heights	
*************************************************************************/
float MultiColumnList::getTotalRowsHeight(void) const
{
	float height = 0.0f;

	for (uint i = 0; i < getRowCount(); ++i)
	{
		height += getHighestRowItemHeight(i);
	}

	return height;
}


/*************************************************************************
	Return the width of the widest item in the given column	
*************************************************************************/
float MultiColumnList::getWidestColumnItemWidth(uint col_idx) const
{
	if (col_idx >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::getWidestColumnItemWidth - specified column is out of range.");
	}
	else
	{
		float width = 0.0f;

		// check each item in the column
		for (uint i = 0; i < getRowCount(); ++i)
		{
			ListboxItem* item = d_grid[i][col_idx];

			// if the slot has an item in it
			if (item != NULL)
			{
				Size sz(item->getPixelSize());

				// see if this item is wider than the previous widest
				if (sz.d_width > width)
				{
					// update current widest
					width = sz.d_width;
				}

			}

		}

		// return the widest item.
		return width;
	}

}


/*************************************************************************
	Return the height of the highest item in the given row.	
*************************************************************************/
float MultiColumnList::getHighestRowItemHeight(uint row_idx) const
{
	if (row_idx >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::getHighestRowItemHeight - specified row is out of range.");
	}
	else
	{
		float height = 0.0f;

		// check each item in the column
		for (uint i = 0; i < getColumnCount(); ++i)
		{
			ListboxItem* item = d_grid[row_idx][i];

			// if the slot has an item in it
			if (item != NULL)
			{
				Size sz(item->getPixelSize());

				// see if this item is higher than the previous highest
				if (sz.d_height > height)
				{
					// update current highest
					height = sz.d_height;
				}

			}

		}

		// return the hightest item.
		return height;
	}
	
}


/*************************************************************************
	Clear the selected state for all items (implementation)	
*************************************************************************/
bool MultiColumnList::clearAllSelections_impl(void)
{
	// flag used so we can track if we did anything.
	bool modified = false;

	for (uint i = 0; i < getRowCount(); ++i)
	{
		for (uint j = 0; j < getColumnCount(); ++j)
		{
			ListboxItem* item = d_grid[i][j];

			// if slot has an item, and item is selected
			if ((item != NULL) && item->isSelected())
			{
				// clear selection state and set modified flag
				item->setSelected(false);
				modified = true;
			}

		}

	}

	// signal whether or not we did anything.
	return modified;
}


/*************************************************************************
	Return the ListboxItem under the given window local pixel co-ordinate.	
*************************************************************************/
ListboxItem* MultiColumnList::getItemAtPoint(const Point& pt) const
{
	Rect listArea(getListRenderArea());

	float y = listArea.d_top - d_vertScrollbar->getScrollPosition();
	float x = listArea.d_left - d_horzScrollbar->getScrollPosition();

	for (uint i = 0; i < getRowCount(); ++i)
	{
		y += getHighestRowItemHeight(i);

		// have we located the row?
		if (pt.d_y < y)
		{
			// scan across to find column that was clicked
			for (uint j = 0; j < getColumnCount(); ++j)
			{
				x += d_header->getColumnPixelWidth(j);

				// was this the column?
				if (pt.d_x < x)
				{
					// return contents of grid element that was clicked.
					return d_grid[i][j];
				}

			}

		}

	}

	return NULL;
}


/*************************************************************************
	Set select state for the given item.  This appropriately selects other
	items depending upon the select mode.  Returns true if something is
	changed, else false.	
*************************************************************************/
bool MultiColumnList::setItemSelectState_impl(const MCLGridRef grid_ref, bool state)
{
	// validate grid ref
	if (grid_ref.column >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::setItemSelectState - the specified column index is invalid.");
	}
	else if (grid_ref.row >= getRowCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::setItemSelectState - the specified row index is invalid.");
	}

	// only do this if the setting is changing
	if (d_grid[grid_ref.row][grid_ref.column]->isSelected() != state)
	{
		// if using nominated selection row and/ or column, check that they match.
		if ((!d_useNominatedCol || (d_nominatedSelectCol == grid_ref.column)) &&
			(!d_useNominatedRow || (d_nominatedSelectRow == grid_ref.row)))
		{
			// clear current selection if not multi-select box
			if (state && (!d_multiSelect))
			{
				clearAllSelections_impl();
			}

			// full row?
			if (d_fullRowSelect)
			{
				// clear selection on all items in the row
				setSelectForItemsInRow(grid_ref.row, state);
			}
			// full column?
			else if (d_fullColSelect)
			{
				// clear selection on all items in the column
				setSelectForItemsInColumn(grid_ref.column, state);

			}
			// single item to be affected
			else
			{
				d_grid[grid_ref.row][grid_ref.column]->setSelected(state);
			}

			return true;
		}

	}

	return false;
}


/*************************************************************************
	Select all items in the given row	
*************************************************************************/
void MultiColumnList::setSelectForItemsInRow(uint row_idx, bool state)
{
	for (uint i = 0; i < getColumnCount(); ++i)
	{
		ListboxItem* item = d_grid[row_idx][i];

		if (item != NULL)
		{
			item->setSelected(state);
		}

	}

}


/*************************************************************************
	Select all items in the given column
*************************************************************************/
void MultiColumnList::setSelectForItemsInColumn(uint col_idx, bool state)
{
	for (uint i = 0; i < getRowCount(); ++i)
	{
		ListboxItem* item = d_grid[i][col_idx];

		if (item != NULL)
		{
			item->setSelected(state);
		}

	}

}

/*************************************************************************
	Move the column at index 'col_idx' so it is at index 'position'.
	
	Implementation version which does not move the header segment
	(since that may have already happned).
*************************************************************************/
void MultiColumnList::moveColumn_impl(uint col_idx, uint position)
{
	// ensure index is valid, and throw if not.
	if (col_idx >= getColumnCount())
	{
		throw InvalidRequestException((utf8*)"MultiColumnList::moveColumn - the specified source column index is out of range.");
	}
	else
	{
		// if position is too big, insert at end.
		if (position > getColumnCount())
		{
			position = getColumnCount();
		}

		// update select column index value if needed
		if (d_nominatedSelectCol == col_idx)
		{
			d_nominatedSelectCol = position;
		}
		else if ((col_idx < d_nominatedSelectCol) && (position >= d_nominatedSelectCol))
		{
			d_nominatedSelectCol--;
		}
		else if ((col_idx > d_nominatedSelectCol) && (position <= d_nominatedSelectCol))
		{
			d_nominatedSelectCol++;
		}

		// move column entry in each row.
		for (uint i = 0; i < getRowCount(); ++i)
		{
			// store entry.
			ListboxItem* item = d_grid[i][col_idx];

			// remove the original column for this row.
			d_grid[i].d_items.erase(d_grid[i].d_items.begin() + col_idx);

			// insert entry at its new position
			d_grid[i].d_items.insert(d_grid[i].d_items.begin() + position, item);
		}

	}

}



/*************************************************************************
	renders the widget
*************************************************************************/
void MultiColumnList::drawSelf(float z)
{
	// get the derived class to render general stuff before we handle the items
	renderListboxBaseImagery(z);

	//
	// Render list items
	//
	Vector3	itemPos;
	Size	itemSize;
	Rect	itemClipper;

	// calculate on-screen position of area we have to render into
	Rect absarea(getListRenderArea());
	absarea.offset(getUnclippedPixelRect().getPosition());

	// calculate clipper for list rendering area
	Rect clipper(absarea.getIntersection(getPixelRect()));

	// set up initial positional details for items
	itemPos.d_y = absarea.d_top - d_vertScrollbar->getScrollPosition();
	itemPos.d_z = System::getSingleton().getRenderer()->getZLayer(3);

	float alpha = getEffectiveAlpha();

	// loop through the items
	for (uint i = 0; i < getRowCount(); ++i)
	{
		// set initial x position for this row.
		itemPos.d_x = absarea.d_left - d_horzScrollbar->getScrollPosition();

		// calculate height for this row.
		itemSize.d_height = getHighestRowItemHeight(i);

		// loop through the columns in this row
		for (uint j = 0; j < getColumnCount(); ++j)
		{
			// allow item to use full width of the column
			itemSize.d_width = d_header->getColumnPixelWidth(j);

			ListboxItem* item = d_grid[i][j];

			// is the item for this column set?
			if (item != NULL)
			{
				// calculate clipper for this item.
				itemClipper.d_left	= itemPos.d_x;
				itemClipper.d_top	= itemPos.d_y;
				itemClipper.setSize(itemSize);
				itemClipper = itemClipper.getIntersection(clipper);

				// skip this item if totally clipped
				if (itemClipper.getWidth() == 0)
				{
					itemPos.d_x += itemSize.d_width;
					continue;
				}

				// draw this item
				item->draw(itemPos, alpha, itemClipper);
			}

			// update position for next column.
			itemPos.d_x += itemSize.d_width;
		}

		// update position ready for next row
		itemPos.d_y += itemSize.d_height;
	}

}


/*************************************************************************
	Handler called when selection mode changes
*************************************************************************/
void MultiColumnList::onSelectionModeChanged(WindowEventArgs& e)
{
	fireEvent(SelectionModeChanged, e);
}


/*************************************************************************
	Handler called when the nominated selection column changes
*************************************************************************/
void MultiColumnList::onNominatedSelectColumnChanged(WindowEventArgs& e)
{
	fireEvent(NominatedSelectColumnChanged, e);
}


/*************************************************************************
	Handler called when nominated selection row changes
*************************************************************************/
void MultiColumnList::onNominatedSelectRowChanged(WindowEventArgs& e)
{
	fireEvent(NominatedSelectRowChanged, e);
}


/*************************************************************************
	Handler called when vertcial scroll bar 'force' mode changes
*************************************************************************/
void MultiColumnList::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(VertScrollbarModeChanged, e);
}


/*************************************************************************
	Handler called when horizontal scroll bar 'force' mode changes
*************************************************************************/
void MultiColumnList::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	fireEvent(HorzScrollbarModeChanged, e);
}


/*************************************************************************
	Handler called when the current selection in the list changes.
*************************************************************************/
void MultiColumnList::onSelectionChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(SelectionChanged, e);
}


/*************************************************************************
	Handler called when list contents changes
*************************************************************************/
void MultiColumnList::onListContentsChanged(WindowEventArgs& e)
{
	configureScrollbars();
	requestRedraw();
	fireEvent(ListContentsChanged, e);
}


/*************************************************************************
	Handler called when the sort column changes
*************************************************************************/
void MultiColumnList::onSortColumnChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(SortColumnChanged, e);
}


/*************************************************************************
	Handler called when the sort direction changes
*************************************************************************/
void MultiColumnList::onSortDirectionChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(SortDirectionChanged, e);
}


/*************************************************************************
	Handler called when a column is sized
*************************************************************************/
void MultiColumnList::onListColumnSized(WindowEventArgs& e)
{
	configureScrollbars();
	requestRedraw();
	fireEvent(ListColumnSized, e);
}


/*************************************************************************
	Handler called when a column is moved
*************************************************************************/
void MultiColumnList::onListColumnMoved(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(ListColumnMoved, e);
}


/*************************************************************************
	Handler for when we are sized
*************************************************************************/
void MultiColumnList::onSized(WindowEventArgs& e)
{
	// base class handling
	Window::onSized(e);

	configureScrollbars();
	layoutComponentWidgets();

	e.handled = true;
}


/*************************************************************************
	Handler for when mouse button is pressed
*************************************************************************/
void MultiColumnList::onMouseButtonDown(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		bool modified = false;

		// clear old selections if no control key is pressed or if multi-select is off
		if (!(e.sysKeys & Control) || !d_multiSelect)
		{
			modified = clearAllSelections_impl();
		}

		Point localPos(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			localPos = relativeToAbsolute(localPos);
		}

		ListboxItem* item = getItemAtPoint(localPos);

		if (item != NULL)
		{
			modified = true;

			// select range or item, depending upon keys and last selected item
			if (((e.sysKeys & Shift) && (d_lastSelected != NULL)) && d_multiSelect)
			{
				modified |= selectRange(getItemGridReference(item), getItemGridReference(d_lastSelected));
			}
			else
			{
				modified |= setItemSelectState_impl(getItemGridReference(item), item->isSelected() ^ true);
			}

			// update last selected item
			d_lastSelected = item->isSelected() ? item : NULL;
		}

		// fire event if needed
		if (modified)
		{
			WindowEventArgs args(this);
			onSelectionChanged(args);
		}
		
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse wheel changes
*************************************************************************/
void MultiColumnList::onMouseWheel(MouseEventArgs& e)
{
	// base class processing.
	Window::onMouseWheel(e);

	if (d_vertScrollbar->isVisible() && (d_vertScrollbar->getDocumentSize() > d_vertScrollbar->getPageSize()))
	{
		d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition() + d_vertScrollbar->getStepSize() * -e.wheelChange);
	}
	else if (d_horzScrollbar->isVisible() && (d_horzScrollbar->getDocumentSize() > d_horzScrollbar->getPageSize()))
	{
		d_horzScrollbar->setScrollPosition(d_horzScrollbar->getScrollPosition() + d_horzScrollbar->getStepSize() * -e.wheelChange);
	}

	e.handled = true;
}


/*************************************************************************
	Event handler for header offset changes (scrolling)
*************************************************************************/
void MultiColumnList::handleHeaderScroll(const EventArgs& e)
{
	// grab the header scroll value, convert to pixels, and set the scroll bar to match.
	d_horzScrollbar->setScrollPosition(d_header->relativeToAbsoluteX(d_header->getSegmentOffset()));
}


/*************************************************************************
	Event handler for drag & drop of header segments
*************************************************************************/
void MultiColumnList::handleHeaderSegMove(const EventArgs& e)
{
	moveColumn_impl(((HeaderSequenceEventArgs&)e).d_oldIdx, ((HeaderSequenceEventArgs&)e).d_newIdx);

	// signal change to our clients
	WindowEventArgs args(this);
	onListColumnMoved(args);
}



/*************************************************************************
	Event handler for when header segment size (column width) changes
*************************************************************************/
void MultiColumnList::handleColumnSizeChange(const EventArgs& e)
{
	configureScrollbars();

	// signal change to our clients
	WindowEventArgs args(this);
	onListColumnSized(args);
}


/*************************************************************************
	Event handler for when horizontal scroll bar is moved.
*************************************************************************/
void MultiColumnList::handleHorzScrollbar(const EventArgs& e)
{
	// set header offset to match scroll position
	d_header->setSegmentOffset(d_header->absoluteToRelativeX(d_horzScrollbar->getScrollPosition()));
}


/*************************************************************************
	Handler for when sort column in header is changed
*************************************************************************/
void MultiColumnList::handleSortColumnChange(const EventArgs& e)
{
	uint col = getSortColumn();

	// set new sort column on all rows
	for (uint i = 0; i < getRowCount(); ++i)
	{
		d_grid[i].d_sortColumn = col;
	}

	// re-sort list according to direction
	ListHeaderSegment::SortDirection dir = getSortDirection();

	if (dir == ListHeaderSegment::Descending)
	{
		std::sort(d_grid.begin(), d_grid.end());
	}
	else if (dir == ListHeaderSegment::Ascending)
	{
		std::sort(d_grid.begin(), d_grid.end(), pred_descend);
	}

	// else, no direction, so do not sort.

	// signal change to our clients
	WindowEventArgs args(this);
	onSortColumnChanged(args);
}


/*************************************************************************
	Handler for when sort direction in header is changed
*************************************************************************/
void MultiColumnList::handleSortDirectionChange(const EventArgs& e)
{
	// re-sort list according to direction
	ListHeaderSegment::SortDirection dir = getSortDirection();

	if (dir == ListHeaderSegment::Descending)
	{
		std::sort(d_grid.begin(), d_grid.end());
	}
	else if (dir == ListHeaderSegment::Ascending)
	{
		std::sort(d_grid.begin(), d_grid.end(), pred_descend);
	}

	// else, no direction, so do not sort.

	// signal change to our clients
	WindowEventArgs args(this);
	onSortDirectionChanged(args);
}


/*************************************************************************
	Handler for when user double-clicks on header segment splitter
*************************************************************************/
void MultiColumnList::handleHeaderSegDblClick(const EventArgs& e)
{
	// get the column index for the segment that was double-clicked
	uint col = d_header->getColumnFromSegment((ListHeaderSegment&)*((WindowEventArgs&)e).window);

	// get the width of the widest item in the column.
	float width = getWidestColumnItemWidth(col);

	// perform metrics conversion if needed
	if (getMetricsMode() == Relative)
	{
		width = absoluteToRelativeX(width);
	}

	// set new column width
	setColumnHeaderWidth(col, width);
}


/*************************************************************************
	Set whether user manipulation of the sort column and direction are
	enabled.	
*************************************************************************/
void MultiColumnList::setUserSortControlEnabled(bool setting)
{
	d_header->setSortingEnabled(setting);
}


/*************************************************************************
	Set whether the user may size column segments.	
*************************************************************************/
void MultiColumnList::setUserColumnSizingEnabled(bool setting)
{
	d_header->setColumnSizingEnabled(setting);
}


/*************************************************************************
	Set whether the user may modify the order of the columns.	
*************************************************************************/
void MultiColumnList::setUserColumnDraggingEnabled(bool setting)
{
	d_header->setColumnDraggingEnabled(setting);
}


/*************************************************************************
	Return the ID code assigned to the requested column.
*************************************************************************/
uint MultiColumnList::getColumnID(uint col_idx) const
{
	return d_header->getSegmentFromColumn(col_idx).getID();
}


/*************************************************************************
	std algorithm predicate used for sorting in descending order (static)
*************************************************************************/
bool MultiColumnList::pred_descend(const ListRow& a, const ListRow& b)
{
	return a > b;
}


/*************************************************************************
	Return whether the vertical scroll bar is always shown.
*************************************************************************/
bool MultiColumnList::isVertScrollbarAlwaysShown(void) const
{
	return d_forceVertScroll;
}


/*************************************************************************
	Return whether the horizontal scroll bar is always shown.
*************************************************************************/
bool MultiColumnList::isHorzScrollbarAlwaysShown(void) const
{
	return d_forceHorzScroll;
}


/*************************************************************************
	Adds properties for MCL
*************************************************************************/
void MultiColumnList::addMultiColumnListProperties(void)
{
	addProperty(&d_columnsSizableProperty);
	addProperty(&d_columnsMovableProperty);
	addProperty(&d_forceHorzScrollProperty);
	addProperty(&d_forceVertScrollProperty);
	addProperty(&d_nominatedSelectColProperty);
	addProperty(&d_nominatedSelectRowProperty);
	addProperty(&d_selectModeProperty);
	addProperty(&d_sortColumnIDProperty);
	addProperty(&d_sortDirectionProperty);
	addProperty(&d_sortSettingProperty);
}


/*************************************************************************
	Remove all items from the list.
*************************************************************************/
bool MultiColumnList::resetList_impl(void)
{
	// just return false if the list is already empty (no rows == empty)
	if (getRowCount() == 0)
	{
		return false;
	}
	// we have items to be removed and possible deleted
	else
	{
		for (uint i = 0; i < getRowCount(); ++i)
		{
			for (uint j = 0; j < getColumnCount(); ++j)
			{
				ListboxItem* item = d_grid[i][j];

				// delete item as needed.
				if ((item != NULL) && item->isAutoDeleted())
				{
					delete item;
				}

			}

		}

		// clear all items from the grid.
		d_grid.clear();

		// reset other affected fields
		d_nominatedSelectRow = 0;
		d_lastSelected = NULL;

		return true;
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Operators for ListRow
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Less-than operator
*************************************************************************/
bool MultiColumnList::ListRow::operator<(const ListRow& rhs) const
{
	ListboxItem* a = d_items[d_sortColumn];
	ListboxItem* b = rhs.d_items[d_sortColumn];

	// handle cases with empty slots
	if (b == NULL)
	{
		return false;
	}
	else if (a == NULL)
	{
		return true;
	}
	else
	{
		return a->getText() < b->getText();
	}

}


/*************************************************************************
	Greater-than operator
*************************************************************************/
bool MultiColumnList::ListRow::operator>(const ListRow& rhs) const
{
	ListboxItem* a = d_items[d_sortColumn];
	ListboxItem* b = rhs.d_items[d_sortColumn];

	// handle cases with empty slots
	if (a == NULL)
	{
		return false;
	}
	else if (b == NULL)
	{
		return true;
	}
	else
	{
		return a->getText() > b->getText();
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Operators for MCLGridRef
*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Assignment operator
*************************************************************************/
MCLGridRef& MCLGridRef::operator=(const MCLGridRef& rhs)
{
	column = rhs.column;
	row = rhs.row;
	return *this;
}


/*************************************************************************
	return true if this is less than (appears before) 'rhs' in grid.
*************************************************************************/
bool MCLGridRef::operator<(const MCLGridRef& rhs) const
{
	if ((row < rhs.row) ||
		((row == rhs.row) && (column < rhs.column)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*************************************************************************
	return true if this is less than or equal to 'rhs'
*************************************************************************/
bool MCLGridRef::operator<=(const MCLGridRef& rhs) const
{
	return !operator>(rhs);
}


/*************************************************************************
	return true if this is greater than (appears after) 'rhs' in grid.
*************************************************************************/
bool MCLGridRef::operator>(const MCLGridRef& rhs) const
{
	return (operator<(rhs) || operator==(rhs)) ? false : true;
}


/*************************************************************************
	return true if this is greater than or equal to 'rhs'
*************************************************************************/
bool MCLGridRef::operator>=(const MCLGridRef& rhs) const
{
	return !operator<(rhs);
}


/*************************************************************************
	return true if this grid ref is equal to that in 'rhs'
*************************************************************************/
bool MCLGridRef::operator==(const MCLGridRef& rhs) const
{
	return ((column == rhs.column) && (row == rhs.row)) ? true : false;
}


/*************************************************************************
	return true if this grid ref is different to that in 'rhs'
*************************************************************************/
bool MCLGridRef::operator!=(const MCLGridRef& rhs) const
{
	return !operator==(rhs);
}

} // End of  CEGUI namespace section
