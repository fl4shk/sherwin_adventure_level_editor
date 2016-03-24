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


#include "mouse_mode_button_group_widget_class.hpp"

mouse_mode_button_group_widget::mouse_mode_button_group_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size,
	QAction* s_draw_mode_tool_button_action, 
	QAction* s_sprite_properties_mode_tool_button_action,
	QAction* s_rect_selection_mode_tool_button_action )
	: QWidget(s_parent), parent(s_parent),
	draw_mode_tool_button_action(s_draw_mode_tool_button_action),
	sprite_properties_mode_tool_button_action
	(s_sprite_properties_mode_tool_button_action),
	rect_selection_mode_tool_button_action
	(s_rect_selection_mode_tool_button_action)
{
	move(s_position);
	resize(s_size);
	
	
	hbox_layout.reset(new QHBoxLayout);
	setLayout(hbox_layout.get());
	
	group_box.reset(new QGroupBox("Tools"));
	group_box_hbox_layout.reset(new QHBoxLayout);
	group_box->setLayout(group_box_hbox_layout.get());
	
	
	button_group.reset(new QButtonGroup);
	
	generate_tool_buttons();
	
	hbox_layout->addWidget(group_box.get());
}


void mouse_mode_button_group_widget::generate_tool_buttons()
{
	draw_mode_tool_button.reset(new QToolButton);
	sprite_properties_mode_tool_button.reset(new QToolButton);
	rect_selection_mode_tool_button.reset(new QToolButton);
	
	draw_mode_tool_button->setDefaultAction(draw_mode_tool_button_action);
	sprite_properties_mode_tool_button->setDefaultAction
		(sprite_properties_mode_tool_button_action);
	rect_selection_mode_tool_button->setDefaultAction
		(rect_selection_mode_tool_button_action);
	
	//draw_mode_tool_button->setMinimumSize(QSize( 32, 32 ));
	//sprite_properties_mode_tool_button->setMinimumSize(QSize( 32, 32 ));
	//rect_selection_mode_tool_button->setMinimumSize(QSize( 32, 32 ));
	
	draw_mode_tool_button->setIconSize(QSize( 20, 20 ));
	sprite_properties_mode_tool_button->setIconSize(QSize( 20, 20 ));
	rect_selection_mode_tool_button->setIconSize(QSize( 20, 20 ));
	
	draw_mode_tool_button_action->setCheckable(true);
	sprite_properties_mode_tool_button_action->setCheckable(true);
	rect_selection_mode_tool_button_action->setCheckable(true);
	
	button_group->addButton( draw_mode_tool_button.get(), 0 );
	button_group->addButton( sprite_properties_mode_tool_button.get(), 1 );
	button_group->addButton( rect_selection_mode_tool_button.get(), 2 );
	
	draw_mode_tool_button_action->setChecked(true);
	
	
	
	group_box_hbox_layout->addWidget(draw_mode_tool_button.get());
	group_box_hbox_layout->addWidget
		(sprite_properties_mode_tool_button.get());
	group_box_hbox_layout->addWidget
		(rect_selection_mode_tool_button.get());
}




