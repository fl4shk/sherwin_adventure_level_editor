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
	primary_widget::draw_mode_icon_file_name
	("icons/draw_mode_icon_32x32.png"),
	primary_widget::sprite_properties_mode_icon_file_name
	("icons/sprite_properties_mode_icon_32x32.png"),
	primary_widget::rect_selection_mode_icon_file_name
	("icons/rect_selection_mode_icon_32x32.png");



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
	// Generate the file_menu actions.
	file_menu_laugh_action.reset(new QAction( "&Laugh", this ));
	file_menu_open_action.reset(new QAction( "&Open", this ));
	file_menu_save_action.reset(new QAction( "&Save", this ));
	file_menu_save_as_action.reset(new QAction( "&Save As", this ));
	file_menu_quit_action.reset(new QAction( "&Quit", this ));
	
	// Generate the edit_menu actions.
	edit_menu_sublevel_properties_action.reset(new QAction
		( "&Sublevel Properties", this ));
	//edit_menu_level_properties_action.reset(new QAction
	//	( "&Level Properties", this ));
	
	
	// Connect the file_menu actions to the slots.
	connect( file_menu_laugh_action.get(), &QAction::triggered, this, 
		&primary_widget::laugh );
	connect( file_menu_open_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::open_level );
	connect( file_menu_save_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::save_level );
	connect( file_menu_save_as_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::save_level_as );
	
	connect( file_menu_quit_action.get(), &QAction::triggered, this, 
		&primary_widget::quit );
	
	
	// Connect the edit_menu actions to the slots.
	connect( edit_menu_sublevel_properties_action.get(), 
		&QAction::triggered, the_central_widget.get(), 
		&level_editor_widget::create_sublevel_properties_widget );
	
	//connect( edit_menu_level_properties_action.get(), 
	//	&QAction::triggered, this, 
	//	&primary_widget::create_level_properties_widget );
	
	
	// Add the menus to the menu bar
	file_menu = menuBar()->addMenu("&File");
	edit_menu = menuBar()->addMenu("&Edit");
	
	
	// Add the file_menu actions to the file_menu
	file_menu->addAction(file_menu_laugh_action.get());
	file_menu->addAction(file_menu_open_action.get());
	file_menu->addAction(file_menu_save_action.get());
	file_menu->addAction(file_menu_save_as_action.get());
	file_menu->addSeparator();
	file_menu->addAction(file_menu_quit_action.get());
	
	
	// Add the edit_menu actions to the edit_menu
	edit_menu->addAction(edit_menu_sublevel_properties_action.get());
	//edit_menu->addAction(edit_menu_level_properties_action.get());
}

