// This file is part of Sherwin's Adventure Level Editor.
// 
// Copyright 2015-2016 by Andrew Clark (FL4SHK).
// 
// Sherwin's Adventure Level Editor is free software: you can redistribute
// it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
// 
// Sherwin's Adventure Level Editor is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with Sherwin's Adventure Level Editor.  If not, see
// <http://www.gnu.org/licenses/>.



#ifndef layout_widget_classes_hpp
#define layout_widget_classes_hpp

#include "misc_includes.hpp"

// These classes are intended to be used as child widgets.

class grid_widget : public QWidget
{
public:		// functions
	inline grid_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QGridLayout(this);
	}
	
public:		// variables
	QGridLayout* layout;
	
};

class vbox_widget : public QWidget
{
public:		// functions
	inline vbox_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QVBoxLayout(this);
	}
	
public:		// variables
	QVBoxLayout* layout;
	
};

class hbox_widget : public QWidget
{
public:		// functions
	inline hbox_widget( QWidget* parent = 0 ) : QWidget(parent)
	{
		layout = new QHBoxLayout(this);
	}
	
public:		// variables
	QHBoxLayout* layout;
	
};

#endif		// layout_widget_classes_hpp
