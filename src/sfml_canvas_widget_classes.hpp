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



#ifndef sfml_canvas_widget_classes_hpp
#define sfml_canvas_widget_classes_hpp

#include "misc_includes.hpp"
#include "sublevel_class.hpp"

#include "sprite_level_data_stuff.hpp"

class sfml_canvas_widget_base : public QWidget, public sf::RenderWindow
{
public:		// variables
	QWidget* parent;
	QTimer timer;
	bool initialized;
	
public:		// functions
	sfml_canvas_widget_base( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, u32 frame_time = 1 );
	virtual ~sfml_canvas_widget_base();
	
	
protected:		// functions
	virtual inline void full_resize( const QSize& n_size )
	{
		parent->resize(n_size);
		resize(n_size);
		sf::RenderWindow::create(winId());
	}
	inline void set_min_max_sizes( const QSize& n_size )
	{
		setMinimumSize(n_size);
		setMaximumSize(n_size);
	}
	
	virtual void on_init();
	virtual void on_update();
	virtual QPaintEngine* paintEngine() const;
	
	virtual void showEvent( QShowEvent* event );
	virtual void paintEvent( QPaintEvent* event );
};



class block_selector_core_widget;
class sprite_16x16_selector_core_widget;
class sprite_16x32_selector_core_widget;

class sfml_canvas_widget : public sfml_canvas_widget_base
{
protected:		// variables
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	sf::View apparent_view;
	
	
	// Block grid drawing stuffs
	struct
	{
		unique_ptr<sf::Image> slot_image;
		unique_ptr<sf::Texture> slot_texture;
		unique_ptr<sf::Sprite> slot_sprite;
		
		bool enabled;
	} the_block_grid_stuff;
	
	
	
	// Rectangular selection drawing stuffs.  This is intended to be used
	// for both individual level elements (like when a sprite is selected
	// so that its properties can be modified) and groups of level
	// elements.
	struct
	{
		unique_ptr<sf::Image> selection_image;
		unique_ptr<sf::Texture> selection_texture;
		unique_ptr<sf::Sprite> selection_sprite;
		
		
		// The actual rectangle representing the selected area.  It is
		// intended to have a position and size that are in block grid
		// coordinates.
		sf::IntRect selection_rect;
		
		
		
		// These are not intended to be used for the mouse_mode where
		// sprite properties can be changed.
		
		// This is the initially clicked block grid coordinate.
		vec2_s32 starting_block_grid_coords_of_mouse;
		
		// This represents the left and top values of selection_rect.
		vec2_s32 starting_block_grid_coords;
		
		// This represents the right and bottom values of selection_rect.
		vec2_s32 ending_block_grid_coords;
		
		
		
		
		// mouse_released is set to true when the mouse is released and the
		// selection mode is NOT for changing sprite properties.
		bool enabled, mouse_released;
		
		// Whether the rectangular selection is in the block layer or the
		// sprite layer.
		rect_selection_layer selection_layer;
		
		// This corresponds to when the level_editor_widget's
		// sprite_properties_widget should be generated.
		bool single_sprite_selected;
		
		
		
		// Stuff for moving around the selection's contents.
		bool moving;
		vec2_s32 clicked_location_in_rect;
		
	} the_rect_selection_stuff;
	
	
	// Door sprite number stuff
	static constexpr u32 door_number_gfx_slot_width = 16,
		door_number_gfx_slot_height = 16,
		door_number_gfx_num_slots_per_row = 13,
		door_number_gfx_num_slots = 52;
	static const string door_number_gfx_file_name;
	
	sf::Image door_number_gfx_raw_image;
	sf::Texture door_number_gfx_raw_texture;
	sf::Sprite door_number_sprite;
	
	
	
	
	QSize unzoomed_size_2d;
	
	
	//// The render texture
	//sf::RenderTexture canvas_render_texture;
	
	//unique_ptr<sf::Sprite> canvas_sprite;
	
	
	// This is mainly used to help with changing the scroll bars after
	// zooming.
	QScrollArea* scroll_area;
	
	
	// This is used, in combination with get_visible_rect(), to allow this
	// sfml_canvas_widget to draw only the parts of the sublevel that are
	// visible.
	sublevel* the_sublevel;
	
	// The raw textures of the level element selector widgets
	block_selector_core_widget* 
		the_block_selector_core_widget;
	sprite_16x16_selector_core_widget* 
		the_sprite_16x16_selector_core_widget;
	sprite_16x32_selector_core_widget* 
		the_sprite_16x32_selector_core_widget;
	
