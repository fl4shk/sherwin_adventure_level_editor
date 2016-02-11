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
	
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	
	
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

//void sfml_canvas_widget::draw_line( const sf::Vector2i& pos_0, 
//	const sf::Vector2i& pos_1, const sf::Color& color )
//{
//	modified_recently = true;
//	
//	sf::Vector2i delta, pixel_coord, offset;
//	
//	delta = sf::Vector2i( pos_1.x - pos_0.x, pos_1.y - pos_0.y );
//	
//	if ( delta.x < 0 )
//	{
//		delta.x = -delta.x;
//	}
//	if ( delta.y < 0 )
//	{
//		delta.y = -delta.y;
//	}
//	
//	pixel_coord = pos_0;
//	
//	if ( pos_0.x > pos_1.x )
//	{
//		offset.x = -1;
//	}
//	else
//	{
//		offset.x = 1;
//	}
//	
//	if ( pos_0.y > pos_1.y )
//	{
//		offset.y = -1;
//	}
//	else
//	{
//		offset.y = 1;
//	}
//	
//	if ( point_is_in_image(pixel_coord) )
//	{
//		canvas_image->setPixel( (u32)pixel_coord.x, (u32)pixel_coord.y, 
//			color );
//	}
//	
//	if ( delta.x > delta.y )
//	{
//		s32 error = delta.x >> 1;
//		
//		while ( pixel_coord.x != pos_1.x )
//		{
//			error -= delta.y;
//			
//			if ( error < 0 )
//			{
//				pixel_coord.y += offset.y;
//				error += delta.x;
//			}
//			
//			pixel_coord.x += offset.x;
//			
//			if ( point_is_in_image(pixel_coord) )
//			{
//				canvas_image->setPixel( (u32)pixel_coord.x, 
//					(u32)pixel_coord.y, color );
//			}
//		}
//	}
//	else
//	{
//		s32 error = delta.y >> 1;
//		
//		while ( pixel_coord.y != pos_1.y )
//		{
//			error -= delta.x;
//			
//			if ( error < 0 )
//			{
//				pixel_coord.x += offset.x;
//				error += delta.y;
//			}
//			
//			pixel_coord.y += offset.y;
//			
//			if ( point_is_in_image(pixel_coord) )
//			{
//				canvas_image->setPixel( (u32)pixel_coord.x, 
//					(u32)pixel_coord.y, color );
//			}
//		}
//	}
//	
//}


