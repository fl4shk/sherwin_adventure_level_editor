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



#include "sfml_canvas_widget_classes.hpp"

#ifdef Q_WS_X11
	#include <Qt/QtX11Extras/qx11info_x11.h>
	#include <X11/Xlib.h>
#endif

#include "block_selector_core_widget_class.hpp"
#include "sprite_16x16_selector_core_widget_class.hpp"
#include "sprite_16x32_selector_core_widget_class.hpp"
#include "level_editor_core_widget_class.hpp"



sfml_canvas_widget_base::sfml_canvas_widget_base( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, u32 frame_time )
	: QWidget(s_parent), parent(s_parent), initialized(false)
{
	// Set up some states to allow direct rendering into the widget.
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	
	//// Set strong focus to enable keyboard events (and maybe mouse events?)
	//// to be received.
	//setFocusPolicy(Qt::StrongFocus);
	
	
	
	// Set up the widget geometry.
	move(s_position);
	resize(s_size);
	
	// Set up the timer.
	timer.setInterval(frame_time);
	
}

sfml_canvas_widget_base::~sfml_canvas_widget_base()
{
}

void sfml_canvas_widget_base::on_init()
{
}
void sfml_canvas_widget_base::on_update()
{
}


QPaintEngine* sfml_canvas_widget_base::paintEngine() const
{
	return 0;
}

void sfml_canvas_widget_base::showEvent( QShowEvent* event )
{
	if (!initialized)
	{
		// Under X11, the commands sent to the server needs to be flushed
		// to ensure that SFML will get an updated view of the windows.
		#ifdef Q_WS_X11
			XFlush(QX11Info::display());
		#endif
		
		// Create the SFML window with the widget handle.
		sf::RenderWindow::create(winId());
		
		// Let the derived class do its specific stuff.
		on_init();
		
		// Set up the timer to trigger a refresh at specified framerate.
		connect( &timer, SIGNAL(timeout()), this, SLOT(repaint()) );
		timer.start();
		
		initialized = true;
	}
}

void sfml_canvas_widget_base::paintEvent( QPaintEvent* event )
{
	// Let the derived class do its specific stuff.
	on_update();
	
	// Display on screen.
	display();
}


const string sfml_canvas_widget::door_number_gfx_file_name
	= "gfx/the_number_gfx.png";


