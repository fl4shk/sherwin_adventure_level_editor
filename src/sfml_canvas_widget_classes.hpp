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


class sfml_canvas_widget : public sfml_canvas_widget_base
{
protected:		// variables
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	sf::View apparent_view;
	
	sf::RenderTexture canvas_tile_grid_render_texture;
	
	unique_ptr<sf::Image> canvas_tile_grid_slot_image;
	unique_ptr<sf::Texture> canvas_tile_grid_slot_texture;
	unique_ptr<sf::Sprite> canvas_tile_grid_slot_sprite;
	
	bool tile_grid_enabled_recently, tile_grid_enabled;
	
	
	// Using "global" instances of canvas_image, canvas_texture,
	// and canvas_sprite appears to cause weirdness, so perhaps using
	// pointers for canvas_image, canvas_texture, and canvas_sprite would
	// be a good idea.  In such a case, the new and delete operators would
	// be used.
	// This weirdness was discovered while the pixel grid display and tile
	// grid display were being implemented.
	// The create() method appears to be the easiest way to implement
	// image resizing functionality.
	unique_ptr<sf::Image> canvas_image;
	unique_ptr<sf::Texture> canvas_texture;
	unique_ptr<sf::Sprite> canvas_sprite;
	
	// This is for line drawing.
	QPoint prev_mouse_pos;
	
	
public:		// variables and constants
	static constexpr u32 minimum_scale_factor_for_tile_grid = 2;
	static constexpr u32 num_pixels_per_tile_row = 8,
		num_pixels_per_tile_column = 8, num_pixels_per_block_row = 16,
		num_pixels_per_block_colum = 16;
	
	//bool zoomed_in_recently, zoomed_out_recently;
	bool modified_recently, zoomed_recently;
	u32 scale_factor;
	float view_center_x, view_center_y;
	
	
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size );
	
	
	//inline void full_resize( const QSize& n_size )
	//{
	//	resize(n_size);
	//	sf::RenderWindow::create(winId());
	//}
	
	
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	const sf::View& get_apparent_view();
	
	inline bool point_is_in_image( const sf::Vector2i& pos )
	{
		return ( ( pos.x >= 0 ) 
			&& ( pos.x < (int)canvas_image->getSize().x ) && ( pos.y >= 0 ) 
			&& ( pos.y < (int)canvas_image->getSize().y ) );
	}
	
	
	inline bool get_tile_grid_enabled() const
	{
		return tile_grid_enabled;
	}
	
	void enable_tile_grid();
	void disable_tile_grid();
	
	
	void export_file_as( const string& output_file_name );
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	
	void generate_canvas_tile_grid();
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//canvas_image->loadFromFile(string("the_powerup_gfx.png"));
		//open_image();
	}
	void on_update();
	
	
};


#endif		// sfml_canvas_widget_classes_hpp