//void sfml_canvas_widget::mousePressEvent( QMouseEvent* event )
//{
//	// This converts the clicked coordinate to pixel coordinates.
//	sf::Vector2f event_pos_in_image_coords = mapPixelToCoords
//		( sf::Vector2i( event->x(), event->y() ), get_apparent_view() );
//	
//	sf::Vector2i event_pos_in_image_pixel_coords
//		= sf::Vector2i( (int)event_pos_in_image_coords.x,
//		(int)event_pos_in_image_coords.y );
//	
//	prev_mouse_pos = event->pos();
//	
//	// Check whether the mouse was clicked somewhere inside the image.
//	if ( !point_is_in_image(event_pos_in_image_pixel_coords) )
//	{
//		return;
//	}
//	
//	modified_recently = true;
//	
//	if ( event->button() == Qt::LeftButton )
//	{
//		//canvas_image->setPixel( (u32)event_pos_in_image_pixel_coords.x,
//		//	(u32)event_pos_in_image_pixel_coords.y, 
//		//	the_block_selector_core_widget->palette.at
//		//	(the_block_selector_core_widget
//		//	->left_current_color_index) );
//	}
//	else if ( event->button() == Qt::RightButton )
//	{
//		//canvas_image->setPixel( (u32)event_pos_in_image_pixel_coords.x,
//		//	(u32)event_pos_in_image_pixel_coords.y, 
//		//	the_block_selector_core_widget->palette.at
//		//	(the_block_selector_core_widget
//		//	->right_current_color_index) );
//	}
//	
//}
//
//void sfml_canvas_widget::mouseMoveEvent( QMouseEvent* event )
//{
//	// This converts the clicked coordinate to pixel coordinates.
//	sf::Vector2f event_pos_in_image_coords = mapPixelToCoords
//		( sf::Vector2i( event->x(), event->y() ), get_apparent_view() );
//	
//	sf::Vector2i event_pos_in_image_pixel_coords
//		= sf::Vector2i( (int)event_pos_in_image_coords.x,
//		(int)event_pos_in_image_coords.y );
//	
//	// Check whether the mouse was moved to somewhere inside the image.
//	if ( !point_is_in_image(event_pos_in_image_pixel_coords) )
//	{
//		prev_mouse_pos = event->pos();
//		return;
//	}
//	
//	sf::Vector2f prev_mouse_pos_in_image_coords = mapPixelToCoords
//		( sf::Vector2i( prev_mouse_pos.x(), prev_mouse_pos.y() ), 
//		get_apparent_view() );
//	
//	sf::Vector2i prev_mouse_pos_in_image_pixel_coords
//		= sf::Vector2i( (int)prev_mouse_pos_in_image_coords.x,
//		(int)prev_mouse_pos_in_image_coords.y );
//	
//	prev_mouse_pos = event->pos();
//	
//	//modified_recently = true;
//	//draw_line( prev_mouse_pos_in_image_pixel_coords,
//	//	event_pos_in_image_pixel_coords, sf::Color::Black );
//	
//	if ( event->buttons() == Qt::LeftButton )
//	{
//		//draw_line( prev_mouse_pos_in_image_pixel_coords,
//		//	event_pos_in_image_pixel_coords, 
//		//	the_block_selector_core_widget->palette.at
//		//	(the_block_selector_core_widget
//		//	->left_current_color_index) );
//	}
//	else if ( event->buttons() == Qt::RightButton )
//	{
//		//draw_line( prev_mouse_pos_in_image_pixel_coords,
//		//	event_pos_in_image_pixel_coords, 
//		//	the_block_selector_core_widget->palette.at
//		//	(the_block_selector_core_widget
//		//	->right_current_color_index) );
//	}
//	
//}
//
//// mouseReleaseEvent() will come in handy once a "line" tool exists.
//void sfml_canvas_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}


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
		( (double)visible_rect.left / (double)( num_pixels_per_block_row
		* scale_factor ), 
		(double)visible_rect.top / (double)( num_pixels_per_block_column
		* scale_factor ) );
	vec2<double> visible_block_grid_size_2d
		( (double)visible_rect.width / (double)( num_pixels_per_block_row
		* scale_factor ), 
		(double)visible_rect.height 
		/ (double)( num_pixels_per_block_column * scale_factor ) );
	
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
				( block_grid_pos.x * num_pixels_per_block_row 
				* scale_factor, block_grid_pos.y 
				* num_pixels_per_block_column * scale_factor );
			
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
	
	cout << selection_rect.left << ", " << selection_rect.top << ", "
		<< selection_rect.width << ", " << selection_rect.height << endl;
	
}

void sfml_canvas_widget::update_visible_area()
{
	sf::FloatRect visible_rect = get_visible_rect();
	
	//canvas_render_texture.create ( visible_rect.width, 
	//	visible_rect.height );
	//canvas_render_texture.clear(sf::Color::Cyan);
	
	
	vec2<double> visible_block_grid_start_pos
		( (double)visible_rect.left / (double)( num_pixels_per_block_row
		* scale_factor ), 
		(double)visible_rect.top / (double)( num_pixels_per_block_column
		* scale_factor ) );
	vec2<double> visible_block_grid_size_2d
		( (double)visible_rect.width / (double)( num_pixels_per_block_row
		* scale_factor ), 
		(double)visible_rect.height / (double)( num_pixels_per_block_column
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
					( (u32)block_grid_pos.x * num_pixels_per_block_row
					* scale_factor, (u32)block_grid_pos.y 
					* num_pixels_per_block_column * scale_factor );
				
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
				
				sprite_for_drawing_level_elements.setScale( scale_factor, 
					scale_factor );
				
				sprite_for_drawing_level_elements.setPosition
					( (u32)block_grid_pos.x * num_pixels_per_block_row
					* scale_factor, (u32)block_grid_pos.y 
					* num_pixels_per_block_column * scale_factor );
				
				draw(sprite_for_drawing_level_elements);
				
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
				
				sprite_for_drawing_level_elements.setScale( scale_factor, 
					scale_factor );
				
				sprite_for_drawing_level_elements.setPosition
					( (u32)block_grid_pos.x * num_pixels_per_block_row
					* scale_factor, (u32)block_grid_pos.y 
					* num_pixels_per_block_column * scale_factor );
				
				draw(sprite_for_drawing_level_elements);
				
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



