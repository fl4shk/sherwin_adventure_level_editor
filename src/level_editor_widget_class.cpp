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
	
	init_level_element_selectors_tab_widget();
	
	
	core_widgets_tab_widget.reset(new QTabWidget(this));
	core_widgets_tab_widget->setMovable(true);
	
	connect( core_widgets_tab_widget.get(), &QTabWidget::currentChanged,
		this, &level_editor_widget
		::modify_sprite_properties_widget_at_tab_switch );
	
	// If no file name was passed on the command line, create a new
	// sublevel, with real_size_2d set to vec2_u32( 16, 16 )
	if ( argv_copy->size() == 1 )
	{
		the_level.sublevel_vec.clear();
		the_level.sublevel_vec.push_back(sublevel(vec2_u32( 16, 16 )));
		
		//level_editor_core_widget* to_push = new level_editor_core_widget
		//	( this, QPoint( 0, 0 ), string(""), 
		//	&the_level.sublevel_vec.front(), vec2_u32( 16, 16 ) );
		//level_editor_core_widget* to_push = new level_editor_core_widget
		//	( this, QPoint( 0, 0 ), string(""), 
		//	&the_level.sublevel_vec.front() );
		
		the_core_widget_vec.push_back(unique_ptr<level_editor_core_widget>
			(new level_editor_core_widget( this, QPoint( 0, 0 ), 
			string(""), &the_level.sublevel_vec.front() )));
		
		
		connect( the_core_widget_vec.back().get(),
			&level_editor_core_widget::sprite_was_selected, this,
			&level_editor_widget::show_sprite_properties_widget );
		
		connect( the_core_widget_vec.back().get(),
			&level_editor_core_widget::sprite_no_longer_selected, this,
			&level_editor_widget::hide_sprite_properties_widget );
		
		
		// core_widget_scroll_area stuff
		core_widget_scroll_area_vec.push_back(unique_ptr<QScrollArea>
			(new QScrollArea(this)));
		core_widget_scroll_area_vec.back()->setWidget
			(the_core_widget_vec.back().get());
		core_widget_scroll_area_vec.back()->setHorizontalScrollBarPolicy
			(Qt::ScrollBarAlwaysOn);
		core_widget_scroll_area_vec.back()->setVerticalScrollBarPolicy
			(Qt::ScrollBarAlwaysOn);
		
		
		the_core_widget_vec.back()->the_sfml_canvas_widget
			->set_scroll_area(core_widget_scroll_area_vec.front().get());
		
		
		core_widgets_tab_widget->addTab
			( core_widget_scroll_area_vec.front().get(), "Sublevel 0" );
		
		sprite_properties_widget_enabled_vec.push_back(false);
	}
	else //if ( argv_copy->size() == 3 )
	{
		open_level_core_func(argv_copy->at(1));
	}
	
	init_tab_stuff_for_core_widgets();
	
	
	init_splitters_and_hbox_layout();
	
}



void level_editor_widget::init_level_element_selectors_tab_widget()
{
	#define X(name) \
	the_##name##_selector_widget.reset(new name##_selector_widget( this, \
		QPoint( 0, 0 ), QSize( 64, 64 ) ));
	
	list_of_level_element_widget_name_prefixes(X);
	#undef X
	
	// level_element_selectors_tab_widget stuff
	level_element_selectors_tab_widget.reset(new QTabWidget(this));
	level_element_selectors_tab_widget->setMovable(true);
	level_element_selectors_tab_widget->addTab
		( the_block_selector_widget.get(), "Blocks" );
	level_element_selectors_tab_widget->addTab
		( the_sprite_16x16_selector_widget.get(), "16x16 Sprites" );
	level_element_selectors_tab_widget->addTab
		( the_sprite_16x32_selector_widget.get(), "16x32 Sprites" );
	
	
	//the_core_widget->init_tab_stuff
	//	( level_element_selectors_tab_widget, the_block_selector_widget, 
	//	the_sprite_16x16_selector_widget,
	//	the_sprite_16x32_selector_widget );
	
}

