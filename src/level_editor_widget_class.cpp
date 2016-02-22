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



#include "level_editor_widget_class.hpp"

const QString level_editor_widget::default_parent_title 
	= QString("Sherwin's Adventure Level Editor");

level_editor_widget::level_editor_widget( vector<string>* s_argv_copy,
	QWidget* s_parent ) : QWidget(s_parent), argv_copy(s_argv_copy),
	parent(s_parent), the_sprite_properties_widget(NULL)
{
	//setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
	
	
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ), 
	//	QSize( 200, 200 ), string("the_powerup_gfx.png") );
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ),
	//	QSize( 200, 200 ), argv_copy->at(1) );
	
	//the_core_widget = new level_editor_core_widget( this, QPoint( 0, 0 ),
	//	QSize( 256, 256 ), argv_copy->at(1) );
	the_core_widget = new level_editor_core_widget( this, QPoint( 0, 0 ),
		QSize( 8192, 512 ), argv_copy->at(1) );
	//the_core_widget = new level_editor_core_widget( this, QPoint( 0, 0 ),
	//	QSize( 2048, 512 ), argv_copy->at(1) );
	//the_core_widget = new level_editor_core_widget( this, QPoint( 0, 0 ),
	//	QSize( 512, 512 ), argv_copy->at(1) );
	
	#define X(name) \
	the_##name##_selector_widget = new name##_selector_widget( this, \
		QPoint( 0, 0 ), QSize( 64, 64 ) );
	
	list_of_level_element_widget_name_prefixes(X);
	#undef X
	
	// tab widget stuff
	level_element_selectors_tab_widget = new QTabWidget(this);
	level_element_selectors_tab_widget->setMovable(true);
	level_element_selectors_tab_widget->addTab
		( the_block_selector_widget, "Blocks" );
	level_element_selectors_tab_widget->addTab
		( the_sprite_16x16_selector_widget, "16x16 Sprites" );
	level_element_selectors_tab_widget->addTab
		( the_sprite_16x32_selector_widget, "16x32 Sprites" );
	
	
	the_core_widget->initialize_tab_stuff
		( level_element_selectors_tab_widget, the_block_selector_widget, 
		the_sprite_16x16_selector_widget,
		the_sprite_16x32_selector_widget );
	
	//connect( the_core_widget,
	//	&level_editor_core_widget::right_mouse_button_pressed, this,
	//	&level_editor_widget::hello );
	
	connect( the_core_widget,
		&level_editor_core_widget::sprite_was_selected, this,
		&level_editor_widget::show_sprite_properties_widget );
	
	connect( the_core_widget,
		&level_editor_core_widget::sprite_no_longer_selected, this,
		&level_editor_widget::hide_sprite_properties_widget );
	
	
	//if ( !the_sfml_canvas_widget->open_image() )
	//{
	//	quit_non_slot();
	//}
	
	
	// core_widget_scroll_area stuff
	core_widget_scroll_area = new QScrollArea(this);
	core_widget_scroll_area->setWidget(the_core_widget);
	core_widget_scroll_area->setHorizontalScrollBarPolicy
		(Qt::ScrollBarAlwaysOn);
	core_widget_scroll_area->setVerticalScrollBarPolicy
		(Qt::ScrollBarAlwaysOn);
	
	//core_widget_scroll_area->setSizePolicy( QSizePolicy::Minimum, 
	//	QSizePolicy::Minimum );
	//core_widget_scroll_area->setMinimumSize( QSize( 512, 512 ) );
	
	//the_core_widget->setSizePolicy( QSizePolicy::Minimum, 
	//	QSizePolicy::Minimum );
	//the_core_widget->setMinimumSize( QSize( 256, 256 ) );
	
	the_core_widget->the_sfml_canvas_widget->set_scroll_area
		(core_widget_scroll_area);
	
	
	
	
	
	//vbox_layout = new QVBoxLayout(this);
	vbox_layout = new QVBoxLayout;
	vbox_layout->addWidget(level_element_selectors_tab_widget);
	
	
	// hbox_layout stuff
	hbox_layout = new QHBoxLayout(this);
	hbox_layout->addWidget(core_widget_scroll_area);
	
	//hbox_layout->addSpacing(3000);
	
	//hbox_layout->addWidget(level_element_selectors_tab_widget);
	hbox_layout->addLayout(vbox_layout);
	
}


