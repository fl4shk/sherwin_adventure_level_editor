#ifndef level_editor_widget_class_hpp
#define level_editor_widget_class_hpp

#include "misc_includes.hpp"
#include "sfml_canvas_widget_classes.hpp"

class level_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	vector<string>* argv_copy;
	
	static const QString default_parent_title;
	
	QWidget* parent;
	
	QHBoxLayout* hbox_layout;
	//QVBoxLayout* vbox_layout;
	
	QScrollArea* scroll_area;
	
	sfml_canvas_widget* the_sfml_canvas_widget; 
	
	
public:		// functions
	level_editor_widget( vector<string>* s_argv_copy, 
		QWidget* s_parent = 0 );
	
	
protected:		// functions
	//void adjust_scroll_bar( QScrollBar* scroll_bar );
	bool zoom_in();
	bool zoom_out();
	
	// Events
	void keyPressEvent( QKeyEvent* event );
	//void mousePressEvent( QMouseEvent* event );
	//void mouseMoveEvent( QMouseEvent* event );
	////void mouseReleaseEvent( QMouseEvent* event );
	//void paintEvent( QPaintEvent* event );
	////void resizeEvent( QResizeEvent* event );
	
	//inline const sf::Vector2i widget_pos_to_scroll_area_coords
	//	( int widget_x, int widget_y ) const
	//{
	//	return sf::Vector2i( widget_x - scroll_area->geometry().x()
	//		+ scroll_area->horizontalScrollBar()->value(),
	//		widget_y - scroll_area->geometry().y()
	//		+ scroll_area->verticalScrollBar()->value() );
	//}
	
	
	
protected slots:		// slots.
	void hello();
	void show_horizontal_scroll_bar_stuff();
	void show_vertical_scroll_bar_stuff();
	void show_geometry_stuff();
	
	
public slots:		// slots
	void save_file();
	void save_file_as();
	
};


#endif		// level_editor_widget_class_hpp