	//mouse_mode* the_mouse_mode;
	
public:		// variables and constants
	//static constexpr u32 minimum_scale_factor_for_block_grid = 2;
	static constexpr u32 minimum_scale_factor_for_block_grid = 1;
	static constexpr u32 num_pixels_per_block_row = 16,
		num_pixels_per_block_column = 16;
	
	////bool zoomed_in_recently, zoomed_out_recently;
	//bool modified_recently, zoomed_recently;
	bool modified_recently;
	bool zoomed_in_recently, zoomed_out_recently;
	
	u32 scale_factor;
	float view_center_x, view_center_y;
	
	
	
	
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size );
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, QScrollArea* s_scroll_area );
	
	
	
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	const sf::View& get_apparent_view();
	
	
	//inline bool point_is_in_render_texture( const sf::Vector2i& pos )
	//{
	//	return ( ( pos.x >= 0 ) 
	//		&& ( pos.x < (int)canvas_render_texture.getSize().x ) 
	//		&& ( pos.y >= 0 ) 
	//		&& ( pos.y < (int)canvas_render_texture.getSize().y ) );
	//}
	
	inline bool point_is_in_visible_rect( const sf::Vector2i& pos )
	{
		return get_visible_rect().contains( pos.x, pos.y );
	}
	
	//// This is a purely integer-based line drawing algorithm.
	//void draw_line( const sf::Vector2i& pos_0, 
	//	const sf::Vector2i& pos_1, const sf::Color& color );
	
	//void sfml_canvas_widget::draw_block_line
	//	( const sf::Vector2i& block_pos_0, const sf::Vector2i& block_pos_1,
	//	
	
	
	
	inline bool get_block_grid_enabled() const
	{
		return the_block_grid_stuff.enabled;
	}
	
	inline void enable_block_grid()
	{
		the_block_grid_stuff.enabled = true;
	}
	inline void disable_block_grid()
	{
		the_block_grid_stuff.enabled = false;
	}
	
	
	
	inline bool get_rect_selection_enabled() const
	{
		return the_rect_selection_stuff.enabled;
	}
	inline bool get_rect_selection_single_sprite_selected() const
	{
		return the_rect_selection_stuff.single_sprite_selected;
	}
	inline sprite_init_param_group_with_size*
		get_rect_selection_single_selected_sprite_ipgws()
	{
		return &( the_sublevel->sprite_ipgws_vec_2d
			.at(the_rect_selection_stuff.selection_rect.top)
			.at(the_rect_selection_stuff.selection_rect.left) );
	}
	inline const sf::IntRect& get_rect_selection_rect() const
	{
		return the_rect_selection_stuff.selection_rect;
	}
	
	inline const vec2_s32& get_rect_selection_starting_block_grid_coords()
		const
	{
		return the_rect_selection_stuff.starting_block_grid_coords;
	}
	
	inline bool get_rect_selection_moving() const
	{
		return the_rect_selection_stuff.moving;
	}
	
	//inline void enable_generic_rect_selection
	//	( const sf::IntRect& n_selection_rect )
	//{
	//	the_rect_selection_stuff.enabled = true;
	//	
	//	the_rect_selection_stuff.single_sprite_selected = false;
	//	
	//	the_rect_selection_stuff.selection_rect = n_selection_rect;
	//}
	
	void start_rect_selection
		( const vec2_s32& n_starting_block_grid_coords_of_mouse, 
		rect_selection_layer n_selection_layer );
	void continue_rect_selection
		( const vec2_s32& curr_block_grid_coords_of_mouse );
	void finish_rect_selection();
	
	// Stuff for moving the rectangular selection's contents.
	void start_moving_rect_selection_contents
		( const vec2_s32 n_clicked_location_in_rect );
	void continue_moving_rect_selection_contents
		( const vec2_s32 curr_block_grid_coords_of_mouse );
	void finish_moving_rect_selection_contents();
	
	
	
	inline void enable_single_sprite_rect_selection
		( sprite_init_param_group_with_size* n_selected_sprite_ipgws )
	{
		the_rect_selection_stuff.enabled = true;
		
		the_rect_selection_stuff.single_sprite_selected = true;
		the_rect_selection_stuff.selection_layer = rsl_sprites;
		
		the_rect_selection_stuff.selection_rect = sf::IntRect
			( n_selected_sprite_ipgws->initial_block_grid_x_coord,
			n_selected_sprite_ipgws->initial_block_grid_y_coord,
			n_selected_sprite_ipgws->size_2d.x 
			/ num_pixels_per_block_row, 
			n_selected_sprite_ipgws->size_2d.y 
			/ num_pixels_per_block_column );
		
		//vec2_s32 block_grid_start_pos
		//	( (s32)n_selected_sprite_ipgws.initial_block_grid_x_coord,  
		//	(s32)n_selected_sprite_ipgws.initial_block_grid_y_coord );
		//
		//vec2_s32 block_grid_end_pos = block_grid_start_pos
		//	+ vec2_s32( (s32)n_selected_sprite_ipgws.real_size_2d.x 
		//		* (s32)num_pixels_per_block_column,
		//		(s32)n_selected_sprite_ipgws.real_size_2d.y
		//		* (s32)num_pixels_per_block_row );
		//
		//if ( block_grid_end_pos.x >= (s32)the_sublevel->size_2d.x )
		//{
		//	block_grid_end_pos.x = (s32)( the_sublevel->size_2d.x - 1 );
		//}
		//
		//if ( block_grid_end_pos.y >= the_sublevel->size_2d.y )
		//{
		//	block_grid_end_pos.y = (s32)( the_sublevel->size_2d.y - 1 );
		//}
	}
	
	
	inline void disable_rect_selection()
	{
		the_rect_selection_stuff.enabled = false;
	}
	
	
	
	//// This is just a PNG exporter.
	//inline void export_file()
	//{
	//	export_file_as(image_file_name);
	//}
	//// This is just a PNG exporter.
	//void export_file_as( const string& output_file_name );
	
	inline void set_unzoomed_size_2d( QSize n_unzoomed_size_2d )
	{
		unzoomed_size_2d = n_unzoomed_size_2d;
		full_resize( QSize( unzoomed_size_2d.width() * scale_factor, 
			unzoomed_size_2d.height() * scale_factor ) );
	}
	
	inline void set_scroll_area( QScrollArea* n_scroll_area )
	{
		scroll_area = n_scroll_area;
	}
	inline void set_the_sublevel( sublevel* n_the_sublevel )
	{
		the_sublevel = n_the_sublevel;
	}
	void set_the_block_selector_core_widget
		( block_selector_core_widget* n_the_block_selector_core_widget );
	void set_the_sprite_16x16_selector_core_widget
		( sprite_16x16_selector_core_widget* 
		n_the_sprite_16x16_selector_core_widget );
	void set_the_sprite_16x32_selector_core_widget
		( sprite_16x32_selector_core_widget* 
		n_the_sprite_16x32_selector_core_widget );
	
	void set_the_mouse_mode
		( mouse_mode* n_the_mouse_mode );
	
	
	
	// This is used to determine which parts of the tilemap.
	inline sf::FloatRect get_visible_rect() const
	{
		QRect bounding_rect = visibleRegion().boundingRect();
		
		return sf::FloatRect( sf::Vector2f( (double)bounding_rect.x(), 
			(double)bounding_rect.y() ), 
			sf::Vector2f( (double)bounding_rect.width(), 
			(double)bounding_rect.height() ) );
	}
	
	
protected:		// functions
	//virtual inline void full_resize( const QSize& n_size )
	//{
	//	if ( scroll_area == NULL )
	//	{
	//		parent->resize(n_size);
	//		resize(n_size);
	//		sf::RenderWindow::create(winId());
	//	}
	//	else
	//	{
	//		QSize n_size_modded( n_size.width() + scroll_area
	//			->verticalScrollBar()->width(), n_size.height() 
	//			+ scroll_area->horizontalScrollBar()->height() );
	//		
	//		cout << n_size.width() << ", " << n_size.height() << ";  "
	//			<< n_size_modded.width() << ", " << n_size_modded.height()
	//			<< endl;
	//		parent->resize(n_size_modded);
	//		resize(n_size_modded);
	//		sf::RenderWindow::create(winId());
	//	}
	//	
	//	
	//}
	
	void generate_block_grid();
	void generate_rect_selection_rect();
	
	//void update_canvas_render_texture();
	void update_visible_area();
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//canvas_image->loadFromFile(string("the_powerup_gfx.png"));
		//open_image();
	}
	void on_update();
	
	
	
};



#endif		// sfml_canvas_widget_classes_hpp
