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



#include "primary_widget_class.hpp"


// I really like that this works
#define single_toolbar_icon_file_name(suffix) \
	QString("icons/" #suffix "_icon_32x32.png"),

const vector<QString> primary_widget::toolbar_icon_file_name_vec
	= { list_of_tbt_suffixes(single_toolbar_icon_file_name) };

#undef single_toolbar_icon_file_name


// This needs to be updated manually if more toolbar buttons are added!
const vector<QString> primary_widget::toolbar_action_text_vec
	= { "Laugh", "Draw", "Erase (Single Layer)", "Sprite Properties", 
	"Rect Selection (Single Layer)" };

// This ALSO needs to be updated manually if more toolbar buttons are
// added!
const vector<QKeySequence> primary_widget::tool_button_shortcut_vec 
	= { QKeySequence(Qt::Key_Q), QKeySequence(Qt::Key_W), 
	QKeySequence(Qt::Key_E), QKeySequence(Qt::Key_R) };


primary_widget::primary_widget( vector<string>& s_argv_copy, 
	QWidget* parent ) : QMainWindow(parent), argv_copy(s_argv_copy)
{
	bool generate_toolbar_result = generate_toolbar();
	
	cout << "Here's the result of generate_toolbar():  " 
		<< generate_toolbar_result << endl;
	
	if ( !generate_toolbar_result )
	{
		quit_non_slot();
	}
	
	the_editing_manager.reset(new editing_manager());
	
	generate_central_widget();
	
	generate_menus();
	
	//connect( the_mouse_mode_button_group_widget->button_group.get(),
	//	static_cast<void (QButtonGroup::*)(int)>
	//	(&QButtonGroup::buttonClicked), the_central_widget.get(),
	//	&level_editor_widget::switch_mouse_mode );
	//connect( the_mouse_mode_button_group_widget->button_group.get(),
	//	(void (QButtonGroup::*)(int))(&QButtonGroup::buttonClicked), 
	//	the_central_widget.get(), &level_editor_widget::switch_mouse_mode);
	bool connect_result = connect
		( the_mouse_mode_button_group_widget->button_group.get(),
		(void (QButtonGroup::*)(int))(&QButtonGroup::buttonClicked), 
		the_central_widget.get(), 
		&level_editor_widget::switch_mouse_mode );
	
	cout << "Here's the result of connect() from primary_widget"
		<< "::generate_toolbar():  " << connect_result << endl;
	
	
	
	//if ( !generate_toolbar() )
	//{
	//	quit_non_slot();
	//}
	
	
	
	//if ( !the_central_widget->open_image
	//	(QString("background_tiles_modded.png")) )
	//{
	//	cout << "Error opening background_tiles_modded.png!\n";
	//	quit_non_slot();
	//}
	
	//quit_non_slot();
}

void primary_widget::generate_menus()
{
	// Generate the file_menu actions.
	file_menu_laugh_action.reset(new QAction( "&Laugh", this ));
	file_menu_open_action.reset(new QAction( "&Open", this ));
	file_menu_save_action.reset(new QAction( "&Save", this ));
	file_menu_save_as_action.reset(new QAction( "&Save As", this ));
	file_menu_quit_action.reset(new QAction( "&Quit", this ));
	
	// Generate the edit_menu actions.
	edit_menu_copy_rs_contents_action.reset(new QAction
		( "&Copy Selection (Keyboard Shortcut:  C)", this ));
	edit_menu_paste_copied_rs_contents_action.reset(new QAction
		( "&Paste Copied Selection (Keyboard Shortcut:  V)", this ));
	edit_menu_undo_action.reset(new QAction
		( "&Undo (Keyboard Shortcut:  Z)", this ));
	edit_menu_redo_action.reset(new QAction
		( "&Redo (Keyboard Shortcut:  Y)", this ));
	
	
	// Generate the level menu actions.
	level_menu_sublevel_properties_action.reset(new QAction
		( "&Sublevel Properties", this ));
	//level_menu_level_properties_action.reset(new QAction
	//	( "&Level Properties", this ));
	level_menu_export_action.reset(new QAction( "&Export Level", this ));
	
	
	
	
	// Connect the file_menu actions to the slots.
	connect( file_menu_laugh_action.get(), &QAction::triggered, this, 
		&primary_widget::file_menu_laugh );
	connect( file_menu_open_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::open_level );
	connect( file_menu_save_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::save_level );
	connect( file_menu_save_as_action.get(), &QAction::triggered,
		the_central_widget.get(), &level_editor_widget::save_level_as );
	
	connect( file_menu_quit_action.get(), &QAction::triggered, this, 
		&primary_widget::file_menu_quit );
	
	
	// Connect the edit_menu actions to the slots.
	connect( edit_menu_copy_rs_contents_action.get(), &QAction::triggered, 
		this, &primary_widget::edit_menu_copy_rs_contents );
	connect( edit_menu_paste_copied_rs_contents_action.get(), 
		&QAction::triggered, this, 
		&primary_widget::edit_menu_paste_copied_rs_contents );
	connect( edit_menu_undo_action.get(), &QAction::triggered, this, 
		&primary_widget::edit_menu_undo );
	connect( edit_menu_redo_action.get(), &QAction::triggered, this, 
		&primary_widget::edit_menu_redo );
	
	
	// Connect the level_menu actions to the slots.
	connect( level_menu_sublevel_properties_action.get(), 
		&QAction::triggered, the_central_widget.get(), 
		&level_editor_widget::create_sublevel_properties_widget );
	//connect( level_menu_level_properties_action.get(), 
	//	&QAction::triggered, this, 
	//	&primary_widget::create_level_properties_widget );
	connect( level_menu_export_action.get(), &QAction::triggered, this, 
		&primary_widget::level_menu_export );
	
	
	
	
	// Add the menus to the menu bar
	file_menu = menuBar()->addMenu("&File");
	edit_menu = menuBar()->addMenu("&Edit");
	level_menu = menuBar()->addMenu("&Level");
	
	
	
	
	// Add the file_menu actions to the file_menu
	file_menu->addAction(file_menu_laugh_action.get());
	file_menu->addAction(file_menu_open_action.get());
	file_menu->addAction(file_menu_save_action.get());
	file_menu->addAction(file_menu_save_as_action.get());
	file_menu->addSeparator();
	file_menu->addAction(file_menu_quit_action.get());
	
	
	// Add the edit_menu actions to the edit_menu
	edit_menu->addAction(edit_menu_copy_rs_contents_action.get());
	edit_menu->addAction(edit_menu_paste_copied_rs_contents_action.get());
	edit_menu->addAction(edit_menu_undo_action.get());
	edit_menu->addAction(edit_menu_redo_action.get());
	
	
	// Add the level_menu actions to the level_menu
	level_menu->addAction(level_menu_sublevel_properties_action.get());
	//level_menu->addAction(level_menu_level_properties_action.get());
	level_menu->addAction(level_menu_export_action.get());
}



bool primary_widget::generate_toolbar()
{
	//QPixmap laugh_pixmap(laugh_icon_file_name),
	//	draw_mode_pixmap(draw_mode_icon_file_name),
	//	erase_mode_pixmap(erase_mode_icon_file_name),
	//	sprite_properties_mode_pixmap
	//	(sprite_properties_mode_icon_file_name),
	//	rect_selection_mode_pixmap(rect_selection_mode_icon_file_name);
	
	vector<QPixmap> toolbar_icon_pixmap_vec;
	
	for ( int the_tbt=0; the_tbt<tbt_count; ++the_tbt )
	{
		const QString& the_icon_file_name 
			= toolbar_icon_file_name_vec.at(the_tbt);
		
		toolbar_icon_pixmap_vec.push_back(QPixmap(the_icon_file_name));
		
		if ( toolbar_icon_pixmap_vec.back().isNull() )
		{
			cout << "Unable to open " << the_icon_file_name.toStdString()
				<< " for reading!  ";
			return false;
		}
	}
	
	
	
	toolbar = addToolBar("main toolbar");
	
	toolbar->setIconSize( QSize( 32, 32 ) );
	
	if ( toolbar_action_text_vec.size() != tbt_count )
	{
		say_tool_related_vector_size_is_wrong("toolbar_action_text_vec");
	}
	if ( tool_button_shortcut_vec.size() != tbt_count )
	{
		say_tool_related_vector_size_is_wrong("tool_button_shortcut_vec");
	}
	
	#define multi_equals_comparison(other_suffix) \
		the_tbt == tbt_##other_suffix ||
	
	for ( int the_tbt=0; the_tbt<tbt_count; ++the_tbt )
	{
		const QPixmap& curr_icon_pixmap 
			= toolbar_icon_pixmap_vec.at(the_tbt);
		const QString& curr_action_text 
			= toolbar_action_text_vec.at(the_tbt);
		
		
		
		//same_case_thing(list_of_debug_tbt_suffixes_part_1)
		if ( list_of_debug_tbt_suffixes_part_1(multi_equals_comparison)
			false )
		{
			cout << "laugh\n";
			
			laugh_tool_button_action 
				= toolbar->addAction( QIcon(curr_icon_pixmap), 
				curr_action_text );
			connect( laugh_tool_button_action, &QAction::triggered, 
				this, &primary_widget::file_menu_laugh );
			toolbar->addSeparator();
		}
		
		else if ( list_of_mm_switching_tbt_suffixes
			(multi_equals_comparison) false )
		{
			//cout << the_tbt << endl;
			
			mm_switching_tool_button_action_vec
				.push_back(new QAction( curr_action_text, this ));
			mm_switching_tool_button_action_vec.back()
				->setIcon(QIcon(curr_icon_pixmap));
		}
		
		else
		{
			cout << "Uh oh!  There's a bug in "
				<< "primary_widget::generate_toolbar()!\n";
		}
	}
	
	#undef multi_equals_comparison
	
	
	//the_mouse_mode_button_group_widget.reset
	//	(new mouse_mode_button_group_widget( this, QPoint(), QSize(),
	//	draw_mode_tool_button_action.get(),
	//	sprite_properties_mode_tool_button_action.get(),
	//	rect_selection_mode_tool_button_action.get() ));
	
	//the_mouse_mode_button_group_widget.reset
	//	(new mouse_mode_button_group_widget( this, QPoint(), QSize(),
	//	draw_mode_tool_button_action, erase_mode_tool_button_action,
	//	sprite_properties_mode_tool_button_action,
	//	rect_selection_mode_tool_button_action ));
	
	//the_mouse_mode_button_group_widget.reset
	//	(new mouse_mode_button_group_widget( this, QPoint(), QSize(),
	//	mm_switching_tool_button_action_vec.at(tbt_draw_mode 
	//		- tbt_draw_mode),
	//	mm_switching_tool_button_action_vec.at(tbt_erase_mode 
	//		- tbt_draw_mode),
	//	mm_switching_tool_button_action_vec.at(tbt_sprite_properties_mode
	//		- tbt_draw_mode),
	//	mm_switching_tool_button_action_vec.at(tbt_rect_selection_mode
	//		- tbt_draw_mode) ));
	
	the_mouse_mode_button_group_widget.reset
		(new mouse_mode_button_group_widget( this, QPoint(), QSize(),
		mm_switching_tool_button_action_vec ));
	
	toolbar->addWidget(the_mouse_mode_button_group_widget.get());
	
	toolbar->setMovable(false);
	
	return true;
}

void primary_widget::generate_central_widget()
{
	the_central_widget.reset(new level_editor_widget
		( the_editing_manager.get(), &argv_copy, this ));
	
	setCentralWidget(the_central_widget.get());
}


void primary_widget::generate_level_header_file
	( const string& output_dirname, const string& output_basename )
{
	level& the_level = the_central_widget->the_level;
	
	fstream header_file( output_dirname + "/" + output_basename 
		+ string(".hpp"), ios_base::out );
	
	header_file << "#ifndef " << output_basename << "_hpp\n"
		<< "#define " << output_basename << "_hpp\n\n\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "level_class.hpp\"\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "sprite_level_data_stuff.hpp\"\n";
	
	header_file << "\n\n";
	
	for ( u32 i=0; i<the_level.sublevel_vec.size(); ++i )
	{
		sublevel& the_sublevel = the_level.sublevel_vec.at(i);
		
		header_file << "extern const sublevel< "
			<< the_sublevel.compressed_block_data_vec.size() << ", " 
			<< the_sublevel.real_size_2d.x << ", " 
			<< the_sublevel.real_size_2d.y << ", "
			<< the_sublevel.sprite_ipgws_vec_for_exporting.size() << ", "
			<< the_sublevel.sublevel_entrance_vec.size() 
			<< " > " << output_basename << "_sublevel_" << i << ";\n\n";
	}
	
	
	header_file << "\nextern const level " << output_basename << ";\n";
	
	header_file << "\n\n";
	
	header_file << "#endif\t\t// " << output_basename << "_hpp";
}

void primary_widget::generate_level_cpp_file( const string& output_dirname, 
	const string& output_basename )
{
	level& the_level = the_central_widget->the_level;
	
	fstream cpp_file( output_dirname + "/" + output_basename 
		+ string(".thumb.cpp"), ios_base::out );
	
	cpp_file << "#include \"" << output_basename << ".hpp\"\n\n\n";
	
	//for ( tiled_sublevel& the_tiled_sublevel : tiled_sublevel_vec )
	for ( u32 i=0; i<the_level.sublevel_vec.size(); ++i )
	{
		sublevel& the_sublevel = the_level.sublevel_vec.at(i);
		
		cpp_file << "const sublevel< "
			<< the_sublevel.compressed_block_data_vec .size() << ", "
			<< the_sublevel.real_size_2d.x << ", " 
			<< the_sublevel.real_size_2d.y << ", "
			<< the_sublevel.sprite_ipgws_vec_for_exporting.size() << ", "
			<< the_sublevel.sublevel_entrance_vec.size()
			<< " > " << output_basename << "_sublevel_" << i << "\n" 
			<< "= {\n";
		
		
		// Build the array of compressed block data
		cpp_file << "\t{\n";
		
		u32 counter = 0;
		
		for ( u32& the_u32 
			: the_sublevel.compressed_block_data_vec )
		{
			////cpp_file << hex << "\t\t0x" << *iter << dec << ",\n";
			//cpp_file << hex << "\t\t0x" << the_u32 << dec << ",\n";
			if ( counter % 4 == 0 )
			{
				cpp_file << "\t\t";
			}
			
			//cpp_file << hex << "0x" << *iter << dec;
			cpp_file << hex << "0x" << the_u32 << dec;
			
			if ( counter % 4 == 3 )
			{
				cpp_file << ",\n";
			}
			else
			{
				cpp_file << ", ";
			}
			
			++counter;
		}
		
		if ( counter % 4 != 0 )
		{
			cpp_file << "\n";
		}
		cpp_file << "\t},\n\t\n";
		
		
		// Build the array of sprite_init_param_group's
		cpp_file << "\t{\n";
		
		for ( sprite_ipgws& the_sprite_ipgws
			: the_sublevel.sprite_ipgws_vec_for_exporting )
		{
			cpp_file << "\t\t{ ";
			
			if ( the_sprite_ipgws.type 
				< sprite_type_helper::st_name_vec.size() )
			{
				cpp_file << sprite_type_helper::get_st_name
					(the_sprite_ipgws.type);
			}
			else
			{
				// Print out a number if the_sprite_type doesn't have a
				// corresponding string in sprite_type_helper::st_name_vec.
				
				cout << "Warning:  the_sprite_ipgws.type doesn't have a "
					<< "corresponding string in sprite_type_helper "
					<< "::st_name_vec!\n";
				cpp_file << the_sprite_ipgws.type;
			}
			
			cpp_file << ", " << the_sprite_ipgws.initial_block_grid_x_coord 
				<< ", " << the_sprite_ipgws.initial_block_grid_y_coord 
				<< ", " 
				<< ( the_sprite_ipgws.facing_right ? "true" : "false" );
			
			
			// This had to be changed to make the Sherwin's Adventure code
			// compile with the OUTDATED version of arm-none-eabi- prefixed
			// GCC and Binutils that are included in devkitARM
			//// Don't clutter the sprite_init_param_group's that have only
			//// zeroes as extra parameters.
			//if ( !( the_sprite_ipgws.extra_param_0 == 0 
			//	&& the_sprite_ipgws.extra_param_1 == 0
			//	&& the_sprite_ipgws.extra_param_2 == 0 
			//	&& the_sprite_ipgws.extra_param_3 == 0 ) )
			//{
			//	cpp_file << ", " << the_sprite_ipgws.extra_param_0 << ", " 
			//		<< the_sprite_ipgws.extra_param_1 << ", " 
			//		<< the_sprite_ipgws.extra_param_2 << ", " 
			//		<< the_sprite_ipgws.extra_param_3;
			//}
			
			//cpp_file << ", " << the_sprite_ipgws.extra_param_0 << ", " 
			//	<< the_sprite_ipgws.extra_param_1 << ", " 
			//	<< the_sprite_ipgws.extra_param_2 << ", " 
			//	<< the_sprite_ipgws.extra_param_3;
			
			
			//if ( the_sprite_ipgws.type == st_door )
			//{
			//	cpp_file << ", " << the_sprite_ipgws.extra_param_0 << ", ";
			//	
			//	////if ( the_sprite_ipgws
			//	////	<< the_sprite_ipgws.extra_param_1 << ", ";
			//	//if ( the_sprite_ipgws.extra_param_1 == (u32)-1 )
			//	//{
			//	//	//cout << "Found a sprite_init_param_group with -1 for "
			//	//	//	<< "its extra_param_1!\n";
			//	//	cpp_file << i;
			//	//}
			//	//else
			//	//{
			//	//	cpp_file << the_sprite_ipgws.extra_param_1;
			//	//}
			//	
			//	cpp_file << the_sprite_ipgws.extra_param_1;
			//	
			//	
			//	cpp_file << ", " << the_sprite_ipgws.extra_param_2 << ", " 
			//		<< the_sprite_ipgws.extra_param_3;
			//}
			//else
			//{
			//	cpp_file << ", " << the_sprite_ipgws.extra_param_0 << ", " 
			//		<< the_sprite_ipgws.extra_param_1 << ", " 
			//		<< the_sprite_ipgws.extra_param_2 << ", " 
			//		<< the_sprite_ipgws.extra_param_3;
			//}
			
			cpp_file << ", " << the_sprite_ipgws.extra_param_0 << ", " 
				<< the_sprite_ipgws.extra_param_1 << ", " 
				<< the_sprite_ipgws.extra_param_2 << ", " 
				<< the_sprite_ipgws.extra_param_3;
			
			cpp_file << " },\n";
		}
		
		cpp_file << "\t},\n\t\n";
		
		// Build the array of sublevel_entrance's
		cpp_file << "\t{\n";
		
		for ( sublevel_entrance& the_sle 
			: the_sublevel.sublevel_entrance_vec )
		{
			cpp_file << "\t\t{ " << get_sle_name_debug(the_sle.type)
				<< hex << ", vec2_f24p8( {0x" 
				<< the_sle.in_sublevel_pos.x.data << "}, {0x" 
				<< the_sle.in_sublevel_pos.y.data << "} ) },\n" << dec;
		}
		
		cpp_file << "\t},\n";
		
		
		cpp_file << "};\n\n\n";
	}
	
	
	cpp_file << "const level " << output_basename << " = level\n\t"
		<< "( sublevel_pointer(" << output_basename << "_sublevel_0)";
	
	if ( the_level.sublevel_vec.size() == 1 )
	{
		cpp_file << " );\n";
	}
	else
	{
		cpp_file << ",\n";
		
		//for ( tiled_sublevel& the_tiled_sublevel : sublevel_vec )
		for ( size_t i=1; i<the_level.sublevel_vec.size(); ++i )
		{
			cpp_file << "\tsublevel_pointer(" << output_basename 
				<< "_sublevel_" << i << ")";
			
			if ( i == the_level.sublevel_vec.size() - 1 )
			{
				cpp_file << " );";
			}
			else
			{
				cpp_file << ",";
			}
			
			cpp_file << "\n";
		}
	}
	
	cpp_file << "\n\n\n";
}


// File Menu Slots
void primary_widget::file_menu_laugh()
{
	cout << "AHAHAHAHA\n\n";
	
	
	
	sublevel& the_sublevel = the_central_widget->get_curr_sublevel();
	
	for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
	{
		for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
		{
			cout << block_stuff::get_bt_name
				((block_type)the_sublevel.uncompressed_block_data_vec_2d
				.at(j).at(i).type) << ",";
		}
		cout << endl;
	}
	
	//sfml_canvas_widget* the_sfml_canvas_widget = the_central_widget
	//	->the_core_widget->the_sfml_canvas_widget;
	//
	//QScrollArea* scroll_area = the_central_widget->scroll_area;
	//
	//sf::Vector2u sfml_canvas_size_2d = the_sfml_canvas_widget->getSize();
	//
	//sf::FloatRect sfml_canvas_visible_rect = the_sfml_canvas_widget
	//	->get_visible_rect();
	//
	//QRect scroll_area_bounding_rect = scroll_area->visibleRegion()
	//	.boundingRect();
	//
	//sf::FloatRect scroll_area_visible_rect( sf::Vector2f
	//	( (double)scroll_area_bounding_rect.x(), 
	//	(double)scroll_area_bounding_rect.y() ),
	//	sf::Vector2f( (double)scroll_area_bounding_rect.width(),
	//	(double)scroll_area_bounding_rect.height() ) );
	//
	//
	//
	//cout << sfml_canvas_visible_rect.left << ", "
	//	<< sfml_canvas_visible_rect.top << ", "
	//	<< sfml_canvas_visible_rect.width << ", "
	//	<< sfml_canvas_visible_rect.height << endl;
	//
	////cout << sfml_canvas_size_2d.x << ", " << sfml_canvas_size_2d.y << endl;
	//cout << scroll_area_visible_rect.left << ", "
	//	<< scroll_area_visible_rect.top << ", "
	//	<< scroll_area_visible_rect.width << ", "
	//	<< scroll_area_visible_rect.height << endl;
	//
	//
	//cout << scroll_area->horizontalScrollBar()->height() << ", "
	//	<< scroll_area->verticalScrollBar()->width() << endl;
	//
	//cout << endl;
	
	//sfml_canvas_widget* the_sfml_canvas_widget = the_central_widget
	//	->the_core_widget->the_sfml_canvas_widget;
	//
	//sf::FloatRect the_sfml_canvas_visible_rect = the_sfml_canvas_widget
	//	->get_visible_rect();
	//
	//cout << the_sfml_canvas_visible_rect.left << ", "
	//	<< the_sfml_canvas_visible_rect.top << ", "
	//	<< the_sfml_canvas_visible_rect.width << ", "
	//	<< the_sfml_canvas_visible_rect.height << endl;
	//
	//cout << endl;
	
}

void primary_widget::file_menu_quit()
{
	quit_non_slot();
}



void primary_widget::edit_menu_copy_rs_contents()
{
	//cout << "edit_menu_copy_rs_contents()\n";
	
	level_editor_core_widget* the_core_widget 
		= the_central_widget->get_curr_level_editor_core_widget
		( string("Weird bug in edit_menu_copy_rs_contents():  ")
		+ string("the_core_widget == NULL.\nExpect a segfault....") );
	
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget
		->the_sfml_canvas_widget->the_rect_selection_stuff;
	
	the_editing_manager->copy_rs_contents( the_core_widget, 
		the_rect_selection_stuff );
}

void primary_widget::edit_menu_paste_copied_rs_contents()
{
	//cout << "edit_menu_paste_copied_rs_contents()\n";
	
	level_editor_core_widget* the_core_widget 
		= the_central_widget->get_curr_level_editor_core_widget
		( string("Weird bug in edit_menu_paste_copied_rs_contents():  ")
		+ string("the_core_widget == NULL.\nExpect a segfault....") );
	
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget
		->the_sfml_canvas_widget->the_rect_selection_stuff;
	
	the_editing_manager->paste_copied_rs_contents( the_core_widget,
		the_rect_selection_stuff );
}

void primary_widget::edit_menu_undo()
{
	//cout << "edit_menu_undo()\n";
	
	level_editor_core_widget* the_core_widget 
		= the_central_widget->get_curr_level_editor_core_widget
		( string("Weird bug in edit_menu_undo():  the_core_widget ")
		+ string("== NULL.\nExpect a segfault....") );
	
	the_editing_manager->undo(the_core_widget);
}

void primary_widget::edit_menu_redo()
{
	//cout << "edit_menu_redo()\n";
	
	level_editor_core_widget* the_core_widget 
		= the_central_widget->get_curr_level_editor_core_widget
		( string("Weird bug in edit_menu_redo():  the_core_widget ")
		+ string("== NULL.\nExpect a segfault....") );
	
	the_editing_manager->redo(the_core_widget);
}

void primary_widget::level_menu_export()
{
	cout << "level_menu_export()\n";
	
	if ( argv_copy.size() != 3 )
	{
		cout << "Error in primary_widget::level_menu_export():  "
			<< "argv_copy.size() != 3.\n";
		
		return;
	}
	
	string output_dirname = dirname(strdup(argv_copy.at(2).c_str())),
		output_basename = basename(strdup(argv_copy.at(2).c_str()));
	
	
	if ( !the_central_widget->the_level
		.generate_compressed_block_data_vectors( output_dirname, 
		output_basename ) )
	{
		cout << "Error in primary_widget::level_menu_export() after "
			<< "attempting to generate compressed block data vectors.\n";
		
		return;
	}
	
	if ( !the_central_widget->the_level
		.generate_sprite_ipgws_and_sle_stuff_for_exporting() )
	{
		cout << "Error in primary_widget::level_menu_export() after "
			<< "attempting to generate sprite and sublevel_entrance "
			<< "vectors.\n";
		
		return;
	}
	
	generate_level_header_file( output_dirname, output_basename );
	generate_level_cpp_file( output_dirname, output_basename );
	
}