void level_editor_widget::init_tab_stuff_for_core_widgets()
{
	for ( auto& core_widget : the_core_widget_vec )
	{
		core_widget->init_tab_stuff
			( level_element_selectors_tab_widget.get(), 
			the_block_selector_widget.get(), 
			the_sprite_16x16_selector_widget.get(),
			the_sprite_16x32_selector_widget.get() );
	}
}


void level_editor_widget::init_splitters_and_hbox_layout()
{
	// splitter stuff
	horiz_splitter.reset(new QSplitter(this));
	
	horiz_splitter->setSizePolicy( QSizePolicy::Ignored, 
		QSizePolicy::Ignored );
	
	
	vert_splitter.reset(new QSplitter(this));
	vert_splitter->addWidget(level_element_selectors_tab_widget.get());
	vert_splitter->setOrientation(Qt::Vertical);
	
	
	//horiz_splitter->addWidget(core_widget_scroll_area);
	horiz_splitter->addWidget(core_widgets_tab_widget.get());
	horiz_splitter->addWidget(vert_splitter.get());
	
	
	hbox_layout.reset(new QHBoxLayout);
	setLayout(hbox_layout.get());
	hbox_layout->addWidget(horiz_splitter.get());
	
	
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
	
	level_editor_core_widget* the_core_widget = NULL;
	sfml_canvas_widget* the_sfml_canvas_widget = NULL;
	
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index != -1 )
	{
		the_core_widget = the_core_widget_vec.at(curr_tab_index).get();
		the_sfml_canvas_widget = the_core_widget
			->the_sfml_canvas_widget.get();
	}
	
	if ( the_core_widget == NULL )
	{
		cout << "Weird bug in keyPressEvent():  the_core_widget == "
			<< "NULL.\nExpect a segfault....";
	}
	
	
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
			>= (s32)the_core_widget->the_sublevel->real_size_2d.x )
		{
			visible_block_grid_size_2d.x 
				= the_core_widget->the_sublevel->real_size_2d.x
				- visible_block_grid_start_pos.x;
		}
		if ( ( visible_block_grid_start_pos.y 
			+ visible_block_grid_size_2d.y )
			>= (s32)the_core_widget->the_sublevel->real_size_2d.y )
		{
			visible_block_grid_size_2d.y 
				= the_core_widget->the_sublevel->real_size_2d.y
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


bool level_editor_widget::open_level_core_func
	( const string& n_level_file_name )
{
	fstream level_file( n_level_file_name, ios_base::in );
	
	if ( !level_file.is_open() )
	{
		cout << "Unable to open " << n_level_file_name 
			<< " for reading.\n";
		return false;
	}
	
	level_file.close();
	
	//if ( the_core_widget != NULL )
	//{
	//	the_core_widget->level_file_name = n_level_file_name;
	//}
	
	core_widgets_tab_widget->clear();
	the_level.sublevel_vec.clear();
	
	sprite_properties_widget_enabled_vec.clear();
	
	
	
	if ( !the_core_widget_vec.empty() )
	{
		//for ( auto& core_widget : the_core_widget_vec )
		//{
		//	//delete core_widget;
		//	core_widget.reset(NULL);
		//}
		the_core_widget_vec.clear();
	}
	
	
	if ( !core_widget_scroll_area_vec.empty() )
	{
		//for ( auto& scroll_area : core_widget_scroll_area_vec )
		//{
		//	//delete scroll_area;
		//	scroll_area.reset(NULL);
		//}
		core_widget_scroll_area_vec.clear();
	}
	
	//the_core_widget_vec.clear();
	
	
	
	level temp_level;
	
	xml_document doc;
	
	xml_parse_result result = doc.load_file(n_level_file_name.c_str());
	
	
	
	auto parse_sublevel_block_data_node 
		= [&]( xml_node& sublevel_block_data_node, u32 sublevel_index )
	{
		sublevel& the_sublevel 
			= temp_level.sublevel_vec.at(sublevel_index);
		
		string block_data_str = sublevel_block_data_node.text().get();
		
		//cout << ( block_data_str.at(0) == '\n' ) << endl;
		
		// Get rid of the initial "\n".
		block_data_str = block_data_str.substr(1);
		
		stringstream block_data_sstm(block_data_str);
		
		string line;
		
		u32 j = 0; 
		
		while ( getline( block_data_sstm, line, '\n' ) )
		{
			if ( j >= the_sublevel.real_size_2d.y )
			{
				continue;
			}
			
			u32 i = 0;
			
			stringstream line_sstm(line);
			
			string block_type_str;
			
			//// This is a vector for ONE ROW of block_type's.
			//vector<block_type> block_type_row;
			
			while ( getline( line_sstm, block_type_str, ',' ) )
			{
				if ( i >= the_sublevel.real_size_2d.x )
				{
					continue;
				}
				
				stringstream block_type_sstm;
				
				block_type_sstm << block_type_str;
				block_type_sstm >> the_sublevel
					.uncompressed_block_data_vec_2d.at(j).at(i).type;
				
				++i;
			}
			
			++j;
		}
		
		
	};
	
	auto parse_sublevel_sprites_node 
		= [&]( xml_node& sublevel_sprites_node, u32 sublevel_index )
	{
		sublevel& the_sublevel 
			= temp_level.sublevel_vec.at(sublevel_index);
		
		the_sublevel.sprite_ipgws_vec_for_xml.clear();
		
		for ( xml_node sprite_node=sublevel_sprites_node.first_child();
			sprite_node;
			sprite_node=sprite_node.next_sibling() )
		{
			sprite_init_param_group_with_size to_push;
			
			for ( xml_attribute attr=sprite_node.first_attribute();
				attr;
				attr=attr.next_attribute() )
			{
				if ( attr.name() == string("type") )
				{
					to_push.type = static_cast<sprite_type>
						(attr.as_uint());
				}
				else if ( attr.name() == string("ibg_x_coord") )
				{
					to_push.initial_block_grid_x_coord = attr.as_uint();
				}
				else if ( attr.name() == string("ibg_y_coord") )
				{
					to_push.initial_block_grid_y_coord = attr.as_uint();
				}
				else if ( attr.name() == string("facing_right") )
				{
					to_push.facing_right = attr.as_bool();
				}
				else if ( attr.name() == string("ep0") )
				{
					to_push.extra_param_0 = attr.as_uint();
				}
				else if ( attr.name() == string("ep1") )
				{
					to_push.extra_param_1 = attr.as_uint();
				}
				else if ( attr.name() == string("ep2") )
				{
					to_push.extra_param_2 = attr.as_uint();
				}
				else if ( attr.name() == string("ep3") )
				{
					to_push.extra_param_3 = attr.as_uint();
				}
				else if ( attr.name() == string("width") )
				{
					to_push.size_2d.x = attr.as_uint();
				}
				else if ( attr.name() == string("height") )
				{
					to_push.size_2d.y = attr.as_uint();
				}
			}
			
			the_sublevel.sprite_ipgws_vec_for_xml.push_back(to_push);
		}
		
		the_sublevel.sprite_ipgws_vec_2d.clear();
		
		for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
		{
			the_sublevel.sprite_ipgws_vec_2d.push_back
				(vector<sprite_init_param_group_with_size>());
			for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
			{
				the_sublevel.sprite_ipgws_vec_2d.at(j).push_back
					(sprite_init_param_group_with_size());
			}
		}
		
		
		for ( sprite_init_param_group_with_size& the_sprite_ipgws
			: the_sublevel.sprite_ipgws_vec_for_xml )
		{
			the_sublevel.sprite_ipgws_vec_2d
				.at(the_sprite_ipgws.initial_block_grid_y_coord)
				.at(the_sprite_ipgws.initial_block_grid_x_coord)
				= the_sprite_ipgws;
		}
		
		
	};
	
	auto parse_sublevel_node = [&]( xml_node& sublevel_node, 
		u32 sublevel_index ) -> void
	{
		for ( xml_attribute attr=sublevel_node.first_attribute();
			attr;
			attr=attr.next_attribute() )
		{
			if ( attr.name() == string("width") )
			{
				temp_level.sublevel_vec.at(sublevel_index).real_size_2d.x 
					= attr.as_uint();
			}
			else if ( attr.name() == string("height") )
			{
				temp_level.sublevel_vec.at(sublevel_index).real_size_2d.y 
					= attr.as_uint();
			}
		}
		
		for ( xml_node node=sublevel_node.first_child();
			node;
			node=node.next_sibling() )
		{
			if ( node.name() == string("block_data") )
			{
				parse_sublevel_block_data_node( node, sublevel_index );
			}
			else if ( node.name() == string("sprites") )
			{
				parse_sublevel_sprites_node( node, sublevel_index );
			}
		}
		
		
		the_level.sublevel_vec.push_back
			(temp_level.sublevel_vec.at(sublevel_index));
		
		the_core_widget_vec.push_back(unique_ptr<level_editor_core_widget>
			(new level_editor_core_widget( this, QPoint( 0, 0 ), 
			n_level_file_name, &the_level.sublevel_vec.back() )));
		
		
		connect( the_core_widget_vec.back().get(),
			&level_editor_core_widget::sprite_was_selected, this,
			&level_editor_widget::show_sprite_properties_widget );
		
		connect( the_core_widget_vec.back().get(),
			&level_editor_core_widget::sprite_no_longer_selected, this,
			&level_editor_widget::hide_sprite_properties_widget );
		
		
		// core_widget_scroll_area stuff
		core_widget_scroll_area_vec.push_back(unique_ptr<QScrollArea>
			(new QScrollArea(this)));
		core_widget_scroll_area_vec.back()->setWidget
			(the_core_widget_vec.back().get());
		core_widget_scroll_area_vec.back()->setHorizontalScrollBarPolicy
			(Qt::ScrollBarAlwaysOn);
		core_widget_scroll_area_vec.back()->setVerticalScrollBarPolicy
			(Qt::ScrollBarAlwaysOn);
		
		
		the_core_widget_vec.back()->the_sfml_canvas_widget
			->set_scroll_area(core_widget_scroll_area_vec.back().get());
		
		
		string sublevel_index_str;
		stringstream sublevel_index_sstm;
		
		sublevel_index_sstm << sublevel_index;
		sublevel_index_sstm >> sublevel_index_str;
		
		core_widgets_tab_widget->addTab
			( core_widget_scroll_area_vec.back().get(), 
			( string("Sublevel ") + sublevel_index_str ).c_str() );
		
	};
	
	
	//vec2_u32 real_size_2d_before_parse = the_sublevel.real_size_2d;
	
	u32 real_num_sublevels = 0;
	
	for ( xml_node node=doc.first_child().first_child();
		node;
		node=node.next_sibling() )
	{
		//cout << node.name() << " " << node.value() << endl;
		string node_name_str(node.name());
		
		const string sublevel_word_str("sublevel");
		
		if ( node_name_str.substr( 0, sublevel_word_str.size() ) 
			== sublevel_word_str )
		{
			parse_sublevel_node( node, real_num_sublevels++ );
		}
		
		if ( real_num_sublevels > level::max_num_sublevels )
		{
			cout << "Note:  There are too many sublevels in the level.  "
				<< "The maximum number allowed is " 
				<< level::max_num_sublevels << ".\n";
			break;
		}
	}
	
	
	for ( u32 i=0; i<real_num_sublevels; ++i )
	{
		sprite_properties_widget_enabled_vec.push_back(false);
	}
	
	
	init_tab_stuff_for_core_widgets();
	
	
	return true;
}

// This is semi-temporary until there are multiple levels.  By the way,
// eventually it should be made possible to change the width and height of
// a sublevel.
void level_editor_widget::save_level_as_core_func
	( const string& output_file_name )
{
	//sublevel& the_sublevel = the_level.get_curr_sublevel();
	
	
	xml_document doc;
	
	//cout << the_level.sublevel_vec.size() << endl;
	xml_node level_node = doc.append_child("level");
	
	for ( u32 i=0; i<the_level.sublevel_vec.size(); ++i )
	{
		sublevel& the_sublevel = the_level.sublevel_vec.at(i);
		
		xml_node sublevel_node = level_node.append_child("sublevel");
		
		// Width of the sublevel
		xml_attribute sublevel_width_attr = sublevel_node.append_attribute
			("width");
		sublevel_width_attr.set_value(the_sublevel.real_size_2d.x);
		
		// Height of the sublevel
		xml_attribute sublevel_height_attr = sublevel_node.append_attribute
			("height");
		sublevel_height_attr.set_value(the_sublevel.real_size_2d.y);
		
		
		// This only stores block_type's, not persistent_data_index's.  The
		// export_source_core_func() and export_source_as_core_func() store
		// compressed block data, with persistent_data_index included.
		xml_node uncompressed_block_data_node = sublevel_node.append_child
			("block_data");
		
		string uncompressed_block_data_str = "";
		
		the_sublevel.sprite_ipgws_vec_for_xml.clear();
		
		for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
		{
			uncompressed_block_data_str += "\n\t\t\t";
			for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
			{
				block& the_block = the_sublevel
					.uncompressed_block_data_vec_2d.at(j).at(i);
				stringstream block_type_sstm;
				string number_str;
				
				block_type_sstm << (u32)the_block.type;
				block_type_sstm >> number_str;
				
				uncompressed_block_data_str += number_str;
				uncompressed_block_data_str += ",";
				
				
				// Also build sprite_ipg_vec.
				sprite_init_param_group_with_size& the_sprite_ipgws
					= the_sublevel.sprite_ipgws_vec_2d.at(j).at(i);
				
				if ( the_sprite_ipgws.type != st_default )
				{
					the_sublevel.sprite_ipgws_vec_for_xml.push_back
						(the_sprite_ipgws);
				}
			}
		}
		uncompressed_block_data_str += "\n\t\t";
		
		uncompressed_block_data_node.text().set
			(uncompressed_block_data_str.c_str());
		
		
		xml_node sprites_parent_node = sublevel_node.append_child
			("sprites");
		
		//for ( sprite_init_param_group_with_size& the_sprite_ipgws
		//	: the_sublevel.sprite_ipgws_vec_for_xml )
		for ( u32 i=0; 
			i<the_sublevel.sprite_ipgws_vec_for_xml.size(); 
			++i )
		{
			sprite_init_param_group_with_size& the_sprite_ipgws
				= the_sublevel.sprite_ipgws_vec_for_xml.at(i);
			
			stringstream number_sstm;
			
			// A string for interfacing with the_sstm.
			string number_str;
			
			number_sstm << i;
			number_sstm >> number_str;
			
			xml_node child_node = sprites_parent_node.append_child
				("sprite");
			
			// The type of sprite
			xml_attribute type_attr = child_node.append_attribute("type");
			type_attr.set_value(the_sprite_ipgws.type);
			
			
			// The initial in-level x coordinate, divided by 16
			xml_attribute ibg_x_coord_attr = child_node.append_attribute
				("ibg_x_coord");
			ibg_x_coord_attr.set_value
				(the_sprite_ipgws.initial_block_grid_x_coord);
			
			// The initial in-level y coordinate, divided by 16
			xml_attribute ibg_y_coord_attr = child_node.append_attribute
				("ibg_y_coord");
			ibg_y_coord_attr.set_value
				(the_sprite_ipgws.initial_block_grid_y_coord);
			
			// Here is a s
			xml_attribute facing_right_attr = child_node.append_attribute
				("facing_right");
			facing_right_attr.set_value(the_sprite_ipgws.facing_right);
			
			// Here are extra parameters that are used in different ways
			// depending on the type of sprite.
			xml_attribute ep0_attr = child_node.append_attribute("ep0");
			ep0_attr.set_value(the_sprite_ipgws.extra_param_0);
			
			xml_attribute ep1_attr = child_node.append_attribute("ep1");
			ep1_attr.set_value(the_sprite_ipgws.extra_param_1);
			
			xml_attribute ep2_attr = child_node.append_attribute("ep2");
			ep2_attr.set_value(the_sprite_ipgws.extra_param_2);
			
			xml_attribute ep3_attr = child_node.append_attribute("ep3");
			ep3_attr.set_value(the_sprite_ipgws.extra_param_3);
			
			// There's no need to store the_sprite_ipgws.spawn_state within
			// the XML file.
			
			
			// The size of the sprite within the non-source code level
			// data.
			xml_attribute sprite_width_attr = child_node.append_attribute
				("width");
			sprite_width_attr.set_value(the_sprite_ipgws.size_2d.x);
			
			
			xml_attribute sprite_height_attr = child_node.append_attribute
				("height");
			sprite_height_attr.set_value(the_sprite_ipgws.size_2d.y);
		}
	}
	
	
	//doc.save_file( (output_file_name + ".xml").c_str() );
	doc.save_file(output_file_name.c_str());
	
	
}

void level_editor_widget::export_source_as_core_func
	( const string& output_file_name )
{
	
}



void level_editor_widget::show_sprite_properties_widget()
{
	//sprite_properties_widget_enabled = true;
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	u32& sprite_properties_widget_enabled 
		= sprite_properties_widget_enabled_vec.at(curr_tab_index);
	
	sprite_properties_widget_enabled = true;
	
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget_vec
		.at(curr_tab_index)->the_sfml_canvas_widget
		->the_rect_selection_stuff;
	
	the_sprite_properties_widget.reset(new sprite_properties_widget( this,
		the_rect_selection_stuff.get_single_selected_sprite_ipgws() ));
	
	
	vert_splitter->addWidget(the_sprite_properties_widget.get());
}

void level_editor_widget::hide_sprite_properties_widget()
{
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	u32& sprite_properties_widget_enabled 
		= sprite_properties_widget_enabled_vec.at(curr_tab_index);
	
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

void level_editor_widget::modify_sprite_properties_widget_at_tab_switch
	( int n_index )
{
	//cout << "New tab index:  " << n_index << endl;
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index == -1 )
	{
		return;
	}
	
	if ( sprite_properties_widget_enabled_vec.empty() )
	{
		return;
	}
	
	
	// Hide the_sprite_properties_widget if it has been allocated for a
	// different sublevel/tab.
	if ( the_sprite_properties_widget.get() != NULL )
	{
		the_sprite_properties_widget->hide();
		
		the_sprite_properties_widget.reset(NULL);
	}
	
	
	u32& sprite_properties_widget_enabled 
		= sprite_properties_widget_enabled_vec.at(curr_tab_index);
	
	// If the_sprite_properties_widget was previously enabled for the tab
	// to which we are switching, then re-generate it.
	if (sprite_properties_widget_enabled)
	{
		rect_selection_stuff& the_rect_selection_stuff 
			= the_core_widget_vec.at(curr_tab_index)
			->the_sfml_canvas_widget->the_rect_selection_stuff;
		
		the_sprite_properties_widget.reset(new sprite_properties_widget
			( this, the_rect_selection_stuff
			.get_single_selected_sprite_ipgws() ));
		
		vert_splitter->addWidget(the_sprite_properties_widget.get());
	}
	
}


void level_editor_widget::hello()
{
	cout << "Hello!\n";
}


void level_editor_widget::show_horizontal_scroll_bar_stuff()
{
	QScrollArea* core_widget_scroll_area = NULL;
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index != -1 )
	{
		core_widget_scroll_area = core_widget_scroll_area_vec
			.at(curr_tab_index).get();
	}
	
	
	if ( core_widget_scroll_area == NULL )
	{
		cout << "Weird bug in show_horizontal_scroll_bar_stuff().\n";
	}
	
	cout << core_widget_scroll_area->horizontalScrollBar()->value() << " " 
		<< core_widget_scroll_area->horizontalScrollBar()->minimum() << " "
		<< core_widget_scroll_area->horizontalScrollBar()->maximum() 
		<< endl;
}