sfml_canvas_widget::sfml_canvas_widget( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	
	unzoomed_size_2d(s_size),
	
	scroll_area(NULL),
	
	modified_recently(false), 
	
	zoomed_in_recently(false), zoomed_out_recently(false),
	
	scale_factor(1), 
	
	view_center_x(0.0f), view_center_y(0.0f)
{
	apparent_view = getDefaultView();
	
	the_block_grid_stuff.enabled = false;
	the_rect_selection_stuff.enabled = false;
	
	// (Maybe) Make it so that the widget's full contents can be scrolled
	//setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	
	door_number_gfx_raw_image.loadFromFile(door_number_gfx_file_name);
	door_number_gfx_raw_image.createMaskFromColor(sf::Color::Cyan);
	door_number_gfx_raw_texture.loadFromImage(door_number_gfx_raw_image);
	door_number_sprite.setTexture(door_number_gfx_raw_texture);
}

sfml_canvas_widget::sfml_canvas_widget( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, 
	QScrollArea* s_scroll_area ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	
	unzoomed_size_2d(s_size),
	
	scroll_area(s_scroll_area),
	
	modified_recently(false), 
	zoomed_in_recently(false), zoomed_out_recently(false), 
	
	scale_factor(1), 
	
	view_center_x(0.0f), view_center_y(0.0f)
{
	apparent_view = getDefaultView();
	
	the_block_grid_stuff.enabled = false;
	the_rect_selection_stuff.enabled = false;
	
	// (Maybe) Make it so that the widget's full contents can be scrolled
	//setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}



const sf::View& sfml_canvas_widget::get_apparent_view()
{
	apparent_view = getDefaultView();
	
	apparent_view.setCenter( view_center_x, view_center_y );
	apparent_view.setSize( unzoomed_size_2d.width() * scale_factor,
		unzoomed_size_2d.height() * scale_factor );
	
	apparent_view.move( unzoomed_size_2d.width() / (double)2.0f, 
		unzoomed_size_2d.height() / (double)2.0f );
	
	apparent_view.zoom( 1.0f / (double)scale_factor );
	
	return apparent_view;
}


void sfml_canvas_widget::start_rect_selection
	( const vec2_s32& n_starting_block_grid_coords_of_mouse, 
	rect_selection_layer n_selection_layer )
{
	//cout << "start_rect_selection()\n";
	
	the_rect_selection_stuff.starting_block_grid_coords_of_mouse
		= n_starting_block_grid_coords_of_mouse;
	the_rect_selection_stuff.starting_block_grid_coords
		= n_starting_block_grid_coords_of_mouse;
	
	the_rect_selection_stuff.selection_rect.left 
		= n_starting_block_grid_coords_of_mouse.x;
	the_rect_selection_stuff.selection_rect.top
		= n_starting_block_grid_coords_of_mouse.y;
	
	the_rect_selection_stuff.single_sprite_selected = false;
	
	the_rect_selection_stuff.enabled = true;
	the_rect_selection_stuff.mouse_released = false;
	the_rect_selection_stuff.selection_layer = n_selection_layer;
	the_rect_selection_stuff.moving = false;
	
	if ( the_rect_selection_stuff.selection_layer == rsl_blocks )
	{
		// Just one block
		the_rect_selection_stuff.selection_rect = sf::IntRect
			( n_starting_block_grid_coords_of_mouse.x,
			n_starting_block_grid_coords_of_mouse.y, 1, 1 );
		
		the_rect_selection_stuff.ending_block_grid_coords
			= n_starting_block_grid_coords_of_mouse;
	}
	else if ( the_rect_selection_stuff.selection_layer == rsl_sprites )
	{
		//sprite_init_param_group_with_size* single_selected_sprite_ipgws
		//	= &( the_sublevel->sprite_ipgws_vec_2d
		//	.at(n_starting_block_grid_coords_of_mouse.y)
		//	.at(n_starting_block_grid_coords_of_mouse.x) );
		//
		//if ( single_selected_sprite_ipgws->size_2d.x == 0 
		//	|| single_selected_sprite_ipgws->size_2d.y == 0 )
		//if ( single_selected_sprite_ipgws->type == st_default )
		{
			//disable_rect_selection();
			
			// Just one "block"
			the_rect_selection_stuff.selection_rect = sf::IntRect
				( n_starting_block_grid_coords_of_mouse.x,
				n_starting_block_grid_coords_of_mouse.y, 1, 1 );
			
			the_rect_selection_stuff.ending_block_grid_coords
				= n_starting_block_grid_coords_of_mouse;
		}
		//else
		//{
		//	
		//	// Make it so that the rectangular selection covers the whole
		//	// sprite.  For "blank" sprites (using st_default), this would
		//	// hopefully cause the size of the selection_rect to be 16x16.
		//	the_rect_selection_stuff.selection_rect = sf::IntRect
		//		( single_selected_sprite_ipgws->initial_block_grid_x_coord,
		//		single_selected_sprite_ipgws->initial_block_grid_y_coord,
		//		single_selected_sprite_ipgws->size_2d.x 
		//		/ num_pixels_per_block_row, 
		//		single_selected_sprite_ipgws->size_2d.y 
		//		/ num_pixels_per_block_column );
		//	
		//	the_rect_selection_stuff.ending_block_grid_coords.x
		//		= the_rect_selection_stuff.starting_block_grid_coords.x 
		//		+ single_selected_sprite_ipgws->size_2d.x;
		//	
		//	the_rect_selection_stuff.ending_block_grid_coords.y
		//		= the_rect_selection_stuff.starting_block_grid_coords.y 
		//		+ single_selected_sprite_ipgws->size_2d.y;
		//}
	}
}

void sfml_canvas_widget::continue_rect_selection
	( const vec2_s32& curr_block_grid_coords_of_mouse )
{
	//cout << "continue_rect_selection()\n";
	
	vec2_s32& starting_block_grid_coords_of_mouse 
		= the_rect_selection_stuff.starting_block_grid_coords_of_mouse;
	vec2_s32& starting_block_grid_coords 
		= the_rect_selection_stuff.starting_block_grid_coords;
	vec2_s32& ending_block_grid_coords 
		= the_rect_selection_stuff.ending_block_grid_coords;
	
	
	if ( starting_block_grid_coords_of_mouse.x
		< curr_block_grid_coords_of_mouse.x )
	{
		starting_block_grid_coords.x
			= starting_block_grid_coords_of_mouse.x;
		ending_block_grid_coords.x
			= curr_block_grid_coords_of_mouse.x;
	}
	else
	{
		starting_block_grid_coords.x
			= curr_block_grid_coords_of_mouse.x;
		ending_block_grid_coords.x
			= starting_block_grid_coords_of_mouse.x;
	}
	
	if ( starting_block_grid_coords_of_mouse.y
		< curr_block_grid_coords_of_mouse.y )
	{
		starting_block_grid_coords.y
			= starting_block_grid_coords_of_mouse.y;
		ending_block_grid_coords.y
			= curr_block_grid_coords_of_mouse.y;
	}
	else
	{
		starting_block_grid_coords.y
			= curr_block_grid_coords_of_mouse.y;
		ending_block_grid_coords.y
			= starting_block_grid_coords_of_mouse.y;
	}
	
	
	// Correction things.
	if ( starting_block_grid_coords.x < 0 )
	{
		starting_block_grid_coords.x = 0;
	}
	if ( starting_block_grid_coords.y < 0 )
	{
		starting_block_grid_coords.y = 0;
	}
	if ( starting_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		starting_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( starting_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		starting_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	
	if ( ending_block_grid_coords.x < 0 )
	{
		ending_block_grid_coords.x = 0;
	}
	if ( ending_block_grid_coords.y < 0 )
	{
		ending_block_grid_coords.y = 0;
	}
	if ( ending_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		ending_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( ending_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		ending_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	the_rect_selection_stuff.selection_rect.left 
		= starting_block_grid_coords.x;
	the_rect_selection_stuff.selection_rect.top 
		= starting_block_grid_coords.y;
	
	the_rect_selection_stuff.selection_rect.width 
		= ending_block_grid_coords.x - starting_block_grid_coords.x + 1;
	the_rect_selection_stuff.selection_rect.height 
		= ending_block_grid_coords.y - starting_block_grid_coords.y + 1;
}

void sfml_canvas_widget::finish_rect_selection()
{
	//cout << "finish_rect_selection()\n";
	the_rect_selection_stuff.mouse_released = true;
}



void sfml_canvas_widget::start_moving_rect_selection_contents
	( const vec2_s32 n_clicked_location_in_rect )
{
	//cout << "start_moving_rect_selection_contents()\n";
	
	//the_rect_selection_stuff.starting_block_grid_coords_of_mouse
	//	= n_starting_block_grid_coords_of_mouse;
	//the_rect_selection_stuff.starting_block_grid_coords
	//	= n_starting_block_grid_coords_of_mouse;
	//
	//the_rect_selection_stuff.selection_rect.left 
	//	= n_starting_block_grid_coords_of_mouse.x;
	//the_rect_selection_stuff.selection_rect.top
	//	= n_starting_block_grid_coords_of_mouse.y;
	
	the_rect_selection_stuff.single_sprite_selected = false;
	
	the_rect_selection_stuff.enabled = true;
	the_rect_selection_stuff.mouse_released = false;
	//the_rect_selection_stuff.selection_layer = n_selection_layer;
	the_rect_selection_stuff.moving = true;
	
	the_rect_selection_stuff.clicked_location_in_rect
		= n_clicked_location_in_rect;
	
	the_rect_selection_stuff.starting_block_grid_coords_of_mouse
		= the_rect_selection_stuff.starting_block_grid_coords
		+ n_clicked_location_in_rect;
}

void sfml_canvas_widget::continue_moving_rect_selection_contents
	( const vec2_s32 curr_block_grid_coords_of_mouse )
{
	//cout << "continue_moving_rect_selection_contents()\n";
	
	
	// Right now, this just moves the selection_rect itself, but not the
	// contents (yet).
	
	vec2_s32& starting_block_grid_coords_of_mouse 
		= the_rect_selection_stuff.starting_block_grid_coords_of_mouse;
	vec2_s32& starting_block_grid_coords 
		= the_rect_selection_stuff.starting_block_grid_coords;
	vec2_s32& ending_block_grid_coords 
		= the_rect_selection_stuff.ending_block_grid_coords;
	
	vec2_s32& clicked_location_in_rect
		= the_rect_selection_stuff.clicked_location_in_rect;
	
	
	//cout << "clicked_location_in_rect:  "
	//	<< clicked_location_in_rect.x << ", " << clicked_location_in_rect.y
	//	<< "\n";
	
	vec2_s32 ending_block_grid_coords_offset
		= ending_block_grid_coords - starting_block_grid_coords;
	
	//cout << "ending_block_grid_coords_offset:  " 
	//	<< ending_block_grid_coords_offset.x << ", "
	//	<< ending_block_grid_coords_offset.y << endl;
	
	//starting_block_grid_coords = starting_block_grid_coords_of_mouse
	//	+ ( curr_block_grid_coords_of_mouse 
	//	- starting_block_grid_coords_of_mouse ) + clicked_location_in_rect;
	
	starting_block_grid_coords = curr_block_grid_coords_of_mouse
		- clicked_location_in_rect;
	
	// Correction things.
	if ( starting_block_grid_coords.x < 0 )
	{
		starting_block_grid_coords.x = 0;
	}
	if ( starting_block_grid_coords.y < 0 )
	{
		starting_block_grid_coords.y = 0;
	}
	if ( starting_block_grid_coords.x >= ( (s32)the_sublevel->size_2d.x 
		- ending_block_grid_coords_offset.x ) )
	{
		starting_block_grid_coords.x = ( the_sublevel->size_2d.x - 1 )
			- ending_block_grid_coords_offset.x;
	}
	if ( starting_block_grid_coords.y >= ( (s32)the_sublevel->size_2d.y 
		- ending_block_grid_coords_offset.y ) )
	{
		starting_block_grid_coords.y = ( the_sublevel->size_2d.y - 1 )
			- ending_block_grid_coords_offset.y;
		
	}
	
	ending_block_grid_coords = starting_block_grid_coords
		+ ending_block_grid_coords_offset;
	
	if ( ending_block_grid_coords.x < ending_block_grid_coords_offset.x )
	{
		ending_block_grid_coords.x = ending_block_grid_coords_offset.x;
	}
	if ( ending_block_grid_coords.y < ending_block_grid_coords_offset.y )
	{
		ending_block_grid_coords.y = ending_block_grid_coords_offset.y;
	}
	if ( ending_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		ending_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( ending_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		ending_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	the_rect_selection_stuff.selection_rect.left 
		= starting_block_grid_coords.x;
	the_rect_selection_stuff.selection_rect.top 
		= starting_block_grid_coords.y;
	
	the_rect_selection_stuff.selection_rect.width 
		= ending_block_grid_coords.x - starting_block_grid_coords.x + 1;
	the_rect_selection_stuff.selection_rect.height 
		= ending_block_grid_coords.y - starting_block_grid_coords.y + 1;
	
}

void sfml_canvas_widget::finish_moving_rect_selection_contents()
{
	//cout << "finish_moving_rect_selection_contents()\n"; 
	the_rect_selection_stuff.mouse_released = true;
	the_rect_selection_stuff.moving = false;
	
	// Need to do a bunch more things.
}





void sfml_canvas_widget::set_the_block_selector_core_widget
	( block_selector_core_widget* n_the_block_selector_core_widget )
{
	the_block_selector_core_widget = n_the_block_selector_core_widget;
}
void sfml_canvas_widget::set_the_sprite_16x16_selector_core_widget
	( sprite_16x16_selector_core_widget* 
	n_the_sprite_16x16_selector_core_widget )
{
	the_sprite_16x16_selector_core_widget 
		= n_the_sprite_16x16_selector_core_widget;
}
void sfml_canvas_widget::set_the_sprite_16x32_selector_core_widget
	( sprite_16x32_selector_core_widget* 
	n_the_sprite_16x32_selector_core_widget )
{
	the_sprite_16x32_selector_core_widget 
		= n_the_sprite_16x32_selector_core_widget;
}

//void sfml_canvas_widget::set_the_mouse_mode( mouse_mode* n_the_mouse_mode )
//{
//	the_mouse_mode = n_the_mouse_mode;
//}


void sfml_canvas_widget::generate_block_grid()
{
	//if ( scale_factor < minimum_scale_factor_for_block_grid )
	//{
	//	//canvas_block_grid_render_texture.create( 1, 1 );
	//	//canvas_block_grid_render_texture.clear(sf::Color::Black);
	//	return;
	//}
	
	if ( !get_block_grid_enabled() )
	{
		return;
	}
	
	sf::FloatRect visible_rect = get_visible_rect();

	vec2<double> visible_block_grid_start_pos
		( (double)visible_rect.left 
		/ (double)( num_pixels_per_block_column * scale_factor ), 
		(double)visible_rect.top / (double)( num_pixels_per_block_row
		* scale_factor ) );
	vec2<double> visible_block_grid_size_2d
		( (double)visible_rect.width 
		/ (double)( num_pixels_per_block_column * scale_factor ), 
		(double)visible_rect.height / (double)( num_pixels_per_block_row 
		* scale_factor ) );
	
	//cout << visible_block_grid_size_2d.x << ", "
	//	<< visible_block_grid_size_2d.y << endl;
	
	++visible_block_grid_size_2d.x;
	++visible_block_grid_size_2d.y;
	
	++visible_block_grid_size_2d.x;
	++visible_block_grid_size_2d.y;
	
	//if ( ( visible_block_grid_start_pos.x + visible_block_grid_size_2d.x ) 
	//	> ( the_sublevel->real_size_2d.x ) )
	//{
	//	cout << "visible_block_grid_size_2d.y too large\n";
	//	--visible_block_grid_size_2d.x;
	//}
	//if ( ( visible_block_grid_start_pos.y + visible_block_grid_size_2d.y ) 
	//	> ( the_sublevel->real_size_2d.y ) )
	//{
	//	cout << "visible_block_grid_size_2d.y too large\n";
	//	--visible_block_grid_size_2d.y;
	//}
	
	
	
	// Only render to the visible area and a little bit more (for good
	// measure).
	
	the_block_grid_stuff.slot_image.reset(new sf::Image);
	the_block_grid_stuff.slot_image->create
		( num_pixels_per_block_column * scale_factor, 
		num_pixels_per_block_row * scale_factor, 
		sf::Color( 0, 0, 0, 0 ) );
	
	// Vertical dotted line
	//for ( u32 j=0; j<the_block_grid_stuff.slot_image->getSize().y; ++j )
	for ( u32 j=0; j<the_block_grid_stuff.slot_image->getSize().y; j+=2 )
	{
		//the_block_grid_stuff.slot_image->setPixel
		//	( the_block_grid_stuff.slot_image->getSize().x - 1, j,
		//	sf::Color::Blue );
		the_block_grid_stuff.slot_image->setPixel
			( the_block_grid_stuff.slot_image->getSize().x - 1, j,
			sf::Color( 0, 0, 64 ) );
		//the_block_grid_stuff.slot_image->setPixel( 0, j, sf::Color::Blue );
	}
	
	// Horizontal dotted line
	//for ( u32 i=0; i<the_block_grid_stuff.slot_image->getSize().x; ++i )
	for ( u32 i=0; i<the_block_grid_stuff.slot_image->getSize().x; i+=2 )
	{
		//the_block_grid_stuff.slot_image->setPixel( i, 
		//	the_block_grid_stuff.slot_image->getSize().y - 1,
		//	sf::Color::Blue );
		the_block_grid_stuff.slot_image->setPixel( i, 
			the_block_grid_stuff.slot_image->getSize().y - 1,
			sf::Color( 0, 0, 64 ) );
		//the_block_grid_stuff.slot_image->setPixel( i, 0, sf::Color::Blue );
	}
	
	the_block_grid_stuff.slot_texture.reset(new sf::Texture);
	the_block_grid_stuff.slot_texture->loadFromImage
		(*the_block_grid_stuff.slot_image);
	
	the_block_grid_stuff.slot_sprite.reset(new sf::Sprite);
	the_block_grid_stuff.slot_sprite->setTexture
		(*the_block_grid_stuff.slot_texture);
	
	for ( u32 j=0; j<(u32)visible_block_grid_size_2d.y; ++j )
	{
		for ( u32 i=0; i<(u32)visible_block_grid_size_2d.x; ++i )
		{
			vec2_u32 block_grid_pos
				( i + (u32)visible_block_grid_start_pos.x,
				j + (u32)visible_block_grid_start_pos.y );
			
			//if ( block_grid_pos.x >= the_sublevel->real_size_2d.x
			//	|| block_grid_pos.y >= the_sublevel->real_size_2d.y )
			//{
			//	cout << "block_grid_pos out of bounds:  "
			//		<< block_grid_pos.x << ", " << block_grid_pos.y
			//		<< endl;
			//	continue;
			//}
			
			//the_block_grid_stuff.slot_sprite->setScale( 1.0f, -1.0f );
			the_block_grid_stuff.slot_sprite->setPosition
				( block_grid_pos.x * num_pixels_per_block_column 
				* scale_factor, block_grid_pos.y 
				* num_pixels_per_block_row * scale_factor );
			
			//cout << the_block_grid_stuff.slot_sprite->getPosition().x
			//	<< ", " 
			//	<< the_block_grid_stuff.slot_sprite->getPosition().y
			//	<< endl;
			
			draw(*the_block_grid_stuff.slot_sprite);
		}
	}
	
}

void sfml_canvas_widget::generate_rect_selection_rect()
{
	if ( !get_rect_selection_enabled() )
	{
		return;
	}
	
	const sf::IntRect& selection_rect 
		= the_rect_selection_stuff.selection_rect;
	
	//cout << selection_rect.left << ", " << selection_rect.top << ", "
	//	<< selection_rect.width << ", " << selection_rect.height << endl;
	
	the_rect_selection_stuff.selection_image.reset(new sf::Image);
	the_rect_selection_stuff.selection_image->create( selection_rect.width
		* num_pixels_per_block_column * scale_factor, 
		selection_rect.height * num_pixels_per_block_row * scale_factor, 
		sf::Color( 0, 0, 0, 0 ) );
	
	// Vertical lines
	for ( u32 j=0; 
		j<the_rect_selection_stuff.selection_image->getSize().y;
		++j )
	{
		//the_rect_selection_stuff.selection_image->setPixel( 0, j,
		//	sf::Color( 128, 0, 0 ) );
		//the_rect_selection_stuff.selection_image->setPixel
		//	( the_rect_selection_stuff.selection_image->getSize().x - 1, 
		//	j, sf::Color( 128, 0, 0 ) );
		
		
		
		if ( the_rect_selection_stuff.selection_layer == rsl_blocks )
		{
			the_rect_selection_stuff.selection_image->setPixel( 0, j,
				sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel( 1, j,
				sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 2, j, sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 1, j, sf::Color::Blue );
			
		}
		else if ( the_rect_selection_stuff.selection_layer == rsl_sprites 
			&& the_rect_selection_stuff.single_sprite_selected )
		{
			the_rect_selection_stuff.selection_image->setPixel( 0, j,
				sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel( 1, j,
				sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 2, j, sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 1, j, sf::Color::Red );
		}
		else if ( the_rect_selection_stuff.selection_layer == rsl_sprites 
			&& !the_rect_selection_stuff.single_sprite_selected )
		{
			the_rect_selection_stuff.selection_image->setPixel( 0, j,
				sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel( 1, j,
				sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 2, j, sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel
				( the_rect_selection_stuff.selection_image->getSize().x 
				- 1, j, sf::Color( 0, 188, 0 ) );
		}
	}
	
	// Horizontal lines
	for ( u32 i=0; 
		i<the_rect_selection_stuff.selection_image->getSize().x;
		++i )
	{
		//the_rect_selection_stuff.selection_image->setPixel( i, 0,
		//	sf::Color( 128, 0, 0 ) );
		//the_rect_selection_stuff.selection_image->setPixel( i, 
		//	the_rect_selection_stuff.selection_image->getSize().y - 1,
		//	sf::Color( 128, 0, 0 ) );
		
		if ( the_rect_selection_stuff.selection_layer == rsl_blocks )
		{
			the_rect_selection_stuff.selection_image->setPixel( i, 0,
				sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel( i, 1,
				sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 2,
				sf::Color::Blue );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 1,
				sf::Color::Blue );
		}
		else if ( the_rect_selection_stuff.selection_layer == rsl_sprites 
			&& the_rect_selection_stuff.single_sprite_selected )
		{
			the_rect_selection_stuff.selection_image->setPixel( i, 0,
				sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel( i, 1,
				sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 2,
				sf::Color::Red );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 1,
				sf::Color::Red );
		}
		else if ( the_rect_selection_stuff.selection_layer == rsl_sprites 
			&& !the_rect_selection_stuff.single_sprite_selected )
		{
			the_rect_selection_stuff.selection_image->setPixel( i, 0,
				sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel( i, 1,
				sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 2,
				sf::Color( 0, 188, 0 ) );
			the_rect_selection_stuff.selection_image->setPixel( i, 
				the_rect_selection_stuff.selection_image->getSize().y - 1,
				sf::Color( 0, 188, 0 ) );
		}
	}
	
	
	the_rect_selection_stuff.selection_texture.reset(new sf::Texture);
	the_rect_selection_stuff.selection_texture->loadFromImage
		(*the_rect_selection_stuff.selection_image);
	
	the_rect_selection_stuff.selection_sprite.reset(new sf::Sprite);
	the_rect_selection_stuff.selection_sprite->setTexture
		(*the_rect_selection_stuff.selection_texture);
	
	the_rect_selection_stuff.selection_sprite->setPosition
		( selection_rect.left * num_pixels_per_block_column * scale_factor,
		selection_rect.top * num_pixels_per_block_row * scale_factor );
	
	draw(*the_rect_selection_stuff.selection_sprite);
	
}


void sfml_canvas_widget::update_visible_area()
{
	sf::FloatRect visible_rect = get_visible_rect();
	
	//canvas_render_texture.create ( visible_rect.width, 
	//	visible_rect.height );
	//canvas_render_texture.clear(sf::Color::Cyan);
	
	
	vec2<double> visible_block_grid_start_pos( (double)visible_rect.left 
		/ (double)( num_pixels_per_block_column * scale_factor ), 
		(double)visible_rect.top / (double)( num_pixels_per_block_row
		* scale_factor ) );
	vec2<double> visible_block_grid_size_2d( (double)visible_rect.width 
		/ (double)( num_pixels_per_block_column * scale_factor ), 
		(double)visible_rect.height / (double)( num_pixels_per_block_row
		* scale_factor ) );
	
	
	
	
	// This is so that sprites larger than 16x16 pixels will be drawn if
	// their starting position is offscreen but part of them still IS on
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
	
	
	// Draw the visible blocks
	sf::Sprite sprite_for_drawing_level_elements;
	sprite_for_drawing_level_elements.setTexture
		(the_block_selector_core_widget
		->get_level_element_gfx_raw_texture());
	
	//double offset_x = visible_block_grid_start_pos.x 
	//	- (u32)visible_block_grid_start_pos.x, 
	//	offset_y = visible_block_grid_start_pos.y
	//	- (u32)visible_block_grid_start_pos.y;
	//
	//cout << offset_x << ", " << offset_y << endl;
	
	u32 num_drawn_blocks = 0;
	
	for ( s32 j=0; j<visible_block_grid_size_2d.y; ++j )
	{
		vec2_s32 block_grid_pos( 0, 
			j + visible_block_grid_start_pos.y );
		
		for ( s32 i=0; i<visible_block_grid_size_2d.x; ++i )
		{
			block_grid_pos.x = i + visible_block_grid_start_pos.x;
			
			if ( block_grid_pos.x < 0 
				|| block_grid_pos.x >= (s32)the_sublevel->real_size_2d.x
				|| block_grid_pos.y < 0 
				|| block_grid_pos.y >= (s32)the_sublevel->real_size_2d.y )
			{
				//cout << "block_grid_pos out of bounds:  "
				//	<< block_grid_pos.x << ", " << block_grid_pos.y
				//	<< endl;
				continue;
			}
			
			
			block& the_block = the_sublevel->uncompressed_block_data_vec_2d
				.at((u32)block_grid_pos.y).at((u32)block_grid_pos.x);
			
			
			if ( the_block.type != bt_air )
			{
				sprite_for_drawing_level_elements.setTextureRect
					( the_block_selector_core_widget
					->get_texture_rect_of_other_index(the_block.type) );
				
				sprite_for_drawing_level_elements.setScale( scale_factor, 
					scale_factor );
				
				sprite_for_drawing_level_elements.setPosition
					( (u32)block_grid_pos.x * num_pixels_per_block_column
					* scale_factor, (u32)block_grid_pos.y 
					* num_pixels_per_block_row * scale_factor );
				
				draw(sprite_for_drawing_level_elements);
				
				++num_drawn_blocks;
				
				
				//cout << endl;
			}
			
			//cout << i << " ";
		}
		
		
		//cout << "; " << j << endl;
	}
	//cout << num_drawn_blocks << endl;
	
	
	// Draw 16x16 sprites
	sprite_for_drawing_level_elements.setTexture
		(the_sprite_16x16_selector_core_widget
		->get_level_element_gfx_raw_texture());
	
	u32 num_drawn_16x16_sprites = 0;
	
	for ( s32 j=0; j<visible_block_grid_size_2d.y; ++j )
	{
		vec2_s32 block_grid_pos( 0,
			j + visible_block_grid_start_pos.y );
		
		for ( s32 i=0; i<visible_block_grid_size_2d.x; ++i )
		{
			block_grid_pos.x = i + visible_block_grid_start_pos.x;
			
			
			if ( block_grid_pos.x < 0 
				|| block_grid_pos.x >= (s32)the_sublevel->real_size_2d.x
				|| block_grid_pos.y < 0
				|| block_grid_pos.y >= (s32)the_sublevel->real_size_2d.y )
			{
				//cout << "block_grid_pos out of bounds:  "
				//	<< block_grid_pos.x << ", " << block_grid_pos.y
				//	<< endl;
				continue;
			}
			
			sprite_init_param_group_with_size& the_sprite_ipgws
				= the_sublevel->sprite_ipgws_vec_2d
				.at((u32)block_grid_pos.y).at((u32)block_grid_pos.x);
			
			if ( the_sprite_ipgws.type != st_default 
				&& the_sprite_ipgws.size_2d.x == 16
				&& the_sprite_ipgws.size_2d.y == 16 )
			{
				sprite_for_drawing_level_elements.setTextureRect
					( the_sprite_16x16_selector_core_widget
					->get_texture_rect_of_other_index
					(the_sprite_ipgws.type) );
				
				if (!the_sprite_ipgws.facing_right)
				{
					sprite_for_drawing_level_elements.setScale
						( scale_factor, scale_factor );
					
					sprite_for_drawing_level_elements.setPosition
						( (u32)block_grid_pos.x
						* num_pixels_per_block_column * scale_factor, 
						(u32)block_grid_pos.y 
						* num_pixels_per_block_row * scale_factor );
				}
				else //if (the_sprite_ipgws.facing_right)
				{
					sprite_for_drawing_level_elements.setScale
						( -1 * (s32)scale_factor, scale_factor );
					
					sprite_for_drawing_level_elements.setPosition
						( ( (u32)block_grid_pos.x + 1 )
						* num_pixels_per_block_column * scale_factor, 
						(u32)block_grid_pos.y 
						* num_pixels_per_block_row * scale_factor );
					
				}
				
				
				draw(sprite_for_drawing_level_elements);
				
				if ( the_sprite_ipgws.type == st_door )
				{
					door_number_sprite.setScale( scale_factor, 
						scale_factor );
					
					sf::IntRect n_door_number_sprite_texture_rect;
					
					const u32 the_door_number 
						= the_sprite_ipgws.extra_param_2;
					
					n_door_number_sprite_texture_rect.left
						= ( the_door_number 
						% door_number_gfx_num_slots_per_row ) 
						* door_number_gfx_slot_width; 
					n_door_number_sprite_texture_rect.top
						= ( the_door_number 
						/ door_number_gfx_num_slots_per_row )
						* door_number_gfx_slot_height; 
					n_door_number_sprite_texture_rect.width 
						= door_number_gfx_slot_width;
					n_door_number_sprite_texture_rect.height 
						= door_number_gfx_slot_height;
					
					door_number_sprite.setTextureRect
						(n_door_number_sprite_texture_rect);
					
					if (!the_sprite_ipgws.facing_right)
					{
						door_number_sprite.setPosition
							( sprite_for_drawing_level_elements
							.getPosition().x, 
							sprite_for_drawing_level_elements
							.getPosition().y 
							+ ( num_pixels_per_block_row 
							* scale_factor ) );
					}
					else //if (the_sprite_ipgws.facing_right)
					{
						door_number_sprite.setPosition
							( sprite_for_drawing_level_elements
							.getPosition().x 
							- ( num_pixels_per_block_column 
							* scale_factor ), 
							sprite_for_drawing_level_elements
							.getPosition().y 
							+ ( num_pixels_per_block_row 
							* scale_factor ) );
					}
					
					draw(door_number_sprite);
				}
				
				++num_drawn_16x16_sprites;
				
				
				//cout << endl;
			}
			
			//cout << i << " ";
		}
		//cout << "; " << j << endl;
	}
	//cout << num_drawn_16x16_sprites << endl;
	//cout << endl;
	
	
	// Draw 16x32 sprites
	sprite_for_drawing_level_elements.setTexture
		(the_sprite_16x32_selector_core_widget
		->get_level_element_gfx_raw_texture());
	
	u32 num_drawn_16x32_sprites = 0;
	
	for ( s32 j=0; j<visible_block_grid_size_2d.y; ++j )
	{
		vec2_s32 block_grid_pos( 0,
			j + visible_block_grid_start_pos.y );
		
		for ( s32 i=0; i<visible_block_grid_size_2d.x; ++i )
		{
			block_grid_pos.x = i + visible_block_grid_start_pos.x;
			
			if ( block_grid_pos.x < 0 
				|| block_grid_pos.x >= (s32)the_sublevel->real_size_2d.x
				|| block_grid_pos.y < 0
				|| block_grid_pos.y >= (s32)the_sublevel->real_size_2d.y )
			{
				//cout << "block_grid_pos out of bounds:  "
				//	<< block_grid_pos.x << ", " << block_grid_pos.y
				//	<< endl;
				continue;
			}
			
			sprite_init_param_group_with_size& the_sprite_ipgws
				= the_sublevel->sprite_ipgws_vec_2d
				.at((u32)block_grid_pos.y).at((u32)block_grid_pos.x);
			
			if ( the_sprite_ipgws.type != st_default 
				&& the_sprite_ipgws.size_2d.x == 16
				&& the_sprite_ipgws.size_2d.y == 32 )
			{
				sprite_for_drawing_level_elements.setTextureRect
					( the_sprite_16x32_selector_core_widget
					->get_texture_rect_of_other_index
					(the_sprite_ipgws.type) );
				
				if (!the_sprite_ipgws.facing_right)
				{
					sprite_for_drawing_level_elements.setScale
						( scale_factor, scale_factor );
					
					sprite_for_drawing_level_elements.setPosition
						( (u32)block_grid_pos.x 
						* num_pixels_per_block_column * scale_factor, 
						(u32)block_grid_pos.y 
						* num_pixels_per_block_row * scale_factor );
				}
				else //if (the_sprite_ipgws.facing_right)
				{
					sprite_for_drawing_level_elements.setScale
						( -1 * (s32)scale_factor, scale_factor );
					
					sprite_for_drawing_level_elements.setPosition
						( ( (u32)block_grid_pos.x + 1 )
						* num_pixels_per_block_column * scale_factor, 
						(u32)block_grid_pos.y 
						* num_pixels_per_block_row * scale_factor );
				}
				
				
				draw(sprite_for_drawing_level_elements);
				
				if ( the_sprite_ipgws.type == st_door )
				{
					door_number_sprite.setScale( scale_factor, 
						scale_factor );
					
					sf::IntRect n_door_number_sprite_texture_rect;
					
					const u32 the_door_number 
						= the_sprite_ipgws.extra_param_2;
					
					n_door_number_sprite_texture_rect.left
						= ( the_door_number 
						% door_number_gfx_num_slots_per_row ) 
						* door_number_gfx_slot_width; 
					n_door_number_sprite_texture_rect.top
						= ( the_door_number 
						/ door_number_gfx_num_slots_per_row )
						* door_number_gfx_slot_height; 
					n_door_number_sprite_texture_rect.width 
						= door_number_gfx_slot_width;
					n_door_number_sprite_texture_rect.height 
						= door_number_gfx_slot_height;
					
					door_number_sprite.setTextureRect
						(n_door_number_sprite_texture_rect);
					
					if (!the_sprite_ipgws.facing_right)
					{
						door_number_sprite.setPosition
							( sprite_for_drawing_level_elements
							.getPosition().x, 
							sprite_for_drawing_level_elements
							.getPosition().y 
							+ ( num_pixels_per_block_row 
							* scale_factor ) );
					}
					else //if (the_sprite_ipgws.facing_right)
					{
						door_number_sprite.setPosition
							( sprite_for_drawing_level_elements
							.getPosition().x 
							- ( num_pixels_per_block_column 
							* scale_factor ), 
							sprite_for_drawing_level_elements
							.getPosition().y 
							+ ( num_pixels_per_block_row 
							* scale_factor ) );
					}
					
					draw(door_number_sprite);
				}
				
				++num_drawn_16x32_sprites;
				
				
				//cout << endl;
			}
			
			//cout << i << " ";
		}
		//cout << "; " << j << endl;
	}
	//cout << num_drawn_16x32_sprites << endl;
	//cout << endl;
	
	generate_block_grid();
	generate_rect_selection_rect();
	
	//// Draw a rectangle around the selected sprite
	//if ( *the_mouse_mode == mm_select_sprites )
	//{
	//	
	//}
}


void sfml_canvas_widget::on_update()
{
	if ( zoomed_in_recently || zoomed_out_recently )
	{
		auto zoom_func = [&]( bool& zoomed_recently ) -> void
		{
			zoomed_recently = false;
			
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this);
			//cout << "mouse_pos:  " << mouse_pos.x << ", " << mouse_pos.y 
			//	<< endl;
			
			sf::Vector2f mouse_pos_scaled( (double)mouse_pos.x 
				/ (double)scale_factor, (double)mouse_pos.y 
				/ (double)scale_factor );
			cout << "mouse_pos_scaled:  " << mouse_pos_scaled.x << ", " 
				<< mouse_pos_scaled.y << endl;
			
			int horiz_sb_val_before 
				= scroll_area->horizontalScrollBar()->value();
			int horiz_sb_min_before
				= scroll_area->horizontalScrollBar()->minimum();
			int horiz_sb_max_before
				= scroll_area->horizontalScrollBar()->maximum();
			
			int vert_sb_val_before 
				= scroll_area->verticalScrollBar()->value();
			int vert_sb_min_before
				= scroll_area->verticalScrollBar()->minimum();
			int vert_sb_max_before
				= scroll_area->verticalScrollBar()->maximum();
			
			//cout << "before:  " << getSize().x << ", " << getSize().y 
			//	<< endl;
			
			full_resize
				( QSize( unzoomed_size_2d.width() * scale_factor, 
				unzoomed_size_2d.height() * scale_factor ) );
			
			
			// Small hack that I'm not sure works for everyone or just me.
			// I don't think it's a big deal if one column of pixels at the
			// end of the level doesn't show up.  For your information, the
			// max size an sf::RenderWindow can be seems to limit how much
			// I can zoom in.  Perhaps I should come up with a way to not
			// need such a large sf::RenderWindow.
			if ( ( getSize().x + 1 ) < ( unzoomed_size_2d.width() 
				* scale_factor )
				|| getSize().y < ( unzoomed_size_2d.height() 
				* scale_factor ) )
			{
				cout << "after:  " << getSize().x << ", " << getSize().y 
					<< ";  " << ( unzoomed_size_2d.width() * scale_factor )
					<< ", " << ( unzoomed_size_2d.height() * scale_factor )
					<< endl;
				scale_factor >>= 1;
				
				full_resize
					( QSize( unzoomed_size_2d.width() * scale_factor, 
					unzoomed_size_2d.height() * scale_factor ) );
			}
			
			//cout << "after 2:  " << getSize().x << ", " << getSize().y 
			//	<< endl;
			
			
			generate_block_grid();
			
			//canvas_sprite->setScale( scale_factor, scale_factor );
			
			int horiz_sb_min_after
				= scroll_area->horizontalScrollBar()->minimum();
			int horiz_sb_max_after
				= scroll_area->horizontalScrollBar()->maximum();
			
			int vert_sb_min_after
				= scroll_area->verticalScrollBar()->minimum();
			int vert_sb_max_after
				= scroll_area->verticalScrollBar()->maximum();
			
			
			
			double horiz_sb_val_after = convert_range<double>
				( (double)horiz_sb_val_before,
				(double)horiz_sb_min_before,
				(double)horiz_sb_max_before,
				(double)horiz_sb_min_after, 
				(double)horiz_sb_max_after );
			
			scroll_area->horizontalScrollBar()->setValue
				((int)horiz_sb_val_after);
			
			double vert_sb_val_after = convert_range<double>
				( (double)vert_sb_val_before,
				(double)vert_sb_min_before,
				(double)vert_sb_max_before,
				(double)vert_sb_min_after, 
				(double)vert_sb_max_after );
			
			scroll_area->verticalScrollBar()->setValue
				((int)vert_sb_val_after);
		
			//cout << endl;
		};
		
		if ( zoomed_in_recently && scroll_area != NULL )
		{
			zoom_func(zoomed_in_recently);
		}
		
		if ( zoomed_out_recently && scroll_area != NULL )
		{
			zoom_func(zoomed_out_recently);
		}
		
		if ( scroll_area == NULL )
		{
			//full_resize(QSize( canvas_render_texture.getSize().x 
			//	* scale_factor, 
			//	canvas_render_texture.getSize().y 
			//	* scale_factor ));
			full_resize(QSize( unzoomed_size_2d.width() * scale_factor, 
				unzoomed_size_2d.height() * scale_factor ));
			
			if ( ( getSize().x + 1 ) < ( unzoomed_size_2d.width() 
				* scale_factor )
				|| getSize().y < ( unzoomed_size_2d.height() 
				* scale_factor ) )
			{
				scale_factor >>= 1;
				
				full_resize
					( QSize( unzoomed_size_2d.width() * scale_factor, 
					unzoomed_size_2d.height() * scale_factor ) );
			}
			
			generate_block_grid();
			
			//canvas_sprite->setScale( scale_factor, scale_factor );
			
			if (zoomed_in_recently)
			{
				zoomed_in_recently = false;
			}
			if (zoomed_out_recently)
			{
				zoomed_out_recently = false;
			}
		}
		
		
	}
	if (modified_recently)
	{
		modified_recently = false;
		//canvas_texture->loadFromImage(*canvas_image);
	}
	
	
	
	
	//clear(sf::Color( 0, 128, 0 ));
	
	//// This clear() call is probably not necessary.
	//clear(sf::Color::Cyan);
	clear(sf::Color::Cyan);
	
	
	// Draw sprites ON TOP OF blocks.
	update_visible_area();
	
	
}



