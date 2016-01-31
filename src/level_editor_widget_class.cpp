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
	parent(s_parent)
{
	//setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
	
	
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ), 
	//	QSize( 200, 200 ), string("the_powerup_gfx.png") );
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ),
	//	QSize( 200, 200 ), argv_copy->at(1) );
	the_core_widget = new level_editor_core_widget( this, QPoint( 0, 0 ),
		QSize( 200, 200 ), argv_copy->at(1) );
	the_block_selector_widget = new block_selector_widget( this, 
		QPoint( 0, 0 ), QSize( 300, 300 ) );
	
	//the_sfml_canvas_widget->set_the_block_selector_core_widget
	//	(the_block_selector_widget->the_core_widget);
	//the_block_selector_widget->the_core_widget
	//	->set_the_sfml_canvas_widget(the_sfml_canvas_widget);
	
	the_core_widget->set_the_block_selector_core_widget
		(the_block_selector_widget->the_core_widget);
	the_block_selector_widget->the_core_widget
		->set_the_level_editor_core_widget(the_core_widget);
	
	
	
	//if ( !the_sfml_canvas_widget->open_image() )
	//{
	//	quit_non_slot();
	//}
	
	
	// scroll_area stuff
	scroll_area = new QScrollArea(this);
	scroll_area->setWidget(the_core_widget);
	scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	
	// Test button
	//QPushButton* show_horiz_sb_stuff_button 
	//	= new QPushButton( "Show Horiz", this );
	QPushButton* show_horiz_sb_stuff_button 
		= new QPushButton( "Show Horiz", NULL );
	connect( show_horiz_sb_stuff_button, &QPushButton::clicked, this,
		&level_editor_widget::show_horizontal_scroll_bar_stuff );
	show_horiz_sb_stuff_button->setMaximumSize( 128, 32 );
	
	
	// tab widget stuff
	tab_widget = new QTabWidget(this);
	tab_widget->setMovable(true);
	tab_widget->addTab( the_block_selector_widget, "Blocks" );
	tab_widget->addTab( show_horiz_sb_stuff_button, 
		"Horiz ScrollBar Stuff" );
	
	
	// hbox_layout stuff
	hbox_layout = new QHBoxLayout(this);
	hbox_layout->addWidget(scroll_area);
	
	
	//hbox_layout->addWidget(the_block_selector_widget);
	hbox_layout->addWidget(tab_widget);
	
	
	
}


//void level_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue
//		( ( the_sfml_canvas_widget->scale_factor * scroll_bar->value() ) 
//		+ ( ( the_sfml_canvas_widget->scale_factor - 1 ) 
//		* ( scroll_bar->pageStep() / 2 ) ));
//}

bool level_editor_widget::zoom_in()
{
	if ( the_core_widget->the_sfml_canvas_widget->scale_factor == 16 )
	{
		return false;
	}
	
	the_core_widget->the_sfml_canvas_widget->scale_factor <<= 1;
	the_core_widget->the_sfml_canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() << 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() << 1 );
	
	return true;
}


bool level_editor_widget::zoom_out()
{
	if ( the_core_widget->the_sfml_canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	the_core_widget->the_sfml_canvas_widget->scale_factor >>= 1;
	the_core_widget->the_sfml_canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() >> 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() >> 1 );
	
	return true;
}

void level_editor_widget::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_A )
	{
		zoom_in();
	}
	else if ( event->key() == Qt::Key_Z )
	{
		zoom_out();
	}
	else if ( event->key() == Qt::Key_T )
	{
		// Temporary until a toggle button is created for this purpose.
		if ( !the_core_widget->the_sfml_canvas_widget
			->get_block_grid_enabled() )
		{
			the_core_widget->the_sfml_canvas_widget->enable_block_grid();
			//cout << "Tile grid now enabled.\n";
		}
		else
		{
			the_core_widget->the_sfml_canvas_widget->disable_block_grid();
			//cout << "Tile grid now disabled.\n";
		}
	}
}


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




void level_editor_widget::hello()
{
	cout << "Hello!\n";
}


void level_editor_widget::show_horizontal_scroll_bar_stuff()
{
	cout << scroll_area->horizontalScrollBar()->value() << " "
		<< scroll_area->horizontalScrollBar()->minimum() << " "
		<< scroll_area->horizontalScrollBar()->maximum() << endl;
}

void level_editor_widget::show_vertical_scroll_bar_stuff()
{
	cout << scroll_area->verticalScrollBar()->value() << " "
		<< scroll_area->verticalScrollBar()->minimum() << " "
		<< scroll_area->verticalScrollBar()->maximum() << endl;
}

void level_editor_widget::show_geometry_stuff()
{
	cout << scroll_area->geometry().x() << " " 
		<< scroll_area->geometry().y() << endl;
	cout << scroll_area->frameGeometry().x() << " " 
		<< scroll_area->frameGeometry().y() << endl;
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