void level_editor_widget::show_vertical_scroll_bar_stuff()
{
	QScrollArea* core_widget_scroll_area = NULL;
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index != -1 )
	{
		core_widget_scroll_area = core_widget_scroll_area_vec
			.at(curr_tab_index).get();
	}
	
	if ( core_widget_scroll_area == NULL )
	{
		cout << "Weird bug in show_vertical_scroll_bar_stuff().\n";
	}
	
	cout << core_widget_scroll_area->verticalScrollBar()->value() << " "
		<< core_widget_scroll_area->verticalScrollBar()->minimum() << " "
		<< core_widget_scroll_area->verticalScrollBar()->maximum() << endl;
}

void level_editor_widget::show_geometry_stuff()
{
	QScrollArea* core_widget_scroll_area = NULL;
	
	s32 curr_tab_index = get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index != -1 )
	{
		core_widget_scroll_area = core_widget_scroll_area_vec
			.at(curr_tab_index).get();
	}
	
	if ( core_widget_scroll_area == NULL )
	{
		cout << "Weird bug in show_geometry_stuff().\n";
	}
	
	cout << core_widget_scroll_area->geometry().x() << " " 
		<< core_widget_scroll_area->geometry().y() << endl;
	cout << core_widget_scroll_area->frameGeometry().x() << " " 
		<< core_widget_scroll_area->frameGeometry().y() << endl;
}