//void level_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue
//		( ( the_sfml_canvas_widget->scale_factor * scroll_bar->value() ) 
//		+ ( ( the_sfml_canvas_widget->scale_factor - 1 ) 
//		* ( scroll_bar->pageStep() / 2 ) ));
//}




bool level_editor_widget::open_level()
{
	fstream level_file( the_core_widget->level_file_name, 
		ios_base::in | ios_base::binary );
	
	if ( !level_file.is_open() )
	{
		cout << "Unable to open " << the_core_widget->level_file_name 
			<< " for reading.\n";
		return false;
	}
	
	return true;
}

void level_editor_widget::export_file_as( const string& output_file_name )
{
	
}

void level_editor_widget::show_sprite_properties_widget()
{
	if (!sprite_properties_widget_enabled)
	{
		sprite_properties_widget_enabled = true;
		
		//if ( show_horiz_sb_stuff_button == NULL )
		//{
		//	// Test button
		//	show_horiz_sb_stuff_button = new QPushButton( "Show Horiz", 
		//		NULL );
		//	connect( show_horiz_sb_stuff_button, &QPushButton::clicked, 
		//		this,
		//		&level_editor_widget::show_horizontal_scroll_bar_stuff );
		//	
		//	//show_horiz_sb_stuff_button->setMaximumSize( 128, 32 );
		//	//show_horiz_sb_stuff_button->setSizePolicy( QSizePolicy::Fixed,
		//	//	QSizePolicy::Fixed );
		//	
		//	vbox_layout->addWidget(show_horiz_sb_stuff_button);
		//}
		
		if ( the_sprite_properties_widget == NULL )
		{
			the_sprite_properties_widget 
				= new sprite_properties_widget(this);
			
			vbox_layout->addWidget(the_sprite_properties_widget);
			//cout << "added the_sprite_properties_widget\n";
		}
	}
}

void level_editor_widget::hide_sprite_properties_widget()
{
	if (sprite_properties_widget_enabled)
	{
		sprite_properties_widget_enabled = false;
		
		//if ( show_horiz_sb_stuff_button != NULL )
		//{
		//	//vbox_layout->removeWidget(show_horiz_sb_stuff_button);
		//	
		//	delete show_horiz_sb_stuff_button;
		//	
		//	show_horiz_sb_stuff_button = NULL;
		//}
		
		if ( the_sprite_properties_widget != NULL )
		{
			vbox_layout->removeWidget(the_sprite_properties_widget);
			
			delete the_sprite_properties_widget;
			
			the_sprite_properties_widget = NULL;
		}
	}
}



void level_editor_widget::hello()
{
	cout << "Hello!\n";
}


void level_editor_widget::show_horizontal_scroll_bar_stuff()
{
	cout << core_widget_scroll_area->horizontalScrollBar()->value() << " "
		<< core_widget_scroll_area->horizontalScrollBar()->minimum() << " "
		<< core_widget_scroll_area->horizontalScrollBar()->maximum() 
		<< endl;
}

void level_editor_widget::show_vertical_scroll_bar_stuff()
{
	cout << core_widget_scroll_area->verticalScrollBar()->value() << " "
		<< core_widget_scroll_area->verticalScrollBar()->minimum() << " "
		<< core_widget_scroll_area->verticalScrollBar()->maximum() 
		<< endl;
}

void level_editor_widget::show_geometry_stuff()
{
	cout << core_widget_scroll_area->geometry().x() << " " 
		<< core_widget_scroll_area->geometry().y() << endl;
	cout << core_widget_scroll_area->frameGeometry().x() << " " 
		<< core_widget_scroll_area->frameGeometry().y() << endl;
}



void level_editor_widget::save_file()
{
	cout << "Saving....\n";
	
	export_file();
}

void level_editor_widget::save_file_as()
{
	QString output_file_name = QFileDialog::getSaveFileName( this, 
		tr("Save File"), QString(), tr("PNG File (*.png)") );
	//cout << "This QString was obtained:  " 
	//	<< output_file_name.toStdString() << endl;
	
	cout << "Saving....\n";
	
	export_file_as(output_file_name.toStdString());
}

