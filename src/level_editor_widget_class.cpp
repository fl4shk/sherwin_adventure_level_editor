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
	
	//core_widget_scroll_area->setSizePolicy( QSizePolicy::MinimumExpanding,
	//	QSizePolicy::MinimumExpanding );
	//core_widget_scroll_area->setSizePolicy( QSizePolicy::Ignored,
	//	QSizePolicy::Ignored );
	//core_widget_scroll_area->setWidgetResizable(true);
	
	the_core_widget->the_sfml_canvas_widget->set_scroll_area
		(core_widget_scroll_area);
	
	
	
	////vbox_layout = new QVBoxLayout(this);
	//vbox_layout = new QVBoxLayout;
	//vbox_layout->addWidget(level_element_selectors_tab_widget);
	//
	//
	//// hbox_layout stuff
	//hbox_layout = new QHBoxLayout(this);
	//hbox_layout->addWidget(core_widget_scroll_area);
	//
	////hbox_layout->addSpacing(3000);
	//
	////hbox_layout->addWidget(level_element_selectors_tab_widget);
	//hbox_layout->addLayout(vbox_layout);
	
	// splitter stuff
	horiz_splitter = new QSplitter(this);
	
	horiz_splitter->setSizePolicy( QSizePolicy::Ignored, 
		QSizePolicy::Ignored );
	
	
	vert_splitter = new QSplitter(this);
	vert_splitter->addWidget(level_element_selectors_tab_widget);
	vert_splitter->setOrientation(Qt::Vertical);
	
	
	horiz_splitter->addWidget(core_widget_scroll_area);
	horiz_splitter->addWidget(vert_splitter);
	
	
	hbox_layout = new QHBoxLayout(this);
	hbox_layout->addWidget(horiz_splitter);
	
	
}


//void level_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue
//		( ( the_sfml_canvas_widget->scale_factor * scroll_bar->value() ) 
//		+ ( ( the_sfml_canvas_widget->scale_factor - 1 ) 
//		* ( scroll_bar->pageStep() / 2 ) ));
//}