void level_editor_widget::open_level()
{
	QString input_file_name = QFileDialog::getOpenFileName( this, 
		tr("Open Level"), QString(), tr("Level File (*.xml)") );
	//cout << "This QString was obtained:  " 
	//	<< input_file_name.toStdString() << endl;
	
	//cout << ( input_file_name.toStdString().size() == 0 ) << endl;
	if ( input_file_name.toStdString().size() == 0 )
	{
		return;
	}
	
	//for ( auto& core_widget : the_core_widget_vec )
	//{
	//	core_widget->level_file_name = input_file_name.toStdString();
	//}
	
	open_level_core_func(input_file_name.toStdString());
}

void level_editor_widget::save_level()
{
	if ( the_core_widget_vec.front()->level_file_name == string("") )
	{
		QString output_file_name = QFileDialog::getSaveFileName( this, 
			tr("Save Level"), QString(), tr("Level File (*.xml)") );
		//cout << "This QString was obtained:  " 
		//	<< output_file_name.toStdString() << endl;
		
		//cout << ( output_file_name.toStdString().size() == 0 ) << endl;
		if ( output_file_name.toStdString().size() == 0 )
		{
			return;
		}
		
		for ( auto& core_widget : the_core_widget_vec )
		{
			core_widget->level_file_name = output_file_name.toStdString();
		}
		
	}
	
	cout << "Saving....\n";
	
	save_level_core_func();
}

void level_editor_widget::save_level_as()
{
	QString output_file_name = QFileDialog::getSaveFileName( this, 
		tr("Save Level"), QString(), tr("Level File (*.xml)") );
	//cout << "This QString was obtained:  " 
	//	<< output_file_name.toStdString() << endl;
	
	if ( output_file_name.toStdString().size() == 0 )
	{
		return;
	}
	
	cout << "Saving....\n";
	
	save_level_as_core_func(output_file_name.toStdString());
}


void level_editor_widget::export_source()
{
	
}

void level_editor_widget::export_source_as()
{
	
}



