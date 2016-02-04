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
	inline void full_resize( const QSize& n_size )
	{
		parent->resize(n_size);
		resize(n_size);
		sf::RenderWindow::create(winId());
		//setVerticalSyncEnabled(true);
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

class sfml_canvas_widget : public sfml_canvas_widget_base
{
protected:		// variables
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	sf::View apparent_view;
	
	sf::RenderTexture canvas_block_grid_render_texture;
	
	unique_ptr<sf::Image> canvas_block_grid_slot_image;
	unique_ptr<sf::Texture> canvas_block_grid_slot_texture;
	unique_ptr<sf::Sprite> canvas_block_grid_slot_sprite;
	
	bool block_grid_enabled_recently, block_grid_enabled;
	
	
	
	unique_ptr<sf::Sprite> canvas_sprite;
	
	
	// This is mainly used to help with changing the scroll bars after
	// zooming.
	QScrollArea* scroll_area;
	
	
	
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
	
	
	sf::RenderTexture canvas_render_texture;
	
	
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size );
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, QScrollArea* s_scroll_area );
	
	
	
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	const sf::View& get_apparent_view();
	
	
	inline bool point_is_in_render_texture( const sf::Vector2i& pos )
	{
		return ( ( pos.x >= 0 ) 
			&& ( pos.x < (int)canvas_render_texture.getSize().x ) 
			&& ( pos.y >= 0 ) 
			&& ( pos.y < (int)canvas_render_texture.getSize().y ) );
	}
	
	//// This is a purely integer-based line drawing algorithm.
	//void draw_line( const sf::Vector2i& pos_0, 
	//	const sf::Vector2i& pos_1, const sf::Color& color );
	
	//void sfml_canvas_widget::draw_block_line
	//	( const sf::Vector2i& block_pos_0, const sf::Vector2i& block_pos_1,
	//	
	
	
	
	inline bool get_block_grid_enabled() const
	{
		return block_grid_enabled;
	}
	
	void enable_block_grid();
	void disable_block_grid();
	
	
	//// This is just a PNG exporter.
	//inline void export_file()
	//{
	//	export_file_as(image_file_name);
	//}
	//// This is just a PNG exporter.
	//void export_file_as( const string& output_file_name );
	
	inline void set_scroll_area( QScrollArea* n_scroll_area )
	{
		scroll_area = n_scroll_area;
	}
	
	
protected:		// functions
	//void mousePressEvent( QMouseEvent* event );
	//void mouseMoveEvent( QMouseEvent* event );
	//void mouseReleaseEvent( QMouseEvent* event );
	
	void generate_canvas_block_grid();
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//canvas_image->loadFromFile(string("the_powerup_gfx.png"));
		//open_image();
	}
	void on_update();
	
	
	friend class block_selector_core_widget;
};



#endif		// sfml_canvas_widget_classes_hpp
