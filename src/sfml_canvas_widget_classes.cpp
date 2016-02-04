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
	
	block_grid_enabled_recently(false), block_grid_enabled(false), 
	
	scroll_area(NULL),
	
	modified_recently(false), 
	
	zoomed_in_recently(false), zoomed_out_recently(false),
	
	scale_factor(1), 
	
	view_center_x(0.0f), view_center_y(0.0f)
{
	apparent_view = getDefaultView();
	
	//canvas_image.reset(new sf::Image);
	//canvas_image->create( s_size.width(), s_size.height(), 
	//	sf::Color::Cyan );
	//
	//canvas_texture.reset(new sf::Texture);
	//canvas_texture->loadFromImage(*canvas_image);
	//
	//canvas_sprite.reset(new sf::Sprite);
	//canvas_sprite->setTexture(*canvas_texture);
	
	//canvas_render_texture.create( s_size.width(), s_size.height(),
	//	sf::Color::Cyan );
	
	canvas_render_texture.create( s_size.width(), s_size.height() );
	canvas_render_texture.clear(sf::Color::Cyan);
	
}

sfml_canvas_widget::sfml_canvas_widget( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, 
	QScrollArea* s_scroll_area ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	
	block_grid_enabled_recently(false), block_grid_enabled(false), 
	
	scroll_area(s_scroll_area),
	
	modified_recently(false), 
	zoomed_in_recently(false), zoomed_out_recently(false), 
	
	scale_factor(1), 
	
	view_center_x(0.0f), view_center_y(0.0f)
{
	apparent_view = getDefaultView();
	
	//canvas_image.reset(new sf::Image);
	//canvas_image->create( s_size.width(), s_size.height(), 
	//	sf::Color::Cyan );
	//
	//canvas_texture.reset(new sf::Texture);
	//canvas_texture->loadFromImage(*canvas_image);
	//
	//canvas_sprite.reset(new sf::Sprite);
	//canvas_sprite->setTexture(*canvas_texture);
	
	//canvas_render_texture.create( s_size.width(), s_size.height(),
	//	sf::Color::Cyan );
	
	canvas_render_texture.create( s_size.width(), s_size.height() );
	canvas_render_texture.clear(sf::Color::Cyan);
	
}



const sf::View& sfml_canvas_widget::get_apparent_view()
{
	apparent_view = getDefaultView();
	
	apparent_view.setCenter( view_center_x, view_center_y );
	
	//apparent_view.move( (double)( canvas_image->getSize().x ) 
	//	/ (double)2.0f, (double)( canvas_image->getSize().y )
	//	/ (double)2.0f );
	apparent_view.move( (double)( canvas_render_texture.getSize().x ) 
		/ (double)2.0f, (double)( canvas_render_texture.getSize().y )
		/ (double)2.0f );
	
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

void sfml_canvas_widget::enable_block_grid()
{
	block_grid_enabled_recently = true;
	block_grid_enabled = true;
	
}

void sfml_canvas_widget::disable_block_grid()
{
	block_grid_enabled = false;
}


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



void sfml_canvas_widget::generate_canvas_block_grid()
{
	//if ( scale_factor < minimum_scale_factor_for_block_grid )
	//{
	//	canvas_block_grid_render_texture.create( 1, 1 );
	//	canvas_block_grid_render_texture.clear(sf::Color::Black);
	//	return;
	//}
	
	if (block_grid_enabled_recently)
	{
		block_grid_enabled_recently = false;
	}
	
	if ( get_block_grid_enabled() )
	{
		canvas_block_grid_render_texture.create( getSize().x, 
			getSize().y );
		
		canvas_block_grid_slot_image.reset(new sf::Image);
		canvas_block_grid_slot_image->create
			( num_pixels_per_block_column * scale_factor, 
			num_pixels_per_block_row * scale_factor, 
			sf::Color( 0, 0, 0, 0 ) );
		
		// Vertical line
		for ( u32 j=0; j<canvas_block_grid_slot_image->getSize().y; ++j )
		{
			canvas_block_grid_slot_image->setPixel
				( canvas_block_grid_slot_image->getSize().x - 1, j,
				sf::Color::Blue );
		}
		
		// Horizontal line
		for ( u32 i=0; i<canvas_block_grid_slot_image->getSize().x; ++i )
		{
			//canvas_block_grid_slot_image->setPixel( i, 
			//	canvas_block_grid_slot_image->getSize().y - 1,
			//	sf::Color::Blue );
			canvas_block_grid_slot_image->setPixel( i, 0,
				sf::Color::Blue );
		}
		
		canvas_block_grid_slot_texture.reset(new sf::Texture);
		canvas_block_grid_slot_texture->loadFromImage
			(*canvas_block_grid_slot_image);
		
		canvas_block_grid_slot_sprite.reset(new sf::Sprite);
		canvas_block_grid_slot_sprite->setTexture
			(*canvas_block_grid_slot_texture);
		
		for ( u32 j=0; 
			j<canvas_render_texture.getSize().y 
				/ num_pixels_per_block_column;
			++j )
		{
			for ( u32 i=0; 
				i<canvas_render_texture.getSize().x 
					/ num_pixels_per_block_row; 
				++i )
			{
				canvas_block_grid_slot_sprite->setPosition
					( i * canvas_block_grid_slot_image->getSize().x, 
					j * canvas_block_grid_slot_image->getSize().y );
				canvas_block_grid_render_texture.draw
					(*canvas_block_grid_slot_sprite);
			}
		}
		
	}
	
	
}


void sfml_canvas_widget::on_update()
{
	if ( zoomed_in_recently || zoomed_out_recently )
	{
		auto zoom_func = [&]( bool& zoomed_recently ) -> void
		{
			zoomed_recently = false;
			
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this);
			cout << "mouse_pos:  " << mouse_pos.x << ", " << mouse_pos.y 
				<< endl;
			
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
			
			full_resize(QSize( canvas_render_texture.getSize().x 
				* scale_factor, canvas_render_texture.getSize().y 
				* scale_factor ));
			
			if ( get_block_grid_enabled() )
			{
				generate_canvas_block_grid();
			}
			
			//canvas_sprite->setScale( scale_factor, scale_factor );
			
			int horiz_sb_min_after
				= scroll_area->horizontalScrollBar()->minimum();
			int horiz_sb_max_after
				= scroll_area->horizontalScrollBar()->maximum();
			
			int vert_sb_min_after
				= scroll_area->verticalScrollBar()->minimum();
			int vert_sb_max_after
				= scroll_area->verticalScrollBar()->maximum();
			
			
			
			//cout << horiz_sb_min_after<< " " << horiz_sb_max_after<< ", "
			//	<< vert_sb_min_after<< " " << vert_sb_max_after<< endl;
			cout << "old max scrollbar values:  " << horiz_sb_max_before 
				<< ", " << vert_sb_max_before << endl;
			cout << "new max scrollbar values:  " << horiz_sb_max_after
				<< ", " << vert_sb_max_after << endl;
			
			
			
			//if ( mouse_pos.x < 0 )
			if ( mouse_pos_scaled.x < 0 )
			{
				//cout << "x < 0\n";
				scroll_area->horizontalScrollBar()->setValue
					(horiz_sb_min_after);
			}
			//else if ( mouse_pos.x
			//	> (int)canvas_render_texture.getSize().x )
			else if ( mouse_pos_scaled.x
				> (int)canvas_render_texture.getSize().x )
			{
				//cout << "x > getSize().x\n";
				scroll_area->horizontalScrollBar()->setValue
					(horiz_sb_max_after);
			}
			else
			{
				//cout << "x in range\n";
				
				//double horiz_sb_val_after = convert_range<double>
				//	( (double)mouse_pos.x, (double)0.0f, 
				//	(double)canvas_render_texture.getSize().x,
				//	(double)horiz_sb_min, (double)horiz_sb_max );
				//double horiz_sb_val_after = convert_range<double>
				//	( (double)mouse_pos_scaled.x, (double)0.0f, 
				//	(double)canvas_render_texture.getSize().x,
				//	(double)horiz_sb_min, (double)horiz_sb_max );
				
				
				//double horiz_sb_val_after = convert_range<double>
				//	( (double)horiz_sb_val_before,
				//	(double)horiz_sb_min_before,
				//	(double)horiz_sb_max_before,
				//	(double)horiz_sb_min_after, 
				//	(double)horiz_sb_max_after )
				//	- ( (double)mouse_pos.x / 2.0f );
				double horiz_sb_val_after = convert_range<double>
					( (double)horiz_sb_val_before,
					(double)horiz_sb_min_before,
					(double)horiz_sb_max_before,
					(double)horiz_sb_min_after, 
					(double)horiz_sb_max_after );
				
				cout << "horiz scrollbar old value:  " 
					<< horiz_sb_val_before << endl;
				cout << "horiz scrollbar new value:  " 
					<< horiz_sb_val_after << endl;
				
				scroll_area->horizontalScrollBar()->setValue
					((int)horiz_sb_val_after);
			}
			
			//if ( mouse_pos.y < 0 )
			if ( mouse_pos_scaled.y < 0 )
			{
				//cout << "y < 0\n";
				scroll_area->verticalScrollBar()->setValue
					(vert_sb_min_after);
			}
			//else if ( mouse_pos.y
			//	> (int)canvas_render_texture.getSize().y )
			else if ( mouse_pos_scaled.y
				> (int)canvas_render_texture.getSize().y )
			{
				//cout << "y > getSize().y\n";
				scroll_area->verticalScrollBar()->setValue
					(vert_sb_max_after);
			}
			else
			{
				//cout << "y in range\n";
				
				//double vert_sb_val_after = convert_range<double>
				//	( (double)mouse_pos.y, (double)0.0f, 
				//	(double)canvas_render_texture.getSize().y,
				//	(double)vert_sb_min, (double)vert_sb_max );
				//double vert_sb_val_after = convert_range<double>
				//	( (double)mouse_pos_scaled.y, (double)0.0f, 
				//	(double)canvas_render_texture.getSize().y,
				//	(double)vert_sb_min, (double)vert_sb_max );
				
				
				//double vert_sb_val_after = convert_range<double>
				//	( (double)vert_sb_val_before,
				//	(double)vert_sb_min_before,
				//	(double)vert_sb_max_before,
				//	(double)vert_sb_min_after, 
				//	(double)vert_sb_max_after )
				//	- ( (double)mouse_pos.y / 2.0f );
				double vert_sb_val_after = convert_range<double>
					( (double)vert_sb_val_before,
					(double)vert_sb_min_before,
					(double)vert_sb_max_before,
					(double)vert_sb_min_after, 
					(double)vert_sb_max_after );
				
				cout << "vert scrollbar old value:  " 
					<< vert_sb_val_before << endl;
				cout << "vert scrollbar new value:  " 
					<< vert_sb_val_after << endl;
				
				scroll_area->verticalScrollBar()->setValue
					((int)vert_sb_val_after);
			}
			
			cout << endl;
		};
		
		if ( scroll_area == NULL )
		{
			full_resize(QSize( canvas_render_texture.getSize().x 
				* scale_factor, canvas_render_texture.getSize().y 
				* scale_factor ));
			
			//full_resize(QSize( canvas_sprite->getSize().x * scale_factor, 
			//	canvas_sprite->getSize().y * scale_factor ));
			
			//full_resize(QSize( canvas_texture->getSize().x * scale_factor, 
			//	canvas_texture->getSize().y * scale_factor ));
			
			if ( get_block_grid_enabled() )
			{
				generate_canvas_block_grid();
			}
			
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
		
		
		else if (zoomed_in_recently)
		{
			zoom_func(zoomed_in_recently);
		}
		
		else if (zoomed_out_recently)
		{
			zoom_func(zoomed_out_recently);
		}
		
		
		
	}
	if (modified_recently)
	{
		modified_recently = false;
		//canvas_texture->loadFromImage(*canvas_image);
	}
	
	// Instead of generating the block grid every frame, only do it if it
	// has been recently enabled (or if it's enabled and zooming happened
	// recently).
	if (block_grid_enabled_recently)
	{
		generate_canvas_block_grid();
	}
	//generate_canvas_grid();
	
	//clear(sf::Color( 0, 128, 0 ));
	
	// This clear() call is probably not necessary.
	clear(sf::Color::Cyan);
	
	//draw(*canvas_sprite);
	
	canvas_sprite.reset(new sf::Sprite);
	canvas_sprite->setTexture(canvas_render_texture.getTexture());
	canvas_sprite->setScale( scale_factor, scale_factor );
	draw(*canvas_sprite);
	
	
	//if ( get_block_grid_enabled() 
	//	&& scale_factor >= minimum_scale_factor_for_block_grid )
	if ( get_block_grid_enabled() )
	{
		sf::Sprite canvas_block_grid_sprite
			(canvas_block_grid_render_texture.getTexture());
		draw(canvas_block_grid_sprite);
	}
}



