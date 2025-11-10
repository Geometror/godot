/**************************************************************************/
/*  shader_loader_rd.cpp                                                  */
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

#include "shader_loader_rd.h"

#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/io/json.h"
#include "core/templates/hash_set.h"
#include "core/templates/vector.h"

ShaderLoaderRD *ShaderLoaderRD::singleton = nullptr;

ShaderLoaderRD::ShaderLoaderRD() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

ShaderLoaderRD::~ShaderLoaderRD() {
	singleton = nullptr;
}

static void _process_shader_file(const String &p_path, Vector<String> &r_vertex_lines, Vector<String> &r_fragment_lines, Vector<String> &r_compute_lines, HashSet<String> &r_vertex_included, HashSet<String> &r_fragment_included, HashSet<String> &r_compute_included, int p_depth);

static void _include_shader_file(const String &p_path, Vector<String> &r_lines, HashSet<String> &r_included_files, int p_depth) {
	if (r_included_files.has(p_path)) {
		return;
	}

	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
	ERR_FAIL_COND_MSG(file.is_null(), vformat("Shader include file does not exist: %s", p_path));

	r_included_files.insert(p_path);

	while (!file->eof_reached()) {
		String line = file->get_line();

		// Note: Keep comments as they might be useful for the LLM.
		// int comment_pos = line.find("//");
		// if (comment_pos != -1) {
		// 	line = line.substr(0, comment_pos);
		// }

		// Check for #include directive.
		int include_pos = line.find("#include ");
		if (include_pos != -1) {
			// Extract the include filename
			String include_directive = line.substr(include_pos + 9).strip_edges();
			if (include_directive.length() >= 2 && include_directive[0] == '"' && include_directive[include_directive.length() - 1] == '"') {
				String include_file = include_directive.substr(1, include_directive.length() - 2);

				// Resolve the include path.
				String resolved_path;
				if (include_file.begins_with("thirdparty/")) {
					resolved_path = include_file;
				} else {
					String base_dir = p_path.get_base_dir();
					resolved_path = base_dir.path_join(include_file).simplify_path();
				}

				_include_shader_file(resolved_path, r_lines, r_included_files, p_depth + 1);
			}
		} else {
			r_lines.push_back(line);
		}
	}
}

static void _process_shader_file(const String &p_path, Vector<String> &r_vertex_lines, Vector<String> &r_fragment_lines, Vector<String> &r_compute_lines, HashSet<String> &r_vertex_included, HashSet<String> &r_fragment_included, HashSet<String> &r_compute_included, int p_depth) {
	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
	ERR_FAIL_COND_MSG(file.is_null(), vformat("Shader file does not exist: %s", p_path));

	enum Section {
		SECTION_NONE,
		SECTION_VERTEX,
		SECTION_FRAGMENT,
		SECTION_COMPUTE
	};

	Section current_section = SECTION_NONE;
	Vector<String> *current_lines = nullptr;
	HashSet<String> *current_included = nullptr;

	while (!file->eof_reached()) {
		String line = file->get_line();

		// Note: Keep comments as they might be useful for the LLM.
		// int comment_pos = line.find("//");
		// if (comment_pos != -1) {
		// 	line = line.substr(0, comment_pos);
		// }

		// Check for section markers
		if (line.find("#[vertex]") != -1) {
			current_section = SECTION_VERTEX;
			current_lines = &r_vertex_lines;
			current_included = &r_vertex_included;
			continue;
		} else if (line.find("#[fragment]") != -1) {
			current_section = SECTION_FRAGMENT;
			current_lines = &r_fragment_lines;
			current_included = &r_fragment_included;
			continue;
		} else if (line.find("#[compute]") != -1) {
			current_section = SECTION_COMPUTE;
			current_lines = &r_compute_lines;
			current_included = &r_compute_included;
			continue;
		}

		if (current_section != SECTION_NONE && current_lines != nullptr && current_included != nullptr) {
			int include_pos = line.find("#include ");
			if (include_pos != -1) {
				// Extract the include filename
				String include_directive = line.substr(include_pos + 9).strip_edges();
				if (include_directive.length() >= 2 && include_directive[0] == '"' && include_directive[include_directive.length() - 1] == '"') {
					String include_file = include_directive.substr(1, include_directive.length() - 2);

					// Resolve the include path
					// TODO: Handle this correctly
					String resolved_path;
					if (include_file.begins_with("thirdparty/")) {
						resolved_path = include_file;
					} else {
						String base_dir = p_path.get_base_dir();
						resolved_path = base_dir.path_join(include_file).simplify_path();
					}

					// Only include if not already included in this section
					if (!current_included->has(resolved_path)) {
						_include_shader_file(resolved_path, *current_lines, *current_included, p_depth + 1);
					}
				}
			} else {
				current_lines->push_back(line);
			}
		}
	}
}

ShaderLoaderRD::ShaderLoadResult ShaderLoaderRD::load_shader_file(const String &p_path) {
	ShaderLoadResult result;

	Vector<String> vertex_lines;
	Vector<String> fragment_lines;
	Vector<String> compute_lines;

	HashSet<String> vertex_included;
	HashSet<String> fragment_included;
	HashSet<String> compute_included;

	Ref<DirAccess> dir = DirAccess::open(p_path.get_base_dir());
	String s = dir->get_full_path(p_path, DirAccess::ACCESS_FILESYSTEM);

	_process_shader_file(p_path, vertex_lines, fragment_lines, compute_lines, vertex_included, fragment_included, compute_included, 0);

	result.vertex_code = String("\n").join(vertex_lines);
	result.fragment_code = String("\n").join(fragment_lines);
	result.compute_code = String("\n").join(compute_lines);

	// Add final newline if there's content.
	if (!result.vertex_code.is_empty()) {
		result.vertex_code += "\n";
	}
	if (!result.fragment_code.is_empty()) {
		result.fragment_code += "\n";
	}
	if (!result.compute_code.is_empty()) {
		result.compute_code += "\n";
	}

	return result;
}