void level_editor_widget::keyPressEvent( QKeyEvent* event )
{
	// These probably ought to be in a toolbar of some sort, as well.
	
	sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget;
	rect_selection_stuff& the_rect_selection_stuff
		= the_sfml_canvas_widget->the_rect_selection_stuff;
	
	if ( event->key() == Qt::Key_A )
	{
		the_core_widget->zoom_in();
	}
	else if ( event->key() == Qt::Key_Z )
	{
		the_core_widget->zoom_out();
	}
	else if ( event->key() == Qt::Key_T )
	{
		// Temporary until a toggle button is created for this purpose.
		if ( !the_sfml_canvas_widget->get_block_grid_enabled() )
		{
			the_sfml_canvas_widget->enable_block_grid();
			//cout << "Tile grid now enabled.\n";
		}
		else
		{
			the_sfml_canvas_widget->disable_block_grid();
			//cout << "Tile grid now disabled.\n";
		}
	}
	else if ( event->key() == Qt::Key_Q )
	{
		if ( the_rect_selection_stuff.get_enabled() )
		{
			the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		
		the_core_widget->the_mouse_mode = mm_place_level_elements;
		
		cout << "Current mouse mode:  place_level_elements\n";
	}
	else if ( event->key() == Qt::Key_W )
	{
		if ( the_rect_selection_stuff.get_enabled() )
		{
			the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		
		the_core_widget->the_mouse_mode = mm_select_single_sprite;
		
		cout << "Current mouse mode:  select_sprites\n";
	}
	else if ( event->key() == Qt::Key_E )
	{
		the_core_widget->the_mouse_mode = mm_rect_selection;
		
		cout << "Current mouse mode:  rect_selection\n";
	}
	else if ( event->key() == Qt::Key_C 
		&& the_core_widget->the_mouse_mode == mm_rect_selection 
		&& the_rect_selection_stuff.get_enabled() )
	{
		//cout << "Copying selection contents\n";
		
		the_rect_selection_stuff.copy_selection_contents();
	}
	else if ( event->key() == Qt::Key_V 
		&& the_core_widget->the_mouse_mode == mm_rect_selection )
	{
		if ( the_rect_selection_stuff.get_enabled() )
		{
			the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		
		//cout << "Pasting the copied selection contents\n";
		
		//the_sfml_canvas_widget->the_rect_selection_stuff
		//	.paste_copied_selection_contents( vec2_s32( 0, 0 ) );
		
		sf::FloatRect visible_rect 
			= the_sfml_canvas_widget->get_visible_rect();
		
		u32 num_pixels_per_block_row 
			= the_sfml_canvas_widget->num_pixels_per_block_row;
		u32 num_pixels_per_block_column 
			= the_sfml_canvas_widget->num_pixels_per_block_column;
		u32 scale_factor = the_sfml_canvas_widget->scale_factor;
		
		vec2<double> visible_block_grid_start_pos
			( (double)visible_rect.left 
			/ (double)( num_pixels_per_block_column * scale_factor ), 
			(double)visible_rect.top / (double)( num_pixels_per_block_row
			* scale_factor ) );
		vec2<double> visible_block_grid_size_2d( (double)visible_rect.width 
			/ (double)( num_pixels_per_block_column * scale_factor ), 
			(double)visible_rect.height 
			/ (double)( num_pixels_per_block_row * scale_factor ) );
		
		// this is so that sprites larger than 16x16 pixels will be drawn if
		// their starting position is offscreen but part of them still is on
		// screen.
		--visible_block_grid_start_pos.x;
		--visible_block_grid_start_pos.y;
		
		//cout << visible_block_grid_start_pos.x << ", "
		//	<< visible_block_grid_start_pos.y << ", "
		//	<< visible_block_grid_size_2d.x << ", "
		//	<< visible_block_grid_size_2d.y << endl;
		
		++visible_block_grid_size_2d.x;
		++visible_block_grid_size_2d.y;
		
		++visible_block_grid_size_2d.x;
		++visible_block_grid_size_2d.y;
		
		if ( visible_block_grid_start_pos.x < 0 )
		{
			visible_block_grid_start_pos.x = 0;
		}
		if ( visible_block_grid_start_pos.y < 0 )
		{
			visible_block_grid_start_pos.y = 0;
		}
		
		if ( ( visible_block_grid_start_pos.x 
			+ visible_block_grid_size_2d.x )
			>= (s32)the_core_widget->the_sublevel.size_2d.x )
		{
			visible_block_grid_size_2d.x 
				= the_core_widget->the_sublevel.size_2d.x
				- visible_block_grid_start_pos.x;
		}
		if ( ( visible_block_grid_start_pos.y 
			+ visible_block_grid_size_2d.y )
			>= (s32)the_core_widget->the_sublevel.size_2d.y )
		{
			visible_block_grid_size_2d.y 
				= the_core_widget->the_sublevel.size_2d.y
				- visible_block_grid_start_pos.y;
		}
		
		
		
		// Paste at the location of the mouse.
		sf::FloatRect visible_block_grid_rect
			( visible_block_grid_start_pos.x,
			visible_block_grid_start_pos.y,
			visible_block_grid_size_2d.x, visible_block_grid_size_2d.y );
		
		
		
		sf::Vector2i mouse_pos_in_canvas_widget_coords 
			= sf::Mouse::getPosition(*the_sfml_canvas_widget);
		
		// This converts the clicked coordinate to pixel coordinates.
		sf::Vector2f mouse_pos_in_canvas_coords
			( (double)mouse_pos_in_canvas_widget_coords.x 
			/ (double)the_sfml_canvas_widget->scale_factor,
			(double)mouse_pos_in_canvas_widget_coords.y
			/ (double)the_sfml_canvas_widget->scale_factor );
		
		vec2_s32 block_grid_coords_of_mouse_pos
			= { (s32)( mouse_pos_in_canvas_coords.x
			/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
			
			(s32)( ( the_sfml_canvas_widget->the_sublevel->real_size_2d.y 
			- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
			- mouse_pos_in_canvas_coords.y )
			/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
		
		
		if ( visible_block_grid_rect.contains
			( block_grid_coords_of_mouse_pos.x, 
			block_grid_coords_of_mouse_pos.y ) )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.paste_copied_selection_contents
				(block_grid_coords_of_mouse_pos);
		}
		//else
		//{
		//	the_sfml_canvas_widget->the_rect_selection_stuff
		//		.paste_copied_selection_contents( vec2_s32
		//		( visible_block_grid_start_pos.x, 
		//		visible_block_grid_start_pos.y ) );
		//}
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

void level_editor_widget::show_sprite_properties_widget()
{
	sprite_properties_widget_enabled = true;
	
	//the_sprite_properties_widget.reset
	//	(new sprite_properties_widget( this, 
	//	the_core_widget->the_sfml_canvas_widget
	//	->get_rect_selection_single_selected_sprite_ipgws() ));
	the_sprite_properties_widget.reset
		(new sprite_properties_widget( this, 
		the_core_widget->the_sfml_canvas_widget->the_rect_selection_stuff
		.get_single_selected_sprite_ipgws() ));
	
	//vbox_layout->addWidget(the_sprite_properties_widget.get());
	//cout << "added the_sprite_properties_widget\n";
	
	vert_splitter->addWidget(the_sprite_properties_widget.get());
}

void level_editor_widget::hide_sprite_properties_widget()
{
	if (sprite_properties_widget_enabled)
	{
		sprite_properties_widget_enabled = false;
		
		if ( the_sprite_properties_widget.get() != NULL )
		{
			//cout << "the_sprite_properties_widget.get() != NULL\n";
			//vbox_layout->removeWidget(the_sprite_properties_widget.get());
			
			the_sprite_properties_widget->hide();
			
			the_sprite_properties_widget.reset(NULL);
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
		tr("Save File"), QString(), tr("Level File (*.xml)") );
	//cout << "This QString was obtained:  " 
	//	<< output_file_name.toStdString() << endl;
	
	cout << "Saving....\n";
	
	export_file_as(output_file_name.toStdString());
}

