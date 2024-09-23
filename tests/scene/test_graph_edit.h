/**************************************************************************/
/*  test_graph_edit.h                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_GRAPH_EDIT_H
#define TEST_GRAPH_EDIT_H

#include "scene/gui/graph_edit.h"
#include "tests/test_macros.h"

namespace TestGraphEdit {

TEST_CASE("[SceneTree][GraphEdit]: Node connection management") {
	GraphEdit graph_edit;
	GraphNode *src_node = memnew(GraphNode);
	src_node->set_slot(0, false, 0, Color(), true, 0, Color(), Ref<Texture2D>(), Ref<Texture2D>());
	src_node->set_slot(1, false, 0, Color(), true, 1, Color(), Ref<Texture2D>(), Ref<Texture2D>());
	GraphNode *sink_node = memnew(GraphNode);
	sink_node->set_slot(0, true, 0, Color(), false, 0, Color(), Ref<Texture2D>(), Ref<Texture2D>());
	sink_node->set_slot(0, true, 1, Color(), false, 0, Color(), Ref<Texture2D>(), Ref<Texture2D>());

	GraphNode *node1 = memnew(GraphNode);
	node1->set_slot(0, true, 0, Color(), true, 0, Color(), Ref<Texture2D>(), Ref<Texture2D>());

	graph_edit.add_child(src_node);
	graph_edit.add_child(sink_node);
	graph_edit.add_child(node1);

	SUBCASE("Connect and check node connection") {
		CHECK(graph_edit.connect_node("src_node", 0, "sink_node", 0) == OK);
		CHECK(graph_edit.connect_node("src_node", 0, "sink_node", 1) == OK);
		CHECK(graph_edit.connect_node("src_node", 1, "sink_node", 1) == OK);
		CHECK(graph_edit.is_node_connected("src_node", 0, "sink_node", 0));
		CHECK(graph_edit.is_node_connected("src_node", 0, "sink_node", 1));
		CHECK(graph_edit.is_node_connected("src_node", 1, "sink_node", 1));
		CHECK(graph_edit.get_connection_list().size() == 3);
	}

	SUBCASE("Disconnect and check node connection") {
		graph_edit.connect_node("src_node", 0, "sink_node", 0);
		graph_edit.connect_node("src_node", 0, "sink_node", 1);
		graph_edit.connect_node("src_node", 1, "sink_node", 1);

		graph_edit.disconnect_node("src_node", 0, "sink_node", 0);
		graph_edit.disconnect_node("src_node", 0, "sink_node", 1);
		graph_edit.disconnect_node("src_node", 1, "sink_node", 1);

		CHECK_FALSE(graph_edit.is_node_connected("src_node", 0, "sink_node", 0));
		CHECK_FALSE(graph_edit.is_node_connected("src_node", 0, "sink_node", 1));
		CHECK_FALSE(graph_edit.is_node_connected("src_node", 1, "sink_node", 1));
		CHECK(graph_edit.get_connection_list().size() == 0);
	}

	SUBCASE("Clear All Connections") {
		graph_edit.connect_node("src_node", 0, "sink_node", 1);
		graph_edit.clear_connections();

		CHECK(graph_edit.is_node_connected("src_node", 0, "sink_node", 1) == false);
		CHECK(graph_edit.get_connection_list().size() == 0);
	}

	SUBCASE("Connect non-existent nodes") {
		// connect_node() does not check whether the nodes/ports exist. Currently, this is done when updating all connections before drawing.
		CHECK(graph_edit.connect_node("src_node", 0, "sink_node", 2) == OK);
		CHECK(graph_edit.get_connection_list().size() == 1);

		MessageQueue::get_singleton()->flush();

		CHECK(graph_edit.get_connection_list().size() == 0);
	}
}

TEST_CASE("GraphEdit: Connection Queries") {
	GraphEdit graph_edit;

	SUBCASE("Get Connection List") {
		CHECK(graph_edit.get_connection_list().size() == 0);
		graph_edit.connect_node("node1", 0, "node2", 1);
		graph_edit.connect_node("node1", 0, "node2", 2);
		graph_edit.connect_node("node1", 0, "node2", 3);
		CHECK(graph_edit.get_connection_list().size() == 3);
	}

	SUBCASE("Get Closest Connection at Point") {
		graph_edit.connect_node("node1", 0, "node2", 1);
		CHECK(graph_edit.get_closest_connection_at_point(Vector2(0, 0)).is_valid() == true);
	}

	SUBCASE("Get Connections Intersecting with Rect") {
		graph_edit.connect_node("node1", 0, "node2", 1);
		CHECK(graph_edit.get_connections_intersecting_with_rect(Rect2(Vector2(0, 0), Vector2(100, 100))).size() == 1);
	}

	SUBCASE("Check Node Hover Validity") {
		CHECK(graph_edit.is_node_hover_valid("node1", 0, "node2", 1) == true);
	}
}

TEST_CASE("GraphEdit: Connection Activity Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Reset Connection Activity") {
		graph_edit.connect_node("node1", 0, "node2", 1);
		graph_edit.set_connection_activity("node1", 0, "node2", 1, 1.0f);
		graph_edit.reset_all_connection_activity();
		// No specific state to check, just ensure it doesn't crash
	}
}

TEST_CASE("GraphEdit: Valid Connection Types Management") {
	GraphEdit graph_edit;

	SUBCASE("Add and Check Valid Connection Type") {
		graph_edit.add_valid_connection_type(1, 2);
		CHECK(graph_edit.is_valid_connection_type(1, 2) == true);
	}

	SUBCASE("Remove and Check Valid Connection Type") {
		graph_edit.add_valid_connection_type(1, 2);
		graph_edit.remove_valid_connection_type(1, 2);
		CHECK(graph_edit.is_valid_connection_type(1, 2) == false);
	}
}

TEST_CASE("GraphEdit: Frame Attachment Management") {
	GraphEdit graph_edit;

	SUBCASE("Attach and Get Element Frame") {
		graph_edit.attach_graph_element_to_frame("node1", "frame1");
		CHECK(graph_edit.get_element_frame("node1") != nullptr);
	}

	SUBCASE("Detach and Get Element Frame") {
		graph_edit.attach_graph_element_to_frame("node1", "frame1");
		graph_edit.detach_graph_element_from_frame("node1");
		CHECK(graph_edit.get_element_frame("node1") == nullptr);
	}

	SUBCASE("Get Attached Nodes of Frame") {
		graph_edit.attach_graph_element_to_frame("node1", "frame1");
		CHECK(graph_edit.get_attached_nodes_of_frame("frame1").size() == 1);
	}
}

TEST_CASE("GraphEdit: Zoom and Panning Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Get Panning Scheme") {
		graph_edit.set_panning_scheme(GraphEdit::SCROLL_PANS);
		CHECK(graph_edit.get_panning_scheme() == GraphEdit::SCROLL_PANS);
	}

	SUBCASE("Set and Get Zoom Level") {
		graph_edit.set_zoom(2.0f);
		CHECK(graph_edit.get_zoom() == 2.0f);
	}

	SUBCASE("Set and Get Zoom Min Level") {
		graph_edit.set_zoom_min(0.5f);
		CHECK(graph_edit.get_zoom_min() == 0.5f);
	}

	SUBCASE("Set and Get Zoom Max Level") {
		graph_edit.set_zoom_max(3.0f);
		CHECK(graph_edit.get_zoom_max() == 3.0f);
	}

	SUBCASE("Set and Get Zoom Step") {
		graph_edit.set_zoom_step(1.1f);
		CHECK(graph_edit.get_zoom_step() == 1.1f);
	}
}

TEST_CASE("GraphEdit: Minimap Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Get Minimap Size") {
		graph_edit.set_minimap_size(Vector2(100, 100));
		CHECK(graph_edit.get_minimap_size() == Vector2(100, 100));
	}

	SUBCASE("Set and Get Minimap Opacity") {
		graph_edit.set_minimap_opacity(0.5f);
		CHECK(graph_edit.get_minimap_opacity() == 0.5f);
	}

	SUBCASE("Enable and Check Minimap") {
		graph_edit.set_minimap_enabled(true);
		CHECK(graph_edit.is_minimap_enabled() == true);
	}
}

TEST_CASE("GraphEdit: UI Elements Visibility Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Check Menu Visibility") {
		graph_edit.set_show_menu(false);
		CHECK(graph_edit.is_showing_menu() == false);
	}

	SUBCASE("Set and Check Zoom Label Visibility") {
		graph_edit.set_show_zoom_label(true);
		CHECK(graph_edit.is_showing_zoom_label() == true);
	}

	SUBCASE("Set and Check Grid Buttons Visibility") {
		graph_edit.set_show_grid_buttons(false);
		CHECK(graph_edit.is_showing_grid_buttons() == false);
	}

	SUBCASE("Set and Check Zoom Buttons Visibility") {
		graph_edit.set_show_zoom_buttons(true);
		CHECK(graph_edit.is_showing_zoom_buttons() == true);
	}

	SUBCASE("Set and Check Minimap Button Visibility") {
		graph_edit.set_show_minimap_button(false);
		CHECK(graph_edit.is_showing_minimap_button() == false);
	}

	SUBCASE("Set and Check Arrange Button Visibility") {
		graph_edit.set_show_arrange_button(true);
		CHECK(graph_edit.is_showing_arrange_button() == true);
	}
}

TEST_CASE("GraphEdit: Shader and Disconnect Management") {
	GraphEdit graph_edit;

	SUBCASE("Override Connections Shader") {
		Ref<Shader> shader;
		graph_edit.override_connections_shader(shader);
		// No specific state to check, just ensure it doesn't crash
	}

	SUBCASE("Set and Check Right Disconnects") {
		graph_edit.set_right_disconnects(true);
		CHECK(graph_edit.is_right_disconnects_enabled() == true);
	}

	SUBCASE("Add and Remove Valid Right Disconnect Type") {
		graph_edit.add_valid_right_disconnect_type(1);
		graph_edit.remove_valid_right_disconnect_type(1);
		// No specific state to check, just ensure it doesn't crash
	}

	SUBCASE("Add and Remove Valid Left Disconnect Type") {
		graph_edit.add_valid_left_disconnect_type(1);
		graph_edit.remove_valid_left_disconnect_type(1);
		// No specific state to check, just ensure it doesn't crash
	}
}

TEST_CASE("GraphEdit: Scroll and Selection Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Get Scroll Offset") {
		graph_edit.set_scroll_offset(Vector2(10, 10));
		CHECK(graph_edit.get_scroll_offset() == Vector2(10, 10));
	}

	SUBCASE("Set Selected Node") {
		Node node;
		graph_edit.set_selected(&node);
		// No specific state to check, just ensure it doesn't crash
	}
}

TEST_CASE("GraphEdit: Snapping and Grid Management") {
	GraphEdit graph_edit;

	SUBCASE("Enable and Check Snapping") {
		graph_edit.set_snapping_enabled(true);
		CHECK(graph_edit.is_snapping_enabled() == true);
	}

	SUBCASE("Set and Get Snapping Distance") {
		graph_edit.set_snapping_distance(15);
		CHECK(graph_edit.get_snapping_distance() == 15);
	}

	SUBCASE("Set and Check Grid Visibility") {
		graph_edit.set_show_grid(false);
		CHECK(graph_edit.is_showing_grid() == false);
	}

	SUBCASE("Set and Get Grid Pattern") {
		graph_edit.set_grid_pattern(GraphEdit::GRID_PATTERN_DOTS);
		CHECK(graph_edit.get_grid_pattern() == GraphEdit::GRID_PATTERN_DOTS);
	}
}

TEST_CASE("GraphEdit: Connection Lines Management") {
	GraphEdit graph_edit;

	SUBCASE("Set and Get Connection Lines Curvature") {
		graph_edit.set_connection_lines_curvature(0.7f);
		CHECK(graph_edit.get_connection_lines_curvature() == 0.7f);
	}

	SUBCASE("Set and Get Connection Lines Thickness") {
		graph_edit.set_connection_lines_thickness(5.0f);
		CHECK(graph_edit.get_connection_lines_thickness() == 5.0f);
	}

	SUBCASE("Set and Check Connection Lines Antialiasing") {
		graph_edit.set_connection_lines_antialiased(false);
		CHECK(graph_edit.is_connection_lines_antialiased() == false);
	}
}

TEST_CASE("GraphEdit: Miscellaneous Methods") {
	GraphEdit graph_edit;

	SUBCASE("Get Menu HBox") {
		CHECK(graph_edit.get_menu_hbox() != nullptr);
	}

	SUBCASE("Get Panner") {
		CHECK(graph_edit.get_panner() != nullptr);
	}

	SUBCASE("Set Warped Panning") {
		graph_edit.set_warped_panning(true);
		// No specific state to check, just ensure it doesn't crash
	}

	SUBCASE("Arrange Nodes") {
		graph_edit.arrange_nodes();
		// No specific state to check, just ensure it doesn't crash
	}
}

} // namespace TestGraphEdit

#endif // TEST_GRAPH_EDIT_H
