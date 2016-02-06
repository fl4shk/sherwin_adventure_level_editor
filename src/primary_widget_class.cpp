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



#include "primary_widget_class.hpp"

const QString primary_widget::laugh_icon_file_name
	("icons/laugh_icon_32x32.png"),
	primary_widget::quit_icon_file_name("icons/quit_32x32.png");


primary_widget::primary_widget( vector<string>& s_argv_copy, 
	QWidget* parent ) : QMainWindow(parent), argv_copy(s_argv_copy)
{
	generate_central_widget();
	
	generate_menus();
	
	if ( !generate_toolbar() )
	{
		quit_non_slot();
	}
	
	
	//if ( !the_central_widget->open_image
	//	(QString("background_tiles_modded.png")) )
	//{
	//	cout << "Error opening background_tiles_modded.png!\n";
	//	quit_non_slot();
	//}
	
	//quit_non_slot();
}

void primary_widget::generate_menus()
{
	menu_laugh_action = new QAction( "&Laugh", this );
	menu_save_action = new QAction( "&Save", this );
	menu_save_as_action = new QAction( "&Save As", this );
	menu_quit_action = new QAction( "&Quit", this );
	
	connect( menu_laugh_action, &QAction::triggered, this, 
		&primary_widget::laugh );
	connect( menu_save_action, &QAction::triggered, the_central_widget,
		&level_editor_widget::save_file );
	connect( menu_save_as_action, &QAction::triggered, the_central_widget,
		&level_editor_widget::save_file_as );
	
	connect( menu_quit_action, &QAction::triggered, this, 
		&primary_widget::quit );
	
	file_menu = menuBar()->addMenu("&File");
	second_menu = menuBar()->addMenu("&Second");
	
	file_menu->addAction(menu_laugh_action);
	file_menu->addAction(menu_save_action);
	file_menu->addAction(menu_save_as_action);
	file_menu->addSeparator();
	file_menu->addAction(menu_quit_action);
	
	second_menu->addAction(menu_quit_action);
}

bool primary_widget::generate_toolbar()
{
	QPixmap laugh_pixmap(laugh_icon_file_name), 
		quit_pixmap(quit_icon_file_name);
	
	if ( laugh_pixmap.isNull() )
	{
		cout << "Unable to open " << laugh_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	if ( quit_pixmap.isNull() )
	{
		cout << "Unable to open " << quit_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	toolbar = addToolBar("main toolbar");
	toolbar_laugh_action = toolbar->addAction( QIcon(laugh_pixmap), 
		"Laugh" );
	toolbar_quit_action = toolbar->addAction( QIcon(quit_pixmap), 
		"Quit" );
	
	connect( toolbar_laugh_action, &QAction::triggered, this,
		&primary_widget::laugh );
	connect( toolbar_quit_action, &QAction::triggered, this,
		&primary_widget::quit );
	
	return true;
}

void primary_widget::generate_central_widget()
{
	the_central_widget = new level_editor_widget( &argv_copy, this );
	
	setCentralWidget(the_central_widget);
}

void primary_widget::laugh()
{
	cout << "AHAHAHAHA\n\n";
	
	
	
	//sublevel& the_sublevel = the_central_widget->the_core_widget
	//	->the_sublevel;
	//for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
	//{
	//	for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
	//	{
	//		cout << block_stuff::get_bt_name
	//			((block_type)the_sublevel.uncompressed_block_data_vec_2d
	//			.at(j).at(i).type) << ",";
	//	}
	//	cout << endl;
	//}
	
	sfml_canvas_widget* the_sfml_canvas_widget = the_central_widget
		->the_core_widget->the_sfml_canvas_widget;
	
	QScrollArea* scroll_area = the_central_widget->scroll_area;
	
	sf::Vector2u sfml_canvas_size_2d = the_sfml_canvas_widget->getSize();
	
	sf::FloatRect sfml_canvas_visible_rect = the_sfml_canvas_widget
		->get_visible_rect();
	
	QRect scroll_area_bounding_rect = scroll_area->visibleRegion()
		.boundingRect();
	
	sf::FloatRect scroll_area_visible_rect( sf::Vector2f
		( (double)scroll_area_bounding_rect.x(), 
		(double)scroll_area_bounding_rect.y() ),
		sf::Vector2f( (double)scroll_area_bounding_rect.width(),
		(double)scroll_area_bounding_rect.height() ) );
	
	
	
	cout << sfml_canvas_visible_rect.left << ", "
		<< sfml_canvas_visible_rect.top << ", "
		<< sfml_canvas_visible_rect.width << ", "
		<< sfml_canvas_visible_rect.height << endl;
	
	//cout << sfml_canvas_size_2d.x << ", " << sfml_canvas_size_2d.y << endl;
	cout << scroll_area_visible_rect.left << ", "
		<< scroll_area_visible_rect.top << ", "
		<< scroll_area_visible_rect.width << ", "
		<< scroll_area_visible_rect.height << endl;
	
	
	cout << scroll_area->horizontalScrollBar()->height() << ", "
		<< scroll_area->verticalScrollBar()->width() << endl;
	
	cout << endl;
	
	//sfml_canvas_widget* the_sfml_canvas_widget = the_central_widget
	//	->the_core_widget->the_sfml_canvas_widget;
	//
	//sf::FloatRect the_sfml_canvas_visible_rect = the_sfml_canvas_widget
	//	->get_visible_rect();
	//
	//cout << the_sfml_canvas_visible_rect.left << ", "
	//	<< the_sfml_canvas_visible_rect.top << ", "
	//	<< the_sfml_canvas_visible_rect.width << ", "
	//	<< the_sfml_canvas_visible_rect.height << endl;
	//
	//cout << endl;
	
}

void primary_widget::quit()
{
	quit_non_slot();
}

