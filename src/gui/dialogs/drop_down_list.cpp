/*
   Copyright (C) 2008 - 2016 by Mark de Wever <koraq@xs4all.nl>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/dialogs/drop_down_list.hpp"
#include "gui/widgets/listbox.hpp"
#include "gui/auxiliary/find_widget.hpp"
#include "gui/dialogs/dialog.hpp"
#include "gui/widgets/integer_selector.hpp"
#include "gui/widgets/window.hpp"
#include "gui/widgets/settings.hpp"

#include "utils/functional.hpp"

namespace gui2
{

REGISTER_DIALOG(drop_down_list)

namespace {
	void click_callback(twindow& window, bool& handled, bool& halt, tpoint coordinate)
	{
		SDL_Rect rect = window.get_rectangle();
		if(coordinate.x < rect.x || coordinate.x > rect.x + rect.w || coordinate.y < rect.y || coordinate.y > rect.y + rect.h ) {
			halt = handled = true;
			window.set_retval(twindow::CANCEL);
			window.close();
		}
	}

	void resize_callback(twindow& window)
	{
		window.set_retval(twindow::CANCEL);
		window.close();
	}
}

void tdrop_down_list::pre_show(twindow& window)
{
	window.set_variable("button_x", variant(button_pos_.x));
	window.set_variable("button_y", variant(button_pos_.y));
	window.set_variable("button_w", variant(button_pos_.w));
	window.set_variable("button_h", variant(button_pos_.h));

	tlistbox& list = find_widget<tlistbox>(&window, "list", true);

	std::map<std::string, string_map> data;
	string_map item;
	
	//t_string& image      = data["icon"]["label"];
	//t_string& label      = data["label"]["label"];
	//t_string& tooltip    = data["label"]["tooltip"];
	//t_string& use_markup = data["label"]["use_markup"];

	for(const auto& entry : items_) {
		data.clear();

		item["label"] = entry["icon"];
		data.emplace("icon", item);

		item["label"] = entry["label"];
		item["tooltip"] = entry["tooltip"];
		item["use_markup"] = "true";
		data.emplace("label", item);

		list.add_row(data);
	}

	list.select_row(selected_item_);

	list.set_callback_item_change(std::bind(&tdrop_down_list::item_change_callback, this, std::ref(window), _1));

	//Dismiss on click outside the window
	window.connect_signal<event::SDL_LEFT_BUTTON_UP>(std::bind(&click_callback, std::ref(window), _3, _4, _5), event::tdispatcher::front_child);

	//Dismiss on resize
	window.connect_signal<event::SDL_VIDEO_RESIZE>(std::bind(&resize_callback, std::ref(window)), event::tdispatcher::front_child);
}

void tdrop_down_list::item_change_callback(twindow& window, size_t item)
{
	selected_item_ = item;
	window.set_retval(twindow::OK);
	window.close();
}

void tdrop_down_list::post_show(twindow&)
{
}

} // namespace gui2