bool primary_widget::generate_toolbar()
{
	QPixmap laugh_pixmap(laugh_icon_file_name),
		draw_mode_pixmap(draw_mode_icon_file_name),
		sprite_properties_mode_pixmap
		(sprite_properties_mode_icon_file_name),
		rect_selection_mode_pixmap(rect_selection_mode_icon_file_name);
	
	if ( laugh_pixmap.isNull() )
	{
		cout << "Unable to open " << laugh_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	if ( draw_mode_pixmap.isNull() )
	{
		cout << "Unable to open " << draw_mode_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	if ( sprite_properties_mode_pixmap.isNull() )
	{
		cout << "Unable to open " 
			<< sprite_properties_mode_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	if ( rect_selection_mode_pixmap.isNull() )
	{
		cout << "Unable to open " 
			<< rect_selection_mode_icon_file_name.toStdString()
			<< " for reading!  ";
		return false;
	}
	
	
	toolbar = addToolBar("main toolbar");
	toolbar_laugh_action = toolbar->addAction( QIcon(laugh_pixmap), 
		"Laugh" );
	
	connect( toolbar_laugh_action, &QAction::triggered, this,
		&primary_widget::laugh );
	
	toolbar->addSeparator();
	
	draw_mode_tool_button_action.reset(new QAction( "Draw", this ));
	sprite_properties_mode_tool_button_action.reset(new QAction
		( "Sprite Properties", this ));
	rect_selection_mode_tool_button_action.reset(new QAction
		( "Rect Selection", this ));
	
	
	draw_mode_tool_button_action->setIcon(QIcon(draw_mode_pixmap));
	sprite_properties_mode_tool_button_action->setIcon(QIcon
		(sprite_properties_mode_pixmap));
	rect_selection_mode_tool_button_action->setIcon(QIcon
		(rect_selection_mode_pixmap));
	
	
	
	the_mouse_mode_button_group_widget.reset
		(new mouse_mode_button_group_widget( this, QPoint(), QSize(),
		draw_mode_tool_button_action.get(),
		sprite_properties_mode_tool_button_action.get(),
		rect_selection_mode_tool_button_action.get() ));
	
	
	toolbar->addWidget(the_mouse_mode_button_group_widget.get());
	
	//connect( the_mouse_mode_button_group_widget->button_group.get(),
	//	static_cast<void (QButtonGroup::*)(int)>
	//	(&QButtonGroup::buttonClicked), the_central_widget.get(),
	//	&level_editor_widget::switch_mouse_mode );
	connect( the_mouse_mode_button_group_widget->button_group.get(),
		(void (QButtonGroup::*)(int))(&QButtonGroup::buttonClicked), 
		the_central_widget.get(), &level_editor_widget::switch_mouse_mode);
	
	
	toolbar->setMovable(false);
	
	return true;
}

void primary_widget::generate_central_widget()
{
	the_central_widget.reset(new level_editor_widget( &argv_copy, this ));
	
	setCentralWidget(the_central_widget.get());
}

void primary_widget::laugh()
{
	cout << "AHAHAHAHA\n\n";
	
	
	
	sublevel& the_sublevel = the_central_widget->get_curr_sublevel();
	
	for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
	{
		for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
		{
			cout << block_stuff::get_bt_name
				((block_type)the_sublevel.uncompressed_block_data_vec_2d
				.at(j).at(i).type) << ",";
		}
		cout << endl;
	}
	
	//sfml_canvas_widget* the_sfml_canvas_widget = the_central_widget
	//	->the_core_widget->the_sfml_canvas_widget;
	//
	//QScrollArea* scroll_area = the_central_widget->scroll_area;
	//
	//sf::Vector2u sfml_canvas_size_2d = the_sfml_canvas_widget->getSize();
	//
	//sf::FloatRect sfml_canvas_visible_rect = the_sfml_canvas_widget
	//	->get_visible_rect();
	//
	//QRect scroll_area_bounding_rect = scroll_area->visibleRegion()
	//	.boundingRect();
	//
	//sf::FloatRect scroll_area_visible_rect( sf::Vector2f
	//	( (double)scroll_area_bounding_rect.x(), 
	//	(double)scroll_area_bounding_rect.y() ),
	//	sf::Vector2f( (double)scroll_area_bounding_rect.width(),
	//	(double)scroll_area_bounding_rect.height() ) );
	//
	//
	//
	//cout << sfml_canvas_visible_rect.left << ", "
	//	<< sfml_canvas_visible_rect.top << ", "
	//	<< sfml_canvas_visible_rect.width << ", "
	//	<< sfml_canvas_visible_rect.height << endl;
	//
	////cout << sfml_canvas_size_2d.x << ", " << sfml_canvas_size_2d.y << endl;
	//cout << scroll_area_visible_rect.left << ", "
	//	<< scroll_area_visible_rect.top << ", "
	//	<< scroll_area_visible_rect.width << ", "
	//	<< scroll_area_visible_rect.height << endl;
	//
	//
	//cout << scroll_area->horizontalScrollBar()->height() << ", "
	//	<< scroll_area->verticalScrollBar()->width() << endl;
	//
	//cout << endl;
	
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





