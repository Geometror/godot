/*************************************************************************/
/*  variant_call.cpp                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "variant.h"
#include "core/debugger/engine_debugger.h"
#include "core/io/compression.h"
#include "core/io/marshalls.h"
#include "core/templates/local_vector.h"
#include "core/templates/oa_hash_map.h"
#include "core/error/error_list.h"
#include "core/error/error_macros.h"
#include "core/math/aabb.h"
#include "core/math/basis.h"
#include "core/math/color.h"
#include "core/math/math_defs.h"
#include "core/math/math_funcs.h"
#include "core/math/plane.h"
#include "core/math/quaternion.h"
#include "core/math/rect2.h"
#include "core/math/rect2i.h"
#include "core/math/transform_2d.h"
#include "core/math/transform_3d.h"
#include "core/math/vector2.h"
#include "core/math/vector2i.h"
#include "core/math/vector3.h"
#include "core/math/vector3i.h"
#include "core/object/method_bind.h"
#include "core/object/object.h"
#include "core/object/object_id.h"
#include "core/os/memory.h"
#include "core/string/node_path.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/templates/hashfuncs.h"
#include "core/templates/list.h"
#include "core/templates/map.h"
#include "core/templates/rid.h"
#include "core/templates/vector.h"
#include "core/typedefs.h"
#include "core/variant/array.h"
#include "core/variant/binder_common.h"
#include "core/variant/callable.h"
#include "core/variant/dictionary.h"
#include "core/variant/method_ptrcall.h"
#include "core/variant/type_info.h"
#include "core/variant/variant_internal.h"

struct _register_variant_builtin_methods::Method_AABB_abs;
struct _register_variant_builtin_methods::Method_AABB_encloses;
struct _register_variant_builtin_methods::Method_AABB_expand;
struct _register_variant_builtin_methods::Method_AABB_get_center;
struct _register_variant_builtin_methods::Method_AABB_get_endpoint;
struct _register_variant_builtin_methods::Method_AABB_get_longest_axis;
struct _register_variant_builtin_methods::Method_AABB_get_longest_axis_index;
struct _register_variant_builtin_methods::Method_AABB_get_longest_axis_size;
struct _register_variant_builtin_methods::Method_AABB_get_shortest_axis;
struct _register_variant_builtin_methods::Method_AABB_get_shortest_axis_index;
struct _register_variant_builtin_methods::Method_AABB_get_shortest_axis_size;
struct _register_variant_builtin_methods::Method_AABB_get_support;
struct _register_variant_builtin_methods::Method_AABB_get_volume;
struct _register_variant_builtin_methods::Method_AABB_grow;
struct _register_variant_builtin_methods::Method_AABB_has_no_surface;
struct _register_variant_builtin_methods::Method_AABB_has_no_volume;
struct _register_variant_builtin_methods::Method_AABB_has_point;
struct _register_variant_builtin_methods::Method_AABB_intersection;
struct _register_variant_builtin_methods::Method_AABB_intersects;
struct _register_variant_builtin_methods::Method_AABB_intersects_plane;
struct _register_variant_builtin_methods::Method_AABB_intersects_ray;
struct _register_variant_builtin_methods::Method_AABB_intersects_segment;
struct _register_variant_builtin_methods::Method_AABB_is_equal_approx;
struct _register_variant_builtin_methods::Method_AABB_merge;
struct _register_variant_builtin_methods::Method_Array_append;
struct _register_variant_builtin_methods::Method_Array_append_array;
struct _register_variant_builtin_methods::Method_Array_back;
struct _register_variant_builtin_methods::Method_Array_bsearch;
struct _register_variant_builtin_methods::Method_Array_bsearch_custom;
struct _register_variant_builtin_methods::Method_Array_clear;
struct _register_variant_builtin_methods::Method_Array_count;
struct _register_variant_builtin_methods::Method_Array_duplicate;
struct _register_variant_builtin_methods::Method_Array_erase;
struct _register_variant_builtin_methods::Method_Array_fill;
struct _register_variant_builtin_methods::Method_Array_filter;
struct _register_variant_builtin_methods::Method_Array_find;
struct _register_variant_builtin_methods::Method_Array_find_last;
struct _register_variant_builtin_methods::Method_Array_front;
struct _register_variant_builtin_methods::Method_Array_has;
struct _register_variant_builtin_methods::Method_Array_hash;
struct _register_variant_builtin_methods::Method_Array_insert;
struct _register_variant_builtin_methods::Method_Array_is_empty;
struct _register_variant_builtin_methods::Method_Array_map;
struct _register_variant_builtin_methods::Method_Array_max;
struct _register_variant_builtin_methods::Method_Array_min;
struct _register_variant_builtin_methods::Method_Array_pop_at;
struct _register_variant_builtin_methods::Method_Array_pop_back;
struct _register_variant_builtin_methods::Method_Array_pop_front;
struct _register_variant_builtin_methods::Method_Array_push_back;
struct _register_variant_builtin_methods::Method_Array_push_front;
struct _register_variant_builtin_methods::Method_Array_reduce;
struct _register_variant_builtin_methods::Method_Array_remove_at;
struct _register_variant_builtin_methods::Method_Array_resize;
struct _register_variant_builtin_methods::Method_Array_reverse;
struct _register_variant_builtin_methods::Method_Array_rfind;
struct _register_variant_builtin_methods::Method_Array_shuffle;
struct _register_variant_builtin_methods::Method_Array_size;
struct _register_variant_builtin_methods::Method_Array_slice;
struct _register_variant_builtin_methods::Method_Array_sort;
struct _register_variant_builtin_methods::Method_Array_sort_custom;
struct _register_variant_builtin_methods::Method_Basis_determinant;
struct _register_variant_builtin_methods::Method_Basis_from_euler;
struct _register_variant_builtin_methods::Method_Basis_from_scale;
struct _register_variant_builtin_methods::Method_Basis_get_euler;
struct _register_variant_builtin_methods::Method_Basis_get_orthogonal_index;
struct _register_variant_builtin_methods::Method_Basis_get_rotation_quaternion;
struct _register_variant_builtin_methods::Method_Basis_get_scale;
struct _register_variant_builtin_methods::Method_Basis_inverse;
struct _register_variant_builtin_methods::Method_Basis_is_equal_approx;
struct _register_variant_builtin_methods::Method_Basis_looking_at;
struct _register_variant_builtin_methods::Method_Basis_orthonormalized;
struct _register_variant_builtin_methods::Method_Basis_rotated;
struct _register_variant_builtin_methods::Method_Basis_scaled;
struct _register_variant_builtin_methods::Method_Basis_slerp;
struct _register_variant_builtin_methods::Method_Basis_tdotx;
struct _register_variant_builtin_methods::Method_Basis_tdoty;
struct _register_variant_builtin_methods::Method_Basis_tdotz;
struct _register_variant_builtin_methods::Method_Basis_transposed;
struct _register_variant_builtin_methods::Method_Callable_bind;
struct _register_variant_builtin_methods::Method_Callable_call;
struct _register_variant_builtin_methods::Method_Callable_call_deferred;
struct _register_variant_builtin_methods::Method_Callable_get_method;
struct _register_variant_builtin_methods::Method_Callable_get_object;
struct _register_variant_builtin_methods::Method_Callable_get_object_id;
struct _register_variant_builtin_methods::Method_Callable_hash;
struct _register_variant_builtin_methods::Method_Callable_is_custom;
struct _register_variant_builtin_methods::Method_Callable_is_null;
struct _register_variant_builtin_methods::Method_Callable_is_standard;
struct _register_variant_builtin_methods::Method_Callable_is_valid;
struct _register_variant_builtin_methods::Method_Callable_rpc;
struct _register_variant_builtin_methods::Method_Callable_rpc_id;
struct _register_variant_builtin_methods::Method_Callable_unbind;
struct _register_variant_builtin_methods::Method_Color_blend;
struct _register_variant_builtin_methods::Method_Color_clamp;
struct _register_variant_builtin_methods::Method_Color_darkened;
struct _register_variant_builtin_methods::Method_Color_find_named_color;
struct _register_variant_builtin_methods::Method_Color_from_hsv;
struct _register_variant_builtin_methods::Method_Color_from_rgbe9995;
struct _register_variant_builtin_methods::Method_Color_from_string;
struct _register_variant_builtin_methods::Method_Color_get_luminance;
struct _register_variant_builtin_methods::Method_Color_get_named_color;
struct _register_variant_builtin_methods::Method_Color_get_named_color_count;
struct _register_variant_builtin_methods::Method_Color_get_named_color_name;
struct _register_variant_builtin_methods::Method_Color_hex64;
struct _register_variant_builtin_methods::Method_Color_hex;
struct _register_variant_builtin_methods::Method_Color_html;
struct _register_variant_builtin_methods::Method_Color_html_is_valid;
struct _register_variant_builtin_methods::Method_Color_inverted;
struct _register_variant_builtin_methods::Method_Color_is_equal_approx;
struct _register_variant_builtin_methods::Method_Color_lerp;
struct _register_variant_builtin_methods::Method_Color_lightened;
struct _register_variant_builtin_methods::Method_Color_to_abgr32;
struct _register_variant_builtin_methods::Method_Color_to_abgr64;
struct _register_variant_builtin_methods::Method_Color_to_argb32;
struct _register_variant_builtin_methods::Method_Color_to_argb64;
struct _register_variant_builtin_methods::Method_Color_to_html;
struct _register_variant_builtin_methods::Method_Color_to_rgba32;
struct _register_variant_builtin_methods::Method_Color_to_rgba64;
struct _register_variant_builtin_methods::Method_Dictionary_clear;
struct _register_variant_builtin_methods::Method_Dictionary_duplicate;
struct _register_variant_builtin_methods::Method_Dictionary_erase;
struct _register_variant_builtin_methods::Method_Dictionary_get;
struct _register_variant_builtin_methods::Method_Dictionary_has;
struct _register_variant_builtin_methods::Method_Dictionary_has_all;
struct _register_variant_builtin_methods::Method_Dictionary_hash;
struct _register_variant_builtin_methods::Method_Dictionary_is_empty;
struct _register_variant_builtin_methods::Method_Dictionary_keys;
struct _register_variant_builtin_methods::Method_Dictionary_size;
struct _register_variant_builtin_methods::Method_Dictionary_values;
struct _register_variant_builtin_methods::Method_NodePath_get_as_property_path;
struct _register_variant_builtin_methods::Method_NodePath_get_concatenated_subnames;
struct _register_variant_builtin_methods::Method_NodePath_get_name;
struct _register_variant_builtin_methods::Method_NodePath_get_name_count;
struct _register_variant_builtin_methods::Method_NodePath_get_subname;
struct _register_variant_builtin_methods::Method_NodePath_get_subname_count;
struct _register_variant_builtin_methods::Method_NodePath_is_absolute;
struct _register_variant_builtin_methods::Method_NodePath_is_empty;
struct _register_variant_builtin_methods::Method_PackedByteArray_append;
struct _register_variant_builtin_methods::Method_PackedByteArray_append_array;
struct _register_variant_builtin_methods::Method_PackedByteArray_bsearch;
struct _register_variant_builtin_methods::Method_PackedByteArray_compress;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_double;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_float;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_half;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_s16;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_s32;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_s64;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_s8;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_u16;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_u32;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_u64;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_u8;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_var;
struct _register_variant_builtin_methods::Method_PackedByteArray_decode_var_size;
struct _register_variant_builtin_methods::Method_PackedByteArray_decompress;
struct _register_variant_builtin_methods::Method_PackedByteArray_decompress_dynamic;
struct _register_variant_builtin_methods::Method_PackedByteArray_duplicate;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_double;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_float;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_half;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_s16;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_s32;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_s64;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_s8;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_u16;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_u32;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_u64;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_u8;
struct _register_variant_builtin_methods::Method_PackedByteArray_encode_var;
struct _register_variant_builtin_methods::Method_PackedByteArray_fill;
struct _register_variant_builtin_methods::Method_PackedByteArray_get_string_from_ascii;
struct _register_variant_builtin_methods::Method_PackedByteArray_get_string_from_utf16;
struct _register_variant_builtin_methods::Method_PackedByteArray_get_string_from_utf32;
struct _register_variant_builtin_methods::Method_PackedByteArray_get_string_from_utf8;
struct _register_variant_builtin_methods::Method_PackedByteArray_has;
struct _register_variant_builtin_methods::Method_PackedByteArray_has_encoded_var;
struct _register_variant_builtin_methods::Method_PackedByteArray_hex_encode;
struct _register_variant_builtin_methods::Method_PackedByteArray_insert;
struct _register_variant_builtin_methods::Method_PackedByteArray_is_empty;
struct _register_variant_builtin_methods::Method_PackedByteArray_push_back;
struct _register_variant_builtin_methods::Method_PackedByteArray_remove_at;
struct _register_variant_builtin_methods::Method_PackedByteArray_resize;
struct _register_variant_builtin_methods::Method_PackedByteArray_reverse;
struct _register_variant_builtin_methods::Method_PackedByteArray_set;
struct _register_variant_builtin_methods::Method_PackedByteArray_size;
struct _register_variant_builtin_methods::Method_PackedByteArray_slice;
struct _register_variant_builtin_methods::Method_PackedByteArray_sort;
struct _register_variant_builtin_methods::Method_PackedByteArray_to_float32_array;
struct _register_variant_builtin_methods::Method_PackedByteArray_to_float64_array;
struct _register_variant_builtin_methods::Method_PackedByteArray_to_int32_array;
struct _register_variant_builtin_methods::Method_PackedByteArray_to_int64_array;
struct _register_variant_builtin_methods::Method_PackedColorArray_append;
struct _register_variant_builtin_methods::Method_PackedColorArray_append_array;
struct _register_variant_builtin_methods::Method_PackedColorArray_bsearch;
struct _register_variant_builtin_methods::Method_PackedColorArray_duplicate;
struct _register_variant_builtin_methods::Method_PackedColorArray_fill;
struct _register_variant_builtin_methods::Method_PackedColorArray_has;
struct _register_variant_builtin_methods::Method_PackedColorArray_insert;
struct _register_variant_builtin_methods::Method_PackedColorArray_is_empty;
struct _register_variant_builtin_methods::Method_PackedColorArray_push_back;
struct _register_variant_builtin_methods::Method_PackedColorArray_remove_at;
struct _register_variant_builtin_methods::Method_PackedColorArray_resize;
struct _register_variant_builtin_methods::Method_PackedColorArray_reverse;
struct _register_variant_builtin_methods::Method_PackedColorArray_set;
struct _register_variant_builtin_methods::Method_PackedColorArray_size;
struct _register_variant_builtin_methods::Method_PackedColorArray_slice;
struct _register_variant_builtin_methods::Method_PackedColorArray_sort;
struct _register_variant_builtin_methods::Method_PackedColorArray_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_append;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_append_array;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_fill;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_has;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_insert;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_push_back;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_resize;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_reverse;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_set;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_size;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_slice;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_sort;
struct _register_variant_builtin_methods::Method_PackedFloat32Array_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_append;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_append_array;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_fill;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_has;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_insert;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_push_back;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_resize;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_reverse;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_set;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_size;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_slice;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_sort;
struct _register_variant_builtin_methods::Method_PackedFloat64Array_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedInt32Array_append;
struct _register_variant_builtin_methods::Method_PackedInt32Array_append_array;
struct _register_variant_builtin_methods::Method_PackedInt32Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedInt32Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedInt32Array_fill;
struct _register_variant_builtin_methods::Method_PackedInt32Array_has;
struct _register_variant_builtin_methods::Method_PackedInt32Array_insert;
struct _register_variant_builtin_methods::Method_PackedInt32Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedInt32Array_push_back;
struct _register_variant_builtin_methods::Method_PackedInt32Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedInt32Array_resize;
struct _register_variant_builtin_methods::Method_PackedInt32Array_reverse;
struct _register_variant_builtin_methods::Method_PackedInt32Array_set;
struct _register_variant_builtin_methods::Method_PackedInt32Array_size;
struct _register_variant_builtin_methods::Method_PackedInt32Array_slice;
struct _register_variant_builtin_methods::Method_PackedInt32Array_sort;
struct _register_variant_builtin_methods::Method_PackedInt32Array_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedInt64Array_append;
struct _register_variant_builtin_methods::Method_PackedInt64Array_append_array;
struct _register_variant_builtin_methods::Method_PackedInt64Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedInt64Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedInt64Array_fill;
struct _register_variant_builtin_methods::Method_PackedInt64Array_has;
struct _register_variant_builtin_methods::Method_PackedInt64Array_insert;
struct _register_variant_builtin_methods::Method_PackedInt64Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedInt64Array_push_back;
struct _register_variant_builtin_methods::Method_PackedInt64Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedInt64Array_resize;
struct _register_variant_builtin_methods::Method_PackedInt64Array_reverse;
struct _register_variant_builtin_methods::Method_PackedInt64Array_set;
struct _register_variant_builtin_methods::Method_PackedInt64Array_size;
struct _register_variant_builtin_methods::Method_PackedInt64Array_slice;
struct _register_variant_builtin_methods::Method_PackedInt64Array_sort;
struct _register_variant_builtin_methods::Method_PackedInt64Array_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedStringArray_append;
struct _register_variant_builtin_methods::Method_PackedStringArray_append_array;
struct _register_variant_builtin_methods::Method_PackedStringArray_bsearch;
struct _register_variant_builtin_methods::Method_PackedStringArray_duplicate;
struct _register_variant_builtin_methods::Method_PackedStringArray_fill;
struct _register_variant_builtin_methods::Method_PackedStringArray_has;
struct _register_variant_builtin_methods::Method_PackedStringArray_insert;
struct _register_variant_builtin_methods::Method_PackedStringArray_is_empty;
struct _register_variant_builtin_methods::Method_PackedStringArray_push_back;
struct _register_variant_builtin_methods::Method_PackedStringArray_remove_at;
struct _register_variant_builtin_methods::Method_PackedStringArray_resize;
struct _register_variant_builtin_methods::Method_PackedStringArray_reverse;
struct _register_variant_builtin_methods::Method_PackedStringArray_set;
struct _register_variant_builtin_methods::Method_PackedStringArray_size;
struct _register_variant_builtin_methods::Method_PackedStringArray_slice;
struct _register_variant_builtin_methods::Method_PackedStringArray_sort;
struct _register_variant_builtin_methods::Method_PackedStringArray_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedVector2Array_append;
struct _register_variant_builtin_methods::Method_PackedVector2Array_append_array;
struct _register_variant_builtin_methods::Method_PackedVector2Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedVector2Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedVector2Array_fill;
struct _register_variant_builtin_methods::Method_PackedVector2Array_has;
struct _register_variant_builtin_methods::Method_PackedVector2Array_insert;
struct _register_variant_builtin_methods::Method_PackedVector2Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedVector2Array_push_back;
struct _register_variant_builtin_methods::Method_PackedVector2Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedVector2Array_resize;
struct _register_variant_builtin_methods::Method_PackedVector2Array_reverse;
struct _register_variant_builtin_methods::Method_PackedVector2Array_set;
struct _register_variant_builtin_methods::Method_PackedVector2Array_size;
struct _register_variant_builtin_methods::Method_PackedVector2Array_slice;
struct _register_variant_builtin_methods::Method_PackedVector2Array_sort;
struct _register_variant_builtin_methods::Method_PackedVector2Array_to_byte_array;
struct _register_variant_builtin_methods::Method_PackedVector3Array_append;
struct _register_variant_builtin_methods::Method_PackedVector3Array_append_array;
struct _register_variant_builtin_methods::Method_PackedVector3Array_bsearch;
struct _register_variant_builtin_methods::Method_PackedVector3Array_duplicate;
struct _register_variant_builtin_methods::Method_PackedVector3Array_fill;
struct _register_variant_builtin_methods::Method_PackedVector3Array_has;
struct _register_variant_builtin_methods::Method_PackedVector3Array_insert;
struct _register_variant_builtin_methods::Method_PackedVector3Array_is_empty;
struct _register_variant_builtin_methods::Method_PackedVector3Array_push_back;
struct _register_variant_builtin_methods::Method_PackedVector3Array_remove_at;
struct _register_variant_builtin_methods::Method_PackedVector3Array_resize;
struct _register_variant_builtin_methods::Method_PackedVector3Array_reverse;
struct _register_variant_builtin_methods::Method_PackedVector3Array_set;
struct _register_variant_builtin_methods::Method_PackedVector3Array_size;
struct _register_variant_builtin_methods::Method_PackedVector3Array_slice;
struct _register_variant_builtin_methods::Method_PackedVector3Array_sort;
struct _register_variant_builtin_methods::Method_PackedVector3Array_to_byte_array;
struct _register_variant_builtin_methods::Method_Plane_center;
struct _register_variant_builtin_methods::Method_Plane_distance_to;
struct _register_variant_builtin_methods::Method_Plane_has_point;
struct _register_variant_builtin_methods::Method_Plane_intersect_3;
struct _register_variant_builtin_methods::Method_Plane_intersects_ray;
struct _register_variant_builtin_methods::Method_Plane_intersects_segment;
struct _register_variant_builtin_methods::Method_Plane_is_equal_approx;
struct _register_variant_builtin_methods::Method_Plane_is_point_over;
struct _register_variant_builtin_methods::Method_Plane_normalized;
struct _register_variant_builtin_methods::Method_Plane_project;
struct _register_variant_builtin_methods::Method_Quaternion_angle_to;
struct _register_variant_builtin_methods::Method_Quaternion_cubic_slerp;
struct _register_variant_builtin_methods::Method_Quaternion_dot;
struct _register_variant_builtin_methods::Method_Quaternion_get_angle;
struct _register_variant_builtin_methods::Method_Quaternion_get_axis;
struct _register_variant_builtin_methods::Method_Quaternion_get_euler;
struct _register_variant_builtin_methods::Method_Quaternion_inverse;
struct _register_variant_builtin_methods::Method_Quaternion_is_equal_approx;
struct _register_variant_builtin_methods::Method_Quaternion_is_normalized;
struct _register_variant_builtin_methods::Method_Quaternion_length;
struct _register_variant_builtin_methods::Method_Quaternion_length_squared;
struct _register_variant_builtin_methods::Method_Quaternion_normalized;
struct _register_variant_builtin_methods::Method_Quaternion_slerp;
struct _register_variant_builtin_methods::Method_Quaternion_slerpni;
struct _register_variant_builtin_methods::Method_RID_get_id;
struct _register_variant_builtin_methods::Method_Rect2_abs;
struct _register_variant_builtin_methods::Method_Rect2_encloses;
struct _register_variant_builtin_methods::Method_Rect2_expand;
struct _register_variant_builtin_methods::Method_Rect2_get_area;
struct _register_variant_builtin_methods::Method_Rect2_get_center;
struct _register_variant_builtin_methods::Method_Rect2_grow;
struct _register_variant_builtin_methods::Method_Rect2_grow_individual;
struct _register_variant_builtin_methods::Method_Rect2_grow_side;
struct _register_variant_builtin_methods::Method_Rect2_has_no_area;
struct _register_variant_builtin_methods::Method_Rect2_has_point;
struct _register_variant_builtin_methods::Method_Rect2_intersection;
struct _register_variant_builtin_methods::Method_Rect2_intersects;
struct _register_variant_builtin_methods::Method_Rect2_is_equal_approx;
struct _register_variant_builtin_methods::Method_Rect2_merge;
struct _register_variant_builtin_methods::Method_Rect2i_abs;
struct _register_variant_builtin_methods::Method_Rect2i_encloses;
struct _register_variant_builtin_methods::Method_Rect2i_expand;
struct _register_variant_builtin_methods::Method_Rect2i_get_area;
struct _register_variant_builtin_methods::Method_Rect2i_get_center;
struct _register_variant_builtin_methods::Method_Rect2i_grow;
struct _register_variant_builtin_methods::Method_Rect2i_grow_individual;
struct _register_variant_builtin_methods::Method_Rect2i_grow_side;
struct _register_variant_builtin_methods::Method_Rect2i_has_no_area;
struct _register_variant_builtin_methods::Method_Rect2i_has_point;
struct _register_variant_builtin_methods::Method_Rect2i_intersection;
struct _register_variant_builtin_methods::Method_Rect2i_intersects;
struct _register_variant_builtin_methods::Method_Rect2i_merge;
struct _register_variant_builtin_methods::Method_Signal_connect;
struct _register_variant_builtin_methods::Method_Signal_disconnect;
struct _register_variant_builtin_methods::Method_Signal_emit;
struct _register_variant_builtin_methods::Method_Signal_get_connections;
struct _register_variant_builtin_methods::Method_Signal_get_name;
struct _register_variant_builtin_methods::Method_Signal_get_object;
struct _register_variant_builtin_methods::Method_Signal_get_object_id;
struct _register_variant_builtin_methods::Method_Signal_is_connected;
struct _register_variant_builtin_methods::Method_Signal_is_null;
struct _register_variant_builtin_methods::Method_String_begins_with;
struct _register_variant_builtin_methods::Method_String_bigrams;
struct _register_variant_builtin_methods::Method_String_bin_to_int;
struct _register_variant_builtin_methods::Method_String_c_escape;
struct _register_variant_builtin_methods::Method_String_c_unescape;
struct _register_variant_builtin_methods::Method_String_capitalize;
struct _register_variant_builtin_methods::Method_String_casecmp_to;
struct _register_variant_builtin_methods::Method_String_chr;
struct _register_variant_builtin_methods::Method_String_contains;
struct _register_variant_builtin_methods::Method_String_count;
struct _register_variant_builtin_methods::Method_String_countn;
struct _register_variant_builtin_methods::Method_String_dedent;
struct _register_variant_builtin_methods::Method_String_ends_with;
struct _register_variant_builtin_methods::Method_String_find;
struct _register_variant_builtin_methods::Method_String_findn;
struct _register_variant_builtin_methods::Method_String_format;
struct _register_variant_builtin_methods::Method_String_get_base_dir;
struct _register_variant_builtin_methods::Method_String_get_basename;
struct _register_variant_builtin_methods::Method_String_get_extension;
struct _register_variant_builtin_methods::Method_String_get_file;
struct _register_variant_builtin_methods::Method_String_get_slice;
struct _register_variant_builtin_methods::Method_String_hash;
struct _register_variant_builtin_methods::Method_String_hex_to_int;
struct _register_variant_builtin_methods::Method_String_humanize_size;
struct _register_variant_builtin_methods::Method_String_indent;
struct _register_variant_builtin_methods::Method_String_insert;
struct _register_variant_builtin_methods::Method_String_is_absolute_path;
struct _register_variant_builtin_methods::Method_String_is_empty;
struct _register_variant_builtin_methods::Method_String_is_relative_path;
struct _register_variant_builtin_methods::Method_String_is_subsequence_of;
struct _register_variant_builtin_methods::Method_String_is_subsequence_ofn;
struct _register_variant_builtin_methods::Method_String_is_valid_filename;
struct _register_variant_builtin_methods::Method_String_is_valid_float;
struct _register_variant_builtin_methods::Method_String_is_valid_hex_number;
struct _register_variant_builtin_methods::Method_String_is_valid_html_color;
struct _register_variant_builtin_methods::Method_String_is_valid_identifier;
struct _register_variant_builtin_methods::Method_String_is_valid_int;
struct _register_variant_builtin_methods::Method_String_is_valid_ip_address;
struct _register_variant_builtin_methods::Method_String_join;
struct _register_variant_builtin_methods::Method_String_json_escape;
struct _register_variant_builtin_methods::Method_String_left;
struct _register_variant_builtin_methods::Method_String_length;
struct _register_variant_builtin_methods::Method_String_lpad;
struct _register_variant_builtin_methods::Method_String_lstrip;
struct _register_variant_builtin_methods::Method_String_match;
struct _register_variant_builtin_methods::Method_String_matchn;
struct _register_variant_builtin_methods::Method_String_md5_buffer;
struct _register_variant_builtin_methods::Method_String_md5_text;
struct _register_variant_builtin_methods::Method_String_naturalnocasecmp_to;
struct _register_variant_builtin_methods::Method_String_nocasecmp_to;
struct _register_variant_builtin_methods::Method_String_num;
struct _register_variant_builtin_methods::Method_String_num_scientific;
struct _register_variant_builtin_methods::Method_String_pad_decimals;
struct _register_variant_builtin_methods::Method_String_pad_zeros;
struct _register_variant_builtin_methods::Method_String_plus_file;
struct _register_variant_builtin_methods::Method_String_repeat;
struct _register_variant_builtin_methods::Method_String_replace;
struct _register_variant_builtin_methods::Method_String_replacen;
struct _register_variant_builtin_methods::Method_String_rfind;
struct _register_variant_builtin_methods::Method_String_rfindn;
struct _register_variant_builtin_methods::Method_String_right;
struct _register_variant_builtin_methods::Method_String_rpad;
struct _register_variant_builtin_methods::Method_String_rsplit;
struct _register_variant_builtin_methods::Method_String_rstrip;
struct _register_variant_builtin_methods::Method_String_sha1_buffer;
struct _register_variant_builtin_methods::Method_String_sha1_text;
struct _register_variant_builtin_methods::Method_String_sha256_buffer;
struct _register_variant_builtin_methods::Method_String_sha256_text;
struct _register_variant_builtin_methods::Method_String_similarity;
struct _register_variant_builtin_methods::Method_String_simplify_path;
struct _register_variant_builtin_methods::Method_String_split;
struct _register_variant_builtin_methods::Method_String_split_floats;
struct _register_variant_builtin_methods::Method_String_strip_edges;
struct _register_variant_builtin_methods::Method_String_strip_escapes;
struct _register_variant_builtin_methods::Method_String_substr;
struct _register_variant_builtin_methods::Method_String_to_ascii_buffer;
struct _register_variant_builtin_methods::Method_String_to_float;
struct _register_variant_builtin_methods::Method_String_to_int;
struct _register_variant_builtin_methods::Method_String_to_lower;
struct _register_variant_builtin_methods::Method_String_to_upper;
struct _register_variant_builtin_methods::Method_String_to_utf16_buffer;
struct _register_variant_builtin_methods::Method_String_to_utf32_buffer;
struct _register_variant_builtin_methods::Method_String_to_utf8_buffer;
struct _register_variant_builtin_methods::Method_String_trim_prefix;
struct _register_variant_builtin_methods::Method_String_trim_suffix;
struct _register_variant_builtin_methods::Method_String_unicode_at;
struct _register_variant_builtin_methods::Method_String_uri_decode;
struct _register_variant_builtin_methods::Method_String_uri_encode;
struct _register_variant_builtin_methods::Method_String_validate_node_name;
struct _register_variant_builtin_methods::Method_String_xml_escape;
struct _register_variant_builtin_methods::Method_String_xml_unescape;
struct _register_variant_builtin_methods::Method_Transform2D_affine_inverse;
struct _register_variant_builtin_methods::Method_Transform2D_basis_xform;
struct _register_variant_builtin_methods::Method_Transform2D_basis_xform_inv;
struct _register_variant_builtin_methods::Method_Transform2D_get_origin;
struct _register_variant_builtin_methods::Method_Transform2D_get_rotation;
struct _register_variant_builtin_methods::Method_Transform2D_get_scale;
struct _register_variant_builtin_methods::Method_Transform2D_get_skew;
struct _register_variant_builtin_methods::Method_Transform2D_interpolate_with;
struct _register_variant_builtin_methods::Method_Transform2D_inverse;
struct _register_variant_builtin_methods::Method_Transform2D_is_equal_approx;
struct _register_variant_builtin_methods::Method_Transform2D_looking_at;
struct _register_variant_builtin_methods::Method_Transform2D_orthonormalized;
struct _register_variant_builtin_methods::Method_Transform2D_rotated;
struct _register_variant_builtin_methods::Method_Transform2D_scaled;
struct _register_variant_builtin_methods::Method_Transform2D_set_rotation;
struct _register_variant_builtin_methods::Method_Transform2D_set_scale;
struct _register_variant_builtin_methods::Method_Transform2D_set_skew;
struct _register_variant_builtin_methods::Method_Transform2D_translated;
struct _register_variant_builtin_methods::Method_Transform3D_affine_inverse;
struct _register_variant_builtin_methods::Method_Transform3D_interpolate_with;
struct _register_variant_builtin_methods::Method_Transform3D_inverse;
struct _register_variant_builtin_methods::Method_Transform3D_is_equal_approx;
struct _register_variant_builtin_methods::Method_Transform3D_looking_at;
struct _register_variant_builtin_methods::Method_Transform3D_orthonormalized;
struct _register_variant_builtin_methods::Method_Transform3D_rotated;
struct _register_variant_builtin_methods::Method_Transform3D_scaled;
struct _register_variant_builtin_methods::Method_Transform3D_sphere_interpolate_with;
struct _register_variant_builtin_methods::Method_Transform3D_translated;
struct _register_variant_builtin_methods::Method_Vector2_abs;
struct _register_variant_builtin_methods::Method_Vector2_angle;
struct _register_variant_builtin_methods::Method_Vector2_angle_to;
struct _register_variant_builtin_methods::Method_Vector2_angle_to_point;
struct _register_variant_builtin_methods::Method_Vector2_aspect;
struct _register_variant_builtin_methods::Method_Vector2_bounce;
struct _register_variant_builtin_methods::Method_Vector2_ceil;
struct _register_variant_builtin_methods::Method_Vector2_clamp;
struct _register_variant_builtin_methods::Method_Vector2_cross;
struct _register_variant_builtin_methods::Method_Vector2_cubic_interpolate;
struct _register_variant_builtin_methods::Method_Vector2_direction_to;
struct _register_variant_builtin_methods::Method_Vector2_distance_squared_to;
struct _register_variant_builtin_methods::Method_Vector2_distance_to;
struct _register_variant_builtin_methods::Method_Vector2_dot;
struct _register_variant_builtin_methods::Method_Vector2_floor;
struct _register_variant_builtin_methods::Method_Vector2_from_angle;
struct _register_variant_builtin_methods::Method_Vector2_is_equal_approx;
struct _register_variant_builtin_methods::Method_Vector2_is_normalized;
struct _register_variant_builtin_methods::Method_Vector2_length;
struct _register_variant_builtin_methods::Method_Vector2_length_squared;
struct _register_variant_builtin_methods::Method_Vector2_lerp;
struct _register_variant_builtin_methods::Method_Vector2_limit_length;
struct _register_variant_builtin_methods::Method_Vector2_max_axis_index;
struct _register_variant_builtin_methods::Method_Vector2_min_axis_index;
struct _register_variant_builtin_methods::Method_Vector2_move_toward;
struct _register_variant_builtin_methods::Method_Vector2_normalized;
struct _register_variant_builtin_methods::Method_Vector2_orthogonal;
struct _register_variant_builtin_methods::Method_Vector2_posmod;
struct _register_variant_builtin_methods::Method_Vector2_posmodv;
struct _register_variant_builtin_methods::Method_Vector2_project;
struct _register_variant_builtin_methods::Method_Vector2_reflect;
struct _register_variant_builtin_methods::Method_Vector2_rotated;
struct _register_variant_builtin_methods::Method_Vector2_round;
struct _register_variant_builtin_methods::Method_Vector2_sign;
struct _register_variant_builtin_methods::Method_Vector2_slerp;
struct _register_variant_builtin_methods::Method_Vector2_slide;
struct _register_variant_builtin_methods::Method_Vector2_snapped;
struct _register_variant_builtin_methods::Method_Vector2i_abs;
struct _register_variant_builtin_methods::Method_Vector2i_aspect;
struct _register_variant_builtin_methods::Method_Vector2i_clamp;
struct _register_variant_builtin_methods::Method_Vector2i_length;
struct _register_variant_builtin_methods::Method_Vector2i_length_squared;
struct _register_variant_builtin_methods::Method_Vector2i_max_axis_index;
struct _register_variant_builtin_methods::Method_Vector2i_min_axis_index;
struct _register_variant_builtin_methods::Method_Vector2i_sign;
struct _register_variant_builtin_methods::Method_Vector3_abs;
struct _register_variant_builtin_methods::Method_Vector3_angle_to;
struct _register_variant_builtin_methods::Method_Vector3_bounce;
struct _register_variant_builtin_methods::Method_Vector3_ceil;
struct _register_variant_builtin_methods::Method_Vector3_clamp;
struct _register_variant_builtin_methods::Method_Vector3_cross;
struct _register_variant_builtin_methods::Method_Vector3_cubic_interpolate;
struct _register_variant_builtin_methods::Method_Vector3_direction_to;
struct _register_variant_builtin_methods::Method_Vector3_distance_squared_to;
struct _register_variant_builtin_methods::Method_Vector3_distance_to;
struct _register_variant_builtin_methods::Method_Vector3_dot;
struct _register_variant_builtin_methods::Method_Vector3_floor;
struct _register_variant_builtin_methods::Method_Vector3_inverse;
struct _register_variant_builtin_methods::Method_Vector3_is_equal_approx;
struct _register_variant_builtin_methods::Method_Vector3_is_normalized;
struct _register_variant_builtin_methods::Method_Vector3_length;
struct _register_variant_builtin_methods::Method_Vector3_length_squared;
struct _register_variant_builtin_methods::Method_Vector3_lerp;
struct _register_variant_builtin_methods::Method_Vector3_limit_length;
struct _register_variant_builtin_methods::Method_Vector3_max_axis_index;
struct _register_variant_builtin_methods::Method_Vector3_min_axis_index;
struct _register_variant_builtin_methods::Method_Vector3_move_toward;
struct _register_variant_builtin_methods::Method_Vector3_normalized;
struct _register_variant_builtin_methods::Method_Vector3_octahedron_decode;
struct _register_variant_builtin_methods::Method_Vector3_octahedron_encode;
struct _register_variant_builtin_methods::Method_Vector3_outer;
struct _register_variant_builtin_methods::Method_Vector3_posmod;
struct _register_variant_builtin_methods::Method_Vector3_posmodv;
struct _register_variant_builtin_methods::Method_Vector3_project;
struct _register_variant_builtin_methods::Method_Vector3_reflect;
struct _register_variant_builtin_methods::Method_Vector3_rotated;
struct _register_variant_builtin_methods::Method_Vector3_round;
struct _register_variant_builtin_methods::Method_Vector3_sign;
struct _register_variant_builtin_methods::Method_Vector3_signed_angle_to;
struct _register_variant_builtin_methods::Method_Vector3_slerp;
struct _register_variant_builtin_methods::Method_Vector3_slide;
struct _register_variant_builtin_methods::Method_Vector3_snapped;
struct _register_variant_builtin_methods::Method_Vector3i_abs;
struct _register_variant_builtin_methods::Method_Vector3i_clamp;
struct _register_variant_builtin_methods::Method_Vector3i_length;
struct _register_variant_builtin_methods::Method_Vector3i_length_squared;
struct _register_variant_builtin_methods::Method_Vector3i_max_axis_index;
struct _register_variant_builtin_methods::Method_Vector3i_min_axis_index;
struct _register_variant_builtin_methods::Method_Vector3i_sign;

typedef void (*VariantFunc)(Variant &r_ret, Variant &p_self, const Variant **p_args);
typedef void (*VariantConstructFunc)(Variant &r_ret, const Variant **p_args);

template <class R, class... P>
static _FORCE_INLINE_ void vc_static_method_call(R (*method)(P...), const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_static_ret_dv(method, p_args, p_argcount, r_ret, r_error, p_defvals);
}

template <class... P>
static _FORCE_INLINE_ void vc_static_method_call(void (*method)(P...), const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_static_dv(method, p_args, p_argcount, r_error, p_defvals);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_method_call(R (T::*method)(P...), Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_ret_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, r_ret, r_error, p_defvals);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_method_call(R (T::*method)(P...) const, Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_retc_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, r_ret, r_error, p_defvals);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_method_call(void (T::*method)(P...), Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, r_error, p_defvals);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_method_call(void (T::*method)(P...) const, Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_argsc_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, r_error, p_defvals);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_method_call_static(R (*method)(T *, P...), Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_retc_static_helper_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, r_ret, p_defvals, r_error);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_method_call_static(void (*method)(T *, P...), Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) {
	call_with_variant_args_static_helper_dv(VariantGetInternalPtr<T>::get_ptr(base), method, p_args, p_argcount, p_defvals, r_error);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_validated_call(R (T::*method)(P...), Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_ret(base, method, p_args, r_ret);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_validated_call(R (T::*method)(P...) const, Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_retc(base, method, p_args, r_ret);
}
template <class T, class... P>
static _FORCE_INLINE_ void vc_validated_call(void (T::*method)(P...), Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args(base, method, p_args);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_validated_call(void (T::*method)(P...) const, Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_argsc(base, method, p_args);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_validated_call_static(R (*method)(T *, P...), Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_static_retc(base, method, p_args, r_ret);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_validated_call_static(void (*method)(T *, P...), Variant *base, const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_static(base, method, p_args);
}

template <class R, class... P>
static _FORCE_INLINE_ void vc_validated_static_call(R (*method)(P...), const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_static_method_ret(method, p_args, r_ret);
}

template <class... P>
static _FORCE_INLINE_ void vc_validated_static_call(void (*method)(P...), const Variant **p_args, Variant *r_ret) {
	call_with_validated_variant_args_static_method(method, p_args);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(R (T::*method)(P...), void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_args_ret(reinterpret_cast<T *>(p_base), method, p_args, r_ret);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(R (T::*method)(P...) const, void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_args_retc(reinterpret_cast<T *>(p_base), method, p_args, r_ret);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(void (T::*method)(P...), void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_args(reinterpret_cast<T *>(p_base), method, p_args);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(void (T::*method)(P...) const, void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_argsc(reinterpret_cast<T *>(p_base), method, p_args);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ int vc_get_argument_count(R (T::*method)(P...)) {
	return sizeof...(P);
}
template <class R, class T, class... P>
static _FORCE_INLINE_ int vc_get_argument_count(R (T::*method)(P...) const) {
	return sizeof...(P);
}

template <class T, class... P>
static _FORCE_INLINE_ int vc_get_argument_count(void (T::*method)(P...)) {
	return sizeof...(P);
}

template <class T, class... P>
static _FORCE_INLINE_ int vc_get_argument_count(void (T::*method)(P...) const) {
	return sizeof...(P);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ int vc_get_argument_count(R (*method)(T *, P...)) {
	return sizeof...(P);
}

template <class R, class... P>
static _FORCE_INLINE_ int vc_get_argument_count_static(R (*method)(P...)) {
	return sizeof...(P);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type(R (T::*method)(P...), int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}
template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type(R (T::*method)(P...) const, int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type(void (T::*method)(P...), int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type(void (T::*method)(P...) const, int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type(R (*method)(T *, P...), int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}

template <class R, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_argument_type_static(R (*method)(P...), int p_arg) {
	return call_get_argument_type<P...>(p_arg);
}

template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(R (T::*method)(P...)) {
	return GetTypeInfo<R>::VARIANT_TYPE;
}

template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(R (T::*method)(P...) const) {
	return GetTypeInfo<R>::VARIANT_TYPE;
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(void (T::*method)(P...)) {
	return Variant::NIL;
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(void (T::*method)(P...) const) {
	return Variant::NIL;
}

template <class R, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(R (*method)(P...)) {
	return GetTypeInfo<R>::VARIANT_TYPE;
}

template <class... P>
static _FORCE_INLINE_ Variant::Type vc_get_return_type(void (*method)(P...)) {
	return Variant::NIL;
}

template <class R, class T, class... P>
static _FORCE_INLINE_ bool vc_has_return_type(R (T::*method)(P...)) {
	return true;
}
template <class R, class T, class... P>
static _FORCE_INLINE_ bool vc_has_return_type(R (T::*method)(P...) const) {
	return true;
}

template <class T, class... P>
static _FORCE_INLINE_ bool vc_has_return_type(void (T::*method)(P...)) {
	return false;
}

template <class T, class... P>
static _FORCE_INLINE_ bool vc_has_return_type(void (T::*method)(P...) const) {
	return false;
}

template <class... P>
static _FORCE_INLINE_ bool vc_has_return_type_static(void (*method)(P...)) {
	return false;
}

template <class R, class... P>
static _FORCE_INLINE_ bool vc_has_return_type_static(R (*method)(P...)) {
	return true;
}

template <class R, class T, class... P>
static _FORCE_INLINE_ bool vc_is_const(R (T::*method)(P...)) {
	return false;
}
template <class R, class T, class... P>
static _FORCE_INLINE_ bool vc_is_const(R (T::*method)(P...) const) {
	return true;
}

template <class T, class... P>
static _FORCE_INLINE_ bool vc_is_const(void (T::*method)(P...)) {
	return false;
}

template <class T, class... P>
static _FORCE_INLINE_ bool vc_is_const(void (T::*method)(P...) const) {
	return true;
}

template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_base_type(R (T::*method)(P...)) {
	return GetTypeInfo<T>::VARIANT_TYPE;
}
template <class R, class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_base_type(R (T::*method)(P...) const) {
	return GetTypeInfo<T>::VARIANT_TYPE;
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_base_type(void (T::*method)(P...)) {
	return GetTypeInfo<T>::VARIANT_TYPE;
}

template <class T, class... P>
static _FORCE_INLINE_ Variant::Type vc_get_base_type(void (T::*method)(P...) const) {
	return GetTypeInfo<T>::VARIANT_TYPE;
}

#define METHOD_CLASS(m_class, m_method_name, m_method_ptr)                                                                                                        \
	struct Method_##m_class##_##m_method_name {                                                                                                                   \
		static void call(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) { \
			vc_method_call(m_method_ptr, base, p_args, p_argcount, r_ret, p_defvals, r_error);                                                                    \
		}                                                                                                                                                         \
		static void validated_call(Variant *base, const Variant **p_args, int p_argcount, Variant *r_ret) {                                                       \
			vc_validated_call(m_method_ptr, base, p_args, r_ret);                                                                                                 \
		}                                                                                                                                                         \
		static void ptrcall(void *p_base, const void **p_args, void *r_ret, int p_argcount) {                                                                     \
			vc_ptrcall(m_method_ptr, p_base, p_args, r_ret);                                                                                                      \
		}                                                                                                                                                         \
		static int get_argument_count() {                                                                                                                         \
			return vc_get_argument_count(m_method_ptr);                                                                                                           \
		}                                                                                                                                                         \
		static Variant::Type get_argument_type(int p_arg) {                                                                                                       \
			return vc_get_argument_type(m_method_ptr, p_arg);                                                                                                     \
		}                                                                                                                                                         \
		static Variant::Type get_return_type() {                                                                                                                  \
			return vc_get_return_type(m_method_ptr);                                                                                                              \
		}                                                                                                                                                         \
		static bool has_return_type() {                                                                                                                           \
			return vc_has_return_type(m_method_ptr);                                                                                                              \
		}                                                                                                                                                         \
		static bool is_const() {                                                                                                                                  \
			return vc_is_const(m_method_ptr);                                                                                                                     \
		}                                                                                                                                                         \
		static bool is_static() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_vararg() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static Variant::Type get_base_type() {                                                                                                                    \
			return vc_get_base_type(m_method_ptr);                                                                                                                \
		}                                                                                                                                                         \
		static StringName get_name() {                                                                                                                            \
			return #m_method_name;                                                                                                                                \
		}                                                                                                                                                         \
	};

template <class R, class... P>
static _FORCE_INLINE_ void vc_static_ptrcall(R (*method)(P...), const void **p_args, void *r_ret) {
	call_with_ptr_args_static_method_ret<R, P...>(method, p_args, r_ret);
}

template <class... P>
static _FORCE_INLINE_ void vc_static_ptrcall(void (*method)(P...), const void **p_args, void *r_ret) {
	call_with_ptr_args_static_method<P...>(method, p_args);
}

#define STATIC_METHOD_CLASS(m_class, m_method_name, m_method_ptr)                                                                                                 \
	struct Method_##m_class##_##m_method_name {                                                                                                                   \
		static void call(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) { \
			vc_static_method_call(m_method_ptr, p_args, p_argcount, r_ret, p_defvals, r_error);                                                                   \
		}                                                                                                                                                         \
		static void validated_call(Variant *base, const Variant **p_args, int p_argcount, Variant *r_ret) {                                                       \
			vc_validated_static_call(m_method_ptr, p_args, r_ret);                                                                                                \
		}                                                                                                                                                         \
		static void ptrcall(void *p_base, const void **p_args, void *r_ret, int p_argcount) {                                                                     \
			vc_static_ptrcall(m_method_ptr, p_args, r_ret);                                                                                                       \
		}                                                                                                                                                         \
		static int get_argument_count() {                                                                                                                         \
			return vc_get_argument_count_static(m_method_ptr);                                                                                                    \
		}                                                                                                                                                         \
		static Variant::Type get_argument_type(int p_arg) {                                                                                                       \
			return vc_get_argument_type_static(m_method_ptr, p_arg);                                                                                              \
		}                                                                                                                                                         \
		static Variant::Type get_return_type() {                                                                                                                  \
			return vc_get_return_type(m_method_ptr);                                                                                                              \
		}                                                                                                                                                         \
		static bool has_return_type() {                                                                                                                           \
			return vc_has_return_type_static(m_method_ptr);                                                                                                       \
		}                                                                                                                                                         \
		static bool is_const() {                                                                                                                                  \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_static() {                                                                                                                                 \
			return true;                                                                                                                                          \
		}                                                                                                                                                         \
		static bool is_vararg() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static Variant::Type get_base_type() {                                                                                                                    \
			return GetTypeInfo<m_class>::VARIANT_TYPE;                                                                                                            \
		}                                                                                                                                                         \
		static StringName get_name() {                                                                                                                            \
			return #m_method_name;                                                                                                                                \
		}                                                                                                                                                         \
	};

template <class R, class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(R (*method)(T *, P...), void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_args_static_retc<T, R, P...>(reinterpret_cast<T *>(p_base), method, p_args, r_ret);
}

template <class T, class... P>
static _FORCE_INLINE_ void vc_ptrcall(void (*method)(T *, P...), void *p_base, const void **p_args, void *r_ret) {
	call_with_ptr_args_static<T, P...>(reinterpret_cast<T *>(p_base), method, p_args);
}

#define FUNCTION_CLASS(m_class, m_method_name, m_method_ptr, m_const)                                                                                             \
	struct Method_##m_class##_##m_method_name {                                                                                                                   \
		static void call(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) { \
			vc_method_call_static(m_method_ptr, base, p_args, p_argcount, r_ret, p_defvals, r_error);                                                             \
		}                                                                                                                                                         \
		static void validated_call(Variant *base, const Variant **p_args, int p_argcount, Variant *r_ret) {                                                       \
			vc_validated_call_static(m_method_ptr, base, p_args, r_ret);                                                                                          \
		}                                                                                                                                                         \
		static void ptrcall(void *p_base, const void **p_args, void *r_ret, int p_argcount) {                                                                     \
			vc_ptrcall(m_method_ptr, p_base, p_args, r_ret);                                                                                                      \
		}                                                                                                                                                         \
		static int get_argument_count() {                                                                                                                         \
			return vc_get_argument_count(m_method_ptr);                                                                                                           \
		}                                                                                                                                                         \
		static Variant::Type get_argument_type(int p_arg) {                                                                                                       \
			return vc_get_argument_type(m_method_ptr, p_arg);                                                                                                     \
		}                                                                                                                                                         \
		static Variant::Type get_return_type() {                                                                                                                  \
			return vc_get_return_type(m_method_ptr);                                                                                                              \
		}                                                                                                                                                         \
		static bool has_return_type() {                                                                                                                           \
			return vc_has_return_type_static(m_method_ptr);                                                                                                       \
		}                                                                                                                                                         \
		static bool is_const() {                                                                                                                                  \
			return m_const;                                                                                                                                       \
		}                                                                                                                                                         \
		static bool is_static() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_vararg() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static Variant::Type get_base_type() {                                                                                                                    \
			return GetTypeInfo<m_class>::VARIANT_TYPE;                                                                                                            \
		}                                                                                                                                                         \
		static StringName get_name() {                                                                                                                            \
			return #m_method_name;                                                                                                                                \
		}                                                                                                                                                         \
	};

#define VARARG_CLASS(m_class, m_method_name, m_method_ptr, m_has_return, m_return_type)                                                                           \
	struct Method_##m_class##_##m_method_name {                                                                                                                   \
		static void call(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) { \
			m_method_ptr(base, p_args, p_argcount, r_ret, r_error);                                                                                               \
		}                                                                                                                                                         \
		static void validated_call(Variant *base, const Variant **p_args, int p_argcount, Variant *r_ret) {                                                       \
			Callable::CallError ce;                                                                                                                               \
			m_method_ptr(base, p_args, p_argcount, *r_ret, ce);                                                                                                   \
		}                                                                                                                                                         \
		static void ptrcall(void *p_base, const void **p_args, void *r_ret, int p_argcount) {                                                                     \
			LocalVector<Variant> vars;                                                                                                                            \
			vars.resize(p_argcount);                                                                                                                              \
			LocalVector<const Variant *> vars_ptrs;                                                                                                               \
			vars_ptrs.resize(p_argcount);                                                                                                                         \
			for (int i = 0; i < p_argcount; i++) {                                                                                                                \
				vars[i] = PtrToArg<Variant>::convert(p_args[i]);                                                                                                  \
				vars_ptrs[i] = &vars[i];                                                                                                                          \
			}                                                                                                                                                     \
			Variant base = PtrToArg<m_class>::convert(p_base);                                                                                                    \
			Variant ret;                                                                                                                                          \
			Callable::CallError ce;                                                                                                                               \
			m_method_ptr(&base, (const Variant **)&vars_ptrs[0], p_argcount, ret, ce);                                                                            \
			if (m_has_return) {                                                                                                                                   \
				m_return_type r = ret;                                                                                                                            \
				PtrToArg<m_return_type>::encode(ret, r_ret);                                                                                                      \
			}                                                                                                                                                     \
		}                                                                                                                                                         \
		static int get_argument_count() {                                                                                                                         \
			return 0;                                                                                                                                             \
		}                                                                                                                                                         \
		static Variant::Type get_argument_type(int p_arg) {                                                                                                       \
			return Variant::NIL;                                                                                                                                  \
		}                                                                                                                                                         \
		static Variant::Type get_return_type() {                                                                                                                  \
			return GetTypeInfo<m_return_type>::VARIANT_TYPE;                                                                                                      \
		}                                                                                                                                                         \
		static bool has_return_type() {                                                                                                                           \
			return m_has_return;                                                                                                                                  \
		}                                                                                                                                                         \
		static bool is_const() {                                                                                                                                  \
			return true;                                                                                                                                          \
		}                                                                                                                                                         \
		static bool is_static() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_vararg() {                                                                                                                                 \
			return true;                                                                                                                                          \
		}                                                                                                                                                         \
		static Variant::Type get_base_type() {                                                                                                                    \
			return GetTypeInfo<m_class>::VARIANT_TYPE;                                                                                                            \
		}                                                                                                                                                         \
		static StringName get_name() {                                                                                                                            \
			return #m_method_name;                                                                                                                                \
		}                                                                                                                                                         \
	};

#define VARARG_CLASS1(m_class, m_method_name, m_method_ptr, m_arg_type)                                                                                           \
	struct Method_##m_class##_##m_method_name {                                                                                                                   \
		static void call(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error) { \
			m_method_ptr(base, p_args, p_argcount, r_ret, r_error);                                                                                               \
		}                                                                                                                                                         \
		static void validated_call(Variant *base, const Variant **p_args, int p_argcount, Variant *r_ret) {                                                       \
			Callable::CallError ce;                                                                                                                               \
			m_method_ptr(base, p_args, p_argcount, *r_ret, ce);                                                                                                   \
		}                                                                                                                                                         \
		static void ptrcall(void *p_base, const void **p_args, void *r_ret, int p_argcount) {                                                                     \
			LocalVector<Variant> vars;                                                                                                                            \
			vars.resize(p_argcount);                                                                                                                              \
			LocalVector<const Variant *> vars_ptrs;                                                                                                               \
			vars_ptrs.resize(p_argcount);                                                                                                                         \
			for (int i = 0; i < p_argcount; i++) {                                                                                                                \
				vars[i] = PtrToArg<Variant>::convert(p_args[i]);                                                                                                  \
				vars_ptrs[i] = &vars[i];                                                                                                                          \
			}                                                                                                                                                     \
			Variant base = PtrToArg<m_class>::convert(p_base);                                                                                                    \
			Variant ret;                                                                                                                                          \
			Callable::CallError ce;                                                                                                                               \
			m_method_ptr(&base, (const Variant **)&vars_ptrs[0], p_argcount, ret, ce);                                                                            \
		}                                                                                                                                                         \
		static int get_argument_count() {                                                                                                                         \
			return 1;                                                                                                                                             \
		}                                                                                                                                                         \
		static Variant::Type get_argument_type(int p_arg) {                                                                                                       \
			return m_arg_type;                                                                                                                                    \
		}                                                                                                                                                         \
		static Variant::Type get_return_type() {                                                                                                                  \
			return Variant::NIL;                                                                                                                                  \
		}                                                                                                                                                         \
		static bool has_return_type() {                                                                                                                           \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_const() {                                                                                                                                  \
			return true;                                                                                                                                          \
		}                                                                                                                                                         \
		static bool is_static() {                                                                                                                                 \
			return false;                                                                                                                                         \
		}                                                                                                                                                         \
		static bool is_vararg() {                                                                                                                                 \
			return true;                                                                                                                                          \
		}                                                                                                                                                         \
		static Variant::Type get_base_type() {                                                                                                                    \
			return GetTypeInfo<m_class>::VARIANT_TYPE;                                                                                                            \
		}                                                                                                                                                         \
		static StringName get_name() {                                                                                                                            \
			return #m_method_name;                                                                                                                                \
		}                                                                                                                                                         \
	};

struct _VariantCall {
	static String func_PackedByteArray_get_string_from_ascii(PackedByteArray *p_instance) {
		String s;
		if (p_instance->size() > 0) {
			const uint8_t *r = p_instance->ptr();
			CharString cs;
			cs.resize(p_instance->size() + 1);
			memcpy(cs.ptrw(), r, p_instance->size());
			cs[p_instance->size()] = 0;

			s = cs.get_data();
		}
		return s;
	}

	static String func_PackedByteArray_get_string_from_utf8(PackedByteArray *p_instance) {
		String s;
		if (p_instance->size() > 0) {
			const uint8_t *r = p_instance->ptr();
			s.parse_utf8((const char *)r, p_instance->size());
		}
		return s;
	}

	static String func_PackedByteArray_get_string_from_utf16(PackedByteArray *p_instance) {
		String s;
		if (p_instance->size() > 0) {
			const uint8_t *r = p_instance->ptr();
			s.parse_utf16((const char16_t *)r, floor((double)p_instance->size() / (double)sizeof(char16_t)));
		}
		return s;
	}

	static String func_PackedByteArray_get_string_from_utf32(PackedByteArray *p_instance) {
		String s;
		if (p_instance->size() > 0) {
			const uint8_t *r = p_instance->ptr();
			s = String((const char32_t *)r, floor((double)p_instance->size() / (double)sizeof(char32_t)));
		}
		return s;
	}

	static PackedByteArray func_PackedByteArray_compress(PackedByteArray *p_instance, int p_mode) {
		PackedByteArray compressed;

		if (p_instance->size() > 0) {
			Compression::Mode mode = (Compression::Mode)(p_mode);
			compressed.resize(Compression::get_max_compressed_buffer_size(p_instance->size(), mode));
			int result = Compression::compress(compressed.ptrw(), p_instance->ptr(), p_instance->size(), mode);

			result = result >= 0 ? result : 0;
			compressed.resize(result);
		}

		return compressed;
	}

	static PackedByteArray func_PackedByteArray_decompress(PackedByteArray *p_instance, int64_t p_buffer_size, int p_mode) {
		PackedByteArray decompressed;
		Compression::Mode mode = (Compression::Mode)(p_mode);

		int64_t buffer_size = p_buffer_size;

		if (buffer_size <= 0) {
			ERR_FAIL_V_MSG(decompressed, "Decompression buffer size must be greater than zero.");
		}
		if (p_instance->size() == 0) {
			ERR_FAIL_V_MSG(decompressed, "Compressed buffer size must be greater than zero.");
		}

		decompressed.resize(buffer_size);
		int result = Compression::decompress(decompressed.ptrw(), buffer_size, p_instance->ptr(), p_instance->size(), mode);

		result = result >= 0 ? result : 0;
		decompressed.resize(result);

		return decompressed;
	}

	static PackedByteArray func_PackedByteArray_decompress_dynamic(PackedByteArray *p_instance, int64_t p_buffer_size, int p_mode) {
		PackedByteArray decompressed;
		int64_t max_output_size = p_buffer_size;
		Compression::Mode mode = (Compression::Mode)(p_mode);

		int result = Compression::decompress_dynamic(&decompressed, max_output_size, p_instance->ptr(), p_instance->size(), mode);

		if (result == OK) {
			return decompressed;
		} else {
			decompressed.clear();
			ERR_FAIL_V_MSG(decompressed, "Decompression failed.");
		}
	}

	static String func_PackedByteArray_hex_encode(PackedByteArray *p_instance) {
		if (p_instance->size() == 0) {
			return String();
		}
		const uint8_t *r = p_instance->ptr();
		String s = String::hex_encode_buffer(&r[0], p_instance->size());
		return s;
	}

	static int64_t func_PackedByteArray_decode_u8(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > int64_t(size) - 1, 0);
		const uint8_t *r = p_instance->ptr();
		return r[p_offset];
	}
	static int64_t func_PackedByteArray_decode_s8(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > int64_t(size) - 1, 0);
		const uint8_t *r = p_instance->ptr();
		return *((const int8_t *)&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_u16(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 2), 0);
		const uint8_t *r = p_instance->ptr();
		return decode_uint16(&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_s16(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 2), 0);
		const uint8_t *r = p_instance->ptr();
		return (int16_t)decode_uint16(&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_u32(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 4), 0);
		const uint8_t *r = p_instance->ptr();
		return decode_uint32(&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_s32(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 4), 0);
		const uint8_t *r = p_instance->ptr();
		return (int32_t)decode_uint32(&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_u64(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 8), 0);
		const uint8_t *r = p_instance->ptr();
		return (int64_t)decode_uint64(&r[p_offset]);
	}
	static int64_t func_PackedByteArray_decode_s64(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 8), 0);
		const uint8_t *r = p_instance->ptr();
		return (int64_t)decode_uint64(&r[p_offset]);
	}
	static double func_PackedByteArray_decode_half(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 2), 0);
		const uint8_t *r = p_instance->ptr();
		return Math::half_to_float(decode_uint16(&r[p_offset]));
	}
	static double func_PackedByteArray_decode_float(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 4), 0);
		const uint8_t *r = p_instance->ptr();
		return decode_float(&r[p_offset]);
	}

	static double func_PackedByteArray_decode_double(PackedByteArray *p_instance, int64_t p_offset) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0 || p_offset > (int64_t(size) - 8), 0);
		const uint8_t *r = p_instance->ptr();
		return decode_double(&r[p_offset]);
	}

	static bool func_PackedByteArray_has_encoded_var(PackedByteArray *p_instance, int64_t p_offset, bool p_allow_objects) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0, false);
		const uint8_t *r = p_instance->ptr();
		Variant ret;
		Error err = decode_variant(ret, r + p_offset, size - p_offset, nullptr, p_allow_objects);
		return err == OK;
	}

	static Variant func_PackedByteArray_decode_var(PackedByteArray *p_instance, int64_t p_offset, bool p_allow_objects) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0, Variant());
		const uint8_t *r = p_instance->ptr();
		Variant ret;
		Error err = decode_variant(ret, r + p_offset, size - p_offset, nullptr, p_allow_objects);
		if (err != OK) {
			ret = Variant();
		}
		return ret;
	}

	static int64_t func_PackedByteArray_decode_var_size(PackedByteArray *p_instance, int64_t p_offset, bool p_allow_objects) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0, 0);
		const uint8_t *r = p_instance->ptr();
		Variant ret;
		int r_size;
		Error err = decode_variant(ret, r + p_offset, size - p_offset, &r_size, p_allow_objects);
		if (err == OK) {
			return r_size;
		}
		return 0;
	}

	static PackedInt32Array func_PackedByteArray_decode_s32_array(PackedByteArray *p_instance) {
		uint64_t size = p_instance->size();
		PackedInt32Array dest;
		ERR_FAIL_COND_V_MSG(size < sizeof(int32_t), dest, "Size didn't match array of size int32_t, maybe you are trying to convert to the wrong type?");
		const uint8_t *r = p_instance->ptr();
		dest.resize(size / sizeof(int32_t));
		memcpy(dest.ptrw(), r, size);
		return dest;
	}

	static PackedInt64Array func_PackedByteArray_decode_s64_array(PackedByteArray *p_instance) {
		uint64_t size = p_instance->size();
		PackedInt64Array dest;
		ERR_FAIL_COND_V_MSG(size < sizeof(int64_t), dest, "Size didn't match array of size int64_t, maybe you are trying to convert to the wrong type?");
		const uint8_t *r = p_instance->ptr();
		dest.resize(size / sizeof(int64_t));
		memcpy(dest.ptrw(), r, size);
		return dest;
	}

	static PackedFloat32Array func_PackedByteArray_decode_float_array(PackedByteArray *p_instance) {
		uint64_t size = p_instance->size();
		PackedFloat32Array dest;
		ERR_FAIL_COND_V_MSG(size < sizeof(float), dest, "Size didn't match array of size float, maybe you are trying to convert to the wrong type?");
		const uint8_t *r = p_instance->ptr();
		dest.resize(size / sizeof(float));
		memcpy(dest.ptrw(), r, size);
		return dest;
	}

	static PackedFloat64Array func_PackedByteArray_decode_double_array(PackedByteArray *p_instance) {
		uint64_t size = p_instance->size();
		PackedFloat64Array dest;
		ERR_FAIL_COND_V_MSG(size < sizeof(double), dest, "Size didn't match array of size double, maybe you are trying to convert to the wrong type?");
		const uint8_t *r = p_instance->ptr();
		dest.resize(size / sizeof(double));
		memcpy(dest.ptrw(), r, size);
		return dest;
	}

	static void func_PackedByteArray_encode_u8(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 1);
		uint8_t *w = p_instance->ptrw();
		*((uint8_t *)&w[p_offset]) = p_value;
	}
	static void func_PackedByteArray_encode_s8(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 1);
		uint8_t *w = p_instance->ptrw();
		*((int8_t *)&w[p_offset]) = p_value;
	}

	static void func_PackedByteArray_encode_u16(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 2);
		uint8_t *w = p_instance->ptrw();
		encode_uint16((uint16_t)p_value, &w[p_offset]);
	}
	static void func_PackedByteArray_encode_s16(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 2);
		uint8_t *w = p_instance->ptrw();
		encode_uint16((int16_t)p_value, &w[p_offset]);
	}

	static void func_PackedByteArray_encode_u32(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 4);
		uint8_t *w = p_instance->ptrw();
		encode_uint32((uint32_t)p_value, &w[p_offset]);
	}
	static void func_PackedByteArray_encode_s32(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 4);
		uint8_t *w = p_instance->ptrw();
		encode_uint32((int32_t)p_value, &w[p_offset]);
	}

	static void func_PackedByteArray_encode_u64(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 8);
		uint8_t *w = p_instance->ptrw();
		encode_uint64((uint64_t)p_value, &w[p_offset]);
	}
	static void func_PackedByteArray_encode_s64(PackedByteArray *p_instance, int64_t p_offset, int64_t p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 8);
		uint8_t *w = p_instance->ptrw();
		encode_uint64((int64_t)p_value, &w[p_offset]);
	}

	static void func_PackedByteArray_encode_half(PackedByteArray *p_instance, int64_t p_offset, double p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 2);
		uint8_t *w = p_instance->ptrw();
		encode_uint16(Math::make_half_float(p_value), &w[p_offset]);
	}
	static void func_PackedByteArray_encode_float(PackedByteArray *p_instance, int64_t p_offset, double p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 4);
		uint8_t *w = p_instance->ptrw();
		encode_float(p_value, &w[p_offset]);
	}
	static void func_PackedByteArray_encode_double(PackedByteArray *p_instance, int64_t p_offset, double p_value) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND(p_offset < 0 || p_offset > int64_t(size) - 8);
		uint8_t *w = p_instance->ptrw();
		encode_double(p_value, &w[p_offset]);
	}
	static int64_t func_PackedByteArray_encode_var(PackedByteArray *p_instance, int64_t p_offset, const Variant &p_value, bool p_allow_objects) {
		uint64_t size = p_instance->size();
		ERR_FAIL_COND_V(p_offset < 0, -1);
		uint8_t *w = p_instance->ptrw();
		int len;
		Error err = encode_variant(p_value, nullptr, len, p_allow_objects);
		if (err != OK) {
			return -1;
		}
		if (uint64_t(p_offset + len) > size) {
			return -1; // did not fit
		}
		encode_variant(p_value, w + p_offset, len, p_allow_objects);

		return len;
	}

	static void func_Callable_call(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		Callable *callable = VariantGetInternalPtr<Callable>::get_ptr(v);
		callable->call(p_args, p_argcount, r_ret, r_error);
	}

	static void func_Callable_call_deferred(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		Callable *callable = VariantGetInternalPtr<Callable>::get_ptr(v);
		callable->call_deferred(p_args, p_argcount);
	}

	static void func_Callable_rpc(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		Callable *callable = VariantGetInternalPtr<Callable>::get_ptr(v);
		callable->rpc(0, p_args, p_argcount, r_error);
	}

	static void func_Callable_rpc_id(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		if (p_argcount == 0) {
			r_error.error = Callable::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument = 0;
			r_error.expected = 1;

		} else if (p_args[0]->get_type() != Variant::INT) {
			r_error.error = Callable::CallError::CALL_ERROR_INVALID_ARGUMENT;
			r_error.argument = 0;
			r_error.expected = Variant::INT;
		} else {
			Callable *callable = VariantGetInternalPtr<Callable>::get_ptr(v);
			callable->rpc(*p_args[0], &p_args[1], p_argcount - 1, r_error);
		}
	}

	static void func_Callable_bind(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		Callable *callable = VariantGetInternalPtr<Callable>::get_ptr(v);
		r_ret = callable->bind(p_args, p_argcount);
	}

	static void func_Signal_emit(Variant *v, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
		Signal *signal = VariantGetInternalPtr<Signal>::get_ptr(v);
		signal->emit(p_args, p_argcount);
	}

	struct ConstantData {
		Map<StringName, int> value;
#ifdef DEBUG_ENABLED
		List<StringName> value_ordered;
#endif
		Map<StringName, Variant> variant_value;
#ifdef DEBUG_ENABLED
		List<StringName> variant_value_ordered;
#endif
	};

	static ConstantData *constant_data;

	static void add_constant(int p_type, StringName p_constant_name, int p_constant_value) {
		constant_data[p_type].value[p_constant_name] = p_constant_value;
#ifdef DEBUG_ENABLED
		constant_data[p_type].value_ordered.push_back(p_constant_name);
#endif
	}

	static void add_variant_constant(int p_type, StringName p_constant_name, const Variant &p_constant_value) {
		constant_data[p_type].variant_value[p_constant_name] = p_constant_value;
#ifdef DEBUG_ENABLED
		constant_data[p_type].variant_value_ordered.push_back(p_constant_name);
#endif
	}
};

_VariantCall::ConstantData *_VariantCall::constant_data = nullptr;

struct VariantBuiltInMethodInfo {
	void (*call)(Variant *base, const Variant **p_args, int p_argcount, Variant &r_ret, const Vector<Variant> &p_defvals, Callable::CallError &r_error);
	Variant::ValidatedBuiltInMethod validated_call;
	Variant::PTRBuiltInMethod ptrcall;

	Vector<Variant> default_arguments;
	Vector<String> argument_names;

	bool is_const;
	bool is_static;
	bool has_return_type;
	bool is_vararg;
	Variant::Type return_type;
	int argument_count;
	Variant::Type (*get_argument_type)(int p_arg);
};

typedef OAHashMap<StringName, VariantBuiltInMethodInfo> BuiltinMethodMap;
static BuiltinMethodMap *builtin_method_info;
static List<StringName> *builtin_method_names;

template <class T>
static void register_builtin_method(const Vector<String> &p_argnames, const Vector<Variant> &p_def_args) {
	StringName name = T::get_name();

	ERR_FAIL_COND(builtin_method_info[T::get_base_type()].has(name));

	VariantBuiltInMethodInfo imi;

	imi.call = T::call;
	imi.validated_call = T::validated_call;
	if (T::is_vararg()) {
		imi.ptrcall = nullptr;
	} else {
		imi.ptrcall = T::ptrcall;
	}

	imi.default_arguments = p_def_args;
	imi.argument_names = p_argnames;

	imi.is_const = T::is_const();
	imi.is_static = T::is_static();
	imi.is_vararg = T::is_vararg();
	imi.has_return_type = T::has_return_type();
	imi.return_type = T::get_return_type();
	imi.argument_count = T::get_argument_count();
	imi.get_argument_type = T::get_argument_type;
#ifdef DEBUG_METHODS_ENABLED
	ERR_FAIL_COND(!imi.is_vararg && imi.argument_count != imi.argument_names.size());
#endif

	builtin_method_info[T::get_base_type()].insert(name, imi);
	builtin_method_names[T::get_base_type()].push_back(name);
}

void Variant::call(const StringName &p_method, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
	if (type == Variant::OBJECT) {
		//call object
		Object *obj = _get_obj().obj;
		if (!obj) {
			r_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
			return;
		}
#ifdef DEBUG_ENABLED
		if (EngineDebugger::is_active() && !_get_obj().id.is_ref_counted() && ObjectDB::get_instance(_get_obj().id) == nullptr) {
			r_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
			return;
		}

#endif
		r_ret = _get_obj().obj->call(p_method, p_args, p_argcount, r_error);

		//else if (type==Variant::METHOD) {
	} else {
		r_error.error = Callable::CallError::CALL_OK;

		const VariantBuiltInMethodInfo *imf = builtin_method_info[type].lookup_ptr(p_method);

		if (!imf) {
			r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
			return;
		}

		imf->call(this, p_args, p_argcount, r_ret, imf->default_arguments, r_error);
	}
}

void Variant::call_static(Variant::Type p_type, const StringName &p_method, const Variant **p_args, int p_argcount, Variant &r_ret, Callable::CallError &r_error) {
	r_error.error = Callable::CallError::CALL_OK;

	const VariantBuiltInMethodInfo *imf = builtin_method_info[p_type].lookup_ptr(p_method);

	if (!imf) {
		r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
		return;
	}

	if (!imf->is_static) {
		r_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return;
	}

	imf->call(nullptr, p_args, p_argcount, r_ret, imf->default_arguments, r_error);
}

bool Variant::has_method(const StringName &p_method) const {
	if (type == OBJECT) {
		Object *obj = get_validated_object();
		if (!obj) {
			return false;
		}

		return obj->has_method(p_method);
	}

	return builtin_method_info[type].has(p_method);
}

bool Variant::has_builtin_method(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	return builtin_method_info[p_type].has(p_method);
}

Variant::ValidatedBuiltInMethod Variant::get_validated_builtin_method(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, nullptr);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, nullptr);
	return method->validated_call;
}

Variant::PTRBuiltInMethod Variant::get_ptr_builtin_method(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, nullptr);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, nullptr);
	return method->ptrcall;
}

int Variant::get_builtin_method_argument_count(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, 0);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, 0);
	return method->argument_count;
}

Variant::Type Variant::get_builtin_method_argument_type(Variant::Type p_type, const StringName &p_method, int p_argument) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, Variant::NIL);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, Variant::NIL);
	ERR_FAIL_INDEX_V(p_argument, method->argument_count, Variant::NIL);
	return method->get_argument_type(p_argument);
}

String Variant::get_builtin_method_argument_name(Variant::Type p_type, const StringName &p_method, int p_argument) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, String());
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, String());
#ifdef DEBUG_METHODS_ENABLED
	ERR_FAIL_INDEX_V(p_argument, method->argument_count, String());
	return method->argument_names[p_argument];
#else
	return "arg" + itos(p_argument + 1);
#endif
}

Vector<Variant> Variant::get_builtin_method_default_arguments(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, Vector<Variant>());
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, Vector<Variant>());
	return method->default_arguments;
}

bool Variant::has_builtin_method_return_value(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, false);
	return method->has_return_type;
}

void Variant::get_builtin_method_list(Variant::Type p_type, List<StringName> *p_list) {
	ERR_FAIL_INDEX(p_type, Variant::VARIANT_MAX);
	for (const StringName &E : builtin_method_names[p_type]) {
		p_list->push_back(E);
	}
}

int Variant::get_builtin_method_count(Variant::Type p_type) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, -1);
	return builtin_method_names[p_type].size();
}

Variant::Type Variant::get_builtin_method_return_type(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, Variant::NIL);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, Variant::NIL);
	return method->return_type;
}

bool Variant::is_builtin_method_const(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, false);
	return method->is_const;
}

bool Variant::is_builtin_method_static(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, false);
	return method->is_static;
}

bool Variant::is_builtin_method_vararg(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, false);
	return method->is_vararg;
}

uint32_t Variant::get_builtin_method_hash(Variant::Type p_type, const StringName &p_method) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, 0);
	const VariantBuiltInMethodInfo *method = builtin_method_info[p_type].lookup_ptr(p_method);
	ERR_FAIL_COND_V(!method, 0);
	uint32_t hash = hash_djb2_one_32(method->is_const);
	hash = hash_djb2_one_32(method->is_static, hash);
	hash = hash_djb2_one_32(method->is_vararg, hash);
	hash = hash_djb2_one_32(method->has_return_type, hash);
	if (method->has_return_type) {
		hash = hash_djb2_one_32(method->return_type, hash);
	}
	hash = hash_djb2_one_32(method->argument_count, hash);
	for (int i = 0; i < method->argument_count; i++) {
		hash = method->get_argument_type(i);
	}

	return hash;
}

void Variant::get_method_list(List<MethodInfo> *p_list) const {
	if (type == OBJECT) {
		Object *obj = get_validated_object();
		if (obj) {
			obj->get_method_list(p_list);
		}
	} else {
		for (const StringName &E : builtin_method_names[type]) {
			const VariantBuiltInMethodInfo *method = builtin_method_info[type].lookup_ptr(E);
			ERR_CONTINUE(!method);

			MethodInfo mi;
			mi.name = E;

			//return type
			if (method->has_return_type) {
				mi.return_val.type = method->return_type;
				if (mi.return_val.type == Variant::NIL) {
					mi.return_val.usage |= PROPERTY_USAGE_NIL_IS_VARIANT;
				}
			}

			if (method->is_const) {
				mi.flags |= METHOD_FLAG_CONST;
			}
			if (method->is_vararg) {
				mi.flags |= METHOD_FLAG_VARARG;
			}
			if (method->is_static) {
				mi.flags |= METHOD_FLAG_STATIC;
			}
			for (int i = 0; i < method->argument_count; i++) {
				PropertyInfo pi;
#ifdef DEBUG_METHODS_ENABLED
				pi.name = method->argument_names[i];
#else
				pi.name = "arg" + itos(i + 1);
#endif
				pi.type = method->get_argument_type(i);
				if (pi.type == Variant::NIL) {
					pi.usage |= PROPERTY_USAGE_NIL_IS_VARIANT;
				}
				mi.arguments.push_back(pi);
			}

			mi.default_arguments = method->default_arguments;
			p_list->push_back(mi);
		}
	}
}

void Variant::get_constants_for_type(Variant::Type p_type, List<StringName> *p_constants) {
	ERR_FAIL_INDEX(p_type, Variant::VARIANT_MAX);

	_VariantCall::ConstantData &cd = _VariantCall::constant_data[p_type];

#ifdef DEBUG_ENABLED
	for (List<StringName>::Element *E = cd.value_ordered.front(); E; E = E->next()) {
		p_constants->push_back(E->get());
#else
	for (const KeyValue<StringName, int> &E : cd.value) {
		p_constants->push_back(E.key);
#endif
	}

#ifdef DEBUG_ENABLED
	for (List<StringName>::Element *E = cd.variant_value_ordered.front(); E; E = E->next()) {
		p_constants->push_back(E->get());
#else
	for (const KeyValue<StringName, Variant> &E : cd.variant_value) {
		p_constants->push_back(E.key);
#endif
	}
}

int Variant::get_constants_count_for_type(Variant::Type p_type) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, -1);
	_VariantCall::ConstantData &cd = _VariantCall::constant_data[p_type];

	return cd.value.size() + cd.variant_value.size();
}

bool Variant::has_constant(Variant::Type p_type, const StringName &p_value) {
	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, false);
	_VariantCall::ConstantData &cd = _VariantCall::constant_data[p_type];
	return cd.value.has(p_value) || cd.variant_value.has(p_value);
}

Variant Variant::get_constant_value(Variant::Type p_type, const StringName &p_value, bool *r_valid) {
	if (r_valid) {
		*r_valid = false;
	}

	ERR_FAIL_INDEX_V(p_type, Variant::VARIANT_MAX, 0);
	_VariantCall::ConstantData &cd = _VariantCall::constant_data[p_type];

	Map<StringName, int>::Element *E = cd.value.find(p_value);
	if (!E) {
		Map<StringName, Variant>::Element *F = cd.variant_value.find(p_value);
		if (F) {
			if (r_valid) {
				*r_valid = true;
			}
			return F->get();
		} else {
			return -1;
		}
	}
	if (r_valid) {
		*r_valid = true;
	}

	return E->get();
}

#ifdef DEBUG_METHODS_ENABLED
#define bind_method(m_type, m_method, m_arg_names, m_default_args) \
	METHOD_CLASS(m_type, m_method, &m_type::m_method);             \
	register_builtin_method<Method_##m_type##_##m_method>(m_arg_names, m_default_args);
#else
#define bind_method(m_type, m_method, m_arg_names, m_default_args) \
	METHOD_CLASS(m_type, m_method, &m_type ::m_method);            \
	register_builtin_method<Method_##m_type##_##m_method>(sarray(), m_default_args);
#endif

#ifdef DEBUG_METHODS_ENABLED
#define bind_static_method(m_type, m_method, m_arg_names, m_default_args) \
	STATIC_METHOD_CLASS(m_type, m_method, m_type::m_method);              \
	register_builtin_method<Method_##m_type##_##m_method>(m_arg_names, m_default_args);
#else
#define bind_static_method(m_type, m_method, m_arg_names, m_default_args) \
	STATIC_METHOD_CLASS(m_type, m_method, m_type ::m_method);             \
	register_builtin_method<Method_##m_type##_##m_method>(sarray(), m_default_args);
#endif

#ifdef DEBUG_METHODS_ENABLED
#define bind_methodv(m_type, m_name, m_method, m_arg_names, m_default_args) \
	METHOD_CLASS(m_type, m_name, m_method);                                 \
	register_builtin_method<Method_##m_type##_##m_name>(m_arg_names, m_default_args);
#else
#define bind_methodv(m_type, m_name, m_method, m_arg_names, m_default_args) \
	METHOD_CLASS(m_type, m_name, m_method);                                 \
	register_builtin_method<Method_##m_type##_##m_name>(sarray(), m_default_args);
#endif

#ifdef DEBUG_METHODS_ENABLED
#define bind_function(m_type, m_name, m_method, m_arg_names, m_default_args) \
	FUNCTION_CLASS(m_type, m_name, m_method, true);                          \
	register_builtin_method<Method_##m_type##_##m_name>(m_arg_names, m_default_args);
#else
#define bind_function(m_type, m_name, m_method, m_arg_names, m_default_args) \
	FUNCTION_CLASS(m_type, m_name, m_method, true);                          \
	register_builtin_method<Method_##m_type##_##m_name>(sarray(), m_default_args);
#endif

#ifdef DEBUG_METHODS_ENABLED
#define bind_functionnc(m_type, m_name, m_method, m_arg_names, m_default_args) \
	FUNCTION_CLASS(m_type, m_name, m_method, false);                           \
	register_builtin_method<Method_##m_type##_##m_name>(m_arg_names, m_default_args);
#else
#define bind_functionnc(m_type, m_name, m_method, m_arg_names, m_default_args) \
	FUNCTION_CLASS(m_type, m_name, m_method, false);                           \
	register_builtin_method<Method_##m_type##_##m_name>(sarray(), m_default_args);
#endif

#define bind_custom(m_type, m_name, m_method, m_has_return, m_ret_type) \
	VARARG_CLASS(m_type, m_name, m_method, m_has_return, m_ret_type)    \
	register_builtin_method<Method_##m_type##_##m_name>(sarray(), Vector<Variant>());

#define bind_custom1(m_type, m_name, m_method, m_arg_type, m_arg_name) \
	VARARG_CLASS1(m_type, m_name, m_method, m_arg_type)                \
	register_builtin_method<Method_##m_type##_##m_name>(sarray(m_arg_name), Vector<Variant>());

static void _register_variant_builtin_methods() {
	_VariantCall::constant_data = memnew_arr(_VariantCall::ConstantData, Variant::VARIANT_MAX);
	builtin_method_info = memnew_arr(BuiltinMethodMap, Variant::VARIANT_MAX);
	builtin_method_names = memnew_arr(List<StringName>, Variant::VARIANT_MAX);

	/* String */

	bind_method(String, casecmp_to, sarray("to"), varray());
	bind_method(String, nocasecmp_to, sarray("to"), varray());
	bind_method(String, naturalnocasecmp_to, sarray("to"), varray());
	bind_method(String, length, sarray(), varray());
	bind_method(String, substr, sarray("from", "len"), varray(-1));
	bind_method(String, get_slice, sarray("delimiter", "slice"), varray());
	bind_methodv(String, find, static_cast<int (String::*)(const String &, int) const>(&String::find), sarray("what", "from"), varray(0));
	bind_method(String, count, sarray("what", "from", "to"), varray(0, 0));
	bind_method(String, countn, sarray("what", "from", "to"), varray(0, 0));
	bind_method(String, findn, sarray("what", "from"), varray(0));
	bind_method(String, rfind, sarray("what", "from"), varray(-1));
	bind_method(String, rfindn, sarray("what", "from"), varray(-1));
	bind_method(String, match, sarray("expr"), varray());
	bind_method(String, matchn, sarray("expr"), varray());
	bind_methodv(String, begins_with, static_cast<bool (String::*)(const String &) const>(&String::begins_with), sarray("text"), varray());
	bind_method(String, ends_with, sarray("text"), varray());
	bind_method(String, is_subsequence_of, sarray("text"), varray());
	bind_method(String, is_subsequence_ofn, sarray("text"), varray());
	bind_method(String, bigrams, sarray(), varray());
	bind_method(String, similarity, sarray("text"), varray());

	bind_method(String, format, sarray("values", "placeholder"), varray("{_}"));
	bind_methodv(String, replace, static_cast<String (String::*)(const String &, const String &) const>(&String::replace), sarray("what", "forwhat"), varray());
	bind_method(String, replacen, sarray("what", "forwhat"), varray());
	bind_method(String, repeat, sarray("count"), varray());
	bind_method(String, insert, sarray("position", "what"), varray());
	bind_method(String, capitalize, sarray(), varray());
	bind_method(String, split, sarray("delimiter", "allow_empty", "maxsplit"), varray(true, 0));
	bind_method(String, rsplit, sarray("delimiter", "allow_empty", "maxsplit"), varray(true, 0));
	bind_method(String, split_floats, sarray("delimiter", "allow_empty"), varray(true));
	bind_method(String, join, sarray("parts"), varray());

	bind_method(String, to_upper, sarray(), varray());
	bind_method(String, to_lower, sarray(), varray());

	bind_method(String, left, sarray("position"), varray());
	bind_method(String, right, sarray("position"), varray());

	bind_method(String, strip_edges, sarray("left", "right"), varray(true, true));
	bind_method(String, strip_escapes, sarray(), varray());
	bind_method(String, lstrip, sarray("chars"), varray());
	bind_method(String, rstrip, sarray("chars"), varray());
	bind_method(String, get_extension, sarray(), varray());
	bind_method(String, get_basename, sarray(), varray());
	bind_method(String, plus_file, sarray("file"), varray());
	bind_method(String, unicode_at, sarray("at"), varray());
	bind_method(String, indent, sarray("prefix"), varray());
	bind_method(String, dedent, sarray(), varray());
	bind_method(String, hash, sarray(), varray());
	bind_method(String, md5_text, sarray(), varray());
	bind_method(String, sha1_text, sarray(), varray());
	bind_method(String, sha256_text, sarray(), varray());
	bind_method(String, md5_buffer, sarray(), varray());
	bind_method(String, sha1_buffer, sarray(), varray());
	bind_method(String, sha256_buffer, sarray(), varray());
	bind_method(String, is_empty, sarray(), varray());
	bind_methodv(String, contains, static_cast<bool (String::*)(const String &) const>(&String::contains), sarray("what"), varray());

	bind_method(String, is_absolute_path, sarray(), varray());
	bind_method(String, is_relative_path, sarray(), varray());
	bind_method(String, simplify_path, sarray(), varray());
	bind_method(String, get_base_dir, sarray(), varray());
	bind_method(String, get_file, sarray(), varray());
	bind_method(String, xml_escape, sarray("escape_quotes"), varray(false));
	bind_method(String, xml_unescape, sarray(), varray());
	bind_method(String, uri_encode, sarray(), varray());
	bind_method(String, uri_decode, sarray(), varray());
	bind_method(String, c_escape, sarray(), varray());
	bind_method(String, c_unescape, sarray(), varray());
	bind_method(String, json_escape, sarray(), varray());

	bind_method(String, validate_node_name, sarray(), varray());

	bind_method(String, is_valid_identifier, sarray(), varray());
	bind_method(String, is_valid_int, sarray(), varray());
	bind_method(String, is_valid_float, sarray(), varray());
	bind_method(String, is_valid_hex_number, sarray("with_prefix"), varray(false));
	bind_method(String, is_valid_html_color, sarray(), varray());
	bind_method(String, is_valid_ip_address, sarray(), varray());
	bind_method(String, is_valid_filename, sarray(), varray());

	bind_method(String, to_int, sarray(), varray());
	bind_method(String, to_float, sarray(), varray());
	bind_method(String, hex_to_int, sarray(), varray());
	bind_method(String, bin_to_int, sarray(), varray());

	bind_method(String, lpad, sarray("min_length", "character"), varray(" "));
	bind_method(String, rpad, sarray("min_length", "character"), varray(" "));
	bind_method(String, pad_decimals, sarray("digits"), varray());
	bind_method(String, pad_zeros, sarray("digits"), varray());
	bind_method(String, trim_prefix, sarray("prefix"), varray());
	bind_method(String, trim_suffix, sarray("suffix"), varray());

	bind_method(String, to_ascii_buffer, sarray(), varray());
	bind_method(String, to_utf8_buffer, sarray(), varray());
	bind_method(String, to_utf16_buffer, sarray(), varray());
	bind_method(String, to_utf32_buffer, sarray(), varray());

	bind_static_method(String, num_scientific, sarray("number"), varray());
	bind_static_method(String, num, sarray("number", "decimals"), varray(-1));
	bind_static_method(String, chr, sarray("char"), varray());
	bind_static_method(String, humanize_size, sarray("size"), varray());

	/* Vector2 */

	bind_method(Vector2, angle, sarray(), varray());
	bind_method(Vector2, angle_to, sarray("to"), varray());
	bind_method(Vector2, angle_to_point, sarray("to"), varray());
	bind_method(Vector2, direction_to, sarray("to"), varray());
	bind_method(Vector2, distance_to, sarray("to"), varray());
	bind_method(Vector2, distance_squared_to, sarray("to"), varray());
	bind_method(Vector2, length, sarray(), varray());
	bind_method(Vector2, length_squared, sarray(), varray());
	bind_method(Vector2, limit_length, sarray("length"), varray(1.0));
	bind_method(Vector2, normalized, sarray(), varray());
	bind_method(Vector2, is_normalized, sarray(), varray());
	bind_method(Vector2, is_equal_approx, sarray("to"), varray());
	bind_method(Vector2, posmod, sarray("mod"), varray());
	bind_method(Vector2, posmodv, sarray("modv"), varray());
	bind_method(Vector2, project, sarray("b"), varray());
	bind_method(Vector2, lerp, sarray("to", "weight"), varray());
	bind_method(Vector2, slerp, sarray("to", "weight"), varray());
	bind_method(Vector2, cubic_interpolate, sarray("b", "pre_a", "post_b", "weight"), varray());
	bind_method(Vector2, max_axis_index, sarray(), varray());
	bind_method(Vector2, min_axis_index, sarray(), varray());
	bind_method(Vector2, move_toward, sarray("to", "delta"), varray());
	bind_method(Vector2, rotated, sarray("phi"), varray());
	bind_method(Vector2, orthogonal, sarray(), varray());
	bind_method(Vector2, floor, sarray(), varray());
	bind_method(Vector2, ceil, sarray(), varray());
	bind_method(Vector2, round, sarray(), varray());
	bind_method(Vector2, aspect, sarray(), varray());
	bind_method(Vector2, dot, sarray("with"), varray());
	bind_method(Vector2, slide, sarray("n"), varray());
	bind_method(Vector2, bounce, sarray("n"), varray());
	bind_method(Vector2, reflect, sarray("n"), varray());
	bind_method(Vector2, cross, sarray("with"), varray());
	bind_method(Vector2, abs, sarray(), varray());
	bind_method(Vector2, sign, sarray(), varray());
	bind_method(Vector2, clamp, sarray("min", "max"), varray());
	bind_method(Vector2, snapped, sarray("step"), varray());

	bind_static_method(Vector2, from_angle, sarray("angle"), varray());

	/* Vector2i */

	bind_method(Vector2i, aspect, sarray(), varray());
	bind_method(Vector2i, max_axis_index, sarray(), varray());
	bind_method(Vector2i, min_axis_index, sarray(), varray());
	bind_method(Vector2i, length, sarray(), varray());
	bind_method(Vector2i, length_squared, sarray(), varray());
	bind_method(Vector2i, sign, sarray(), varray());
	bind_method(Vector2i, abs, sarray(), varray());
	bind_method(Vector2i, clamp, sarray("min", "max"), varray());

	/* Rect2 */

	bind_method(Rect2, get_center, sarray(), varray());
	bind_method(Rect2, get_area, sarray(), varray());
	bind_method(Rect2, has_no_area, sarray(), varray());
	bind_method(Rect2, has_point, sarray("point"), varray());
	bind_method(Rect2, is_equal_approx, sarray("rect"), varray());
	bind_method(Rect2, intersects, sarray("b", "include_borders"), varray(false));
	bind_method(Rect2, encloses, sarray("b"), varray());
	bind_method(Rect2, intersection, sarray("b"), varray());
	bind_method(Rect2, merge, sarray("b"), varray());
	bind_method(Rect2, expand, sarray("to"), varray());
	bind_method(Rect2, grow, sarray("amount"), varray());
	bind_methodv(Rect2, grow_side, &Rect2::grow_side_bind, sarray("side", "amount"), varray());
	bind_method(Rect2, grow_individual, sarray("left", "top", "right", "bottom"), varray());
	bind_method(Rect2, abs, sarray(), varray());

	/* Rect2i */

	bind_method(Rect2i, get_center, sarray(), varray());
	bind_method(Rect2i, get_area, sarray(), varray());
	bind_method(Rect2i, has_no_area, sarray(), varray());
	bind_method(Rect2i, has_point, sarray("point"), varray());
	bind_method(Rect2i, intersects, sarray("b"), varray());
	bind_method(Rect2i, encloses, sarray("b"), varray());
	bind_method(Rect2i, intersection, sarray("b"), varray());
	bind_method(Rect2i, merge, sarray("b"), varray());
	bind_method(Rect2i, expand, sarray("to"), varray());
	bind_method(Rect2i, grow, sarray("amount"), varray());
	bind_methodv(Rect2i, grow_side, &Rect2i::grow_side_bind, sarray("side", "amount"), varray());
	bind_method(Rect2i, grow_individual, sarray("left", "top", "right", "bottom"), varray());
	bind_method(Rect2i, abs, sarray(), varray());

	/* Vector3 */

	bind_method(Vector3, min_axis_index, sarray(), varray());
	bind_method(Vector3, max_axis_index, sarray(), varray());
	bind_method(Vector3, angle_to, sarray("to"), varray());
	bind_method(Vector3, signed_angle_to, sarray("to", "axis"), varray());
	bind_method(Vector3, direction_to, sarray("to"), varray());
	bind_method(Vector3, distance_to, sarray("to"), varray());
	bind_method(Vector3, distance_squared_to, sarray("to"), varray());
	bind_method(Vector3, length, sarray(), varray());
	bind_method(Vector3, length_squared, sarray(), varray());
	bind_method(Vector3, limit_length, sarray("length"), varray(1.0));
	bind_method(Vector3, normalized, sarray(), varray());
	bind_method(Vector3, is_normalized, sarray(), varray());
	bind_method(Vector3, is_equal_approx, sarray("to"), varray());
	bind_method(Vector3, inverse, sarray(), varray());
	bind_method(Vector3, clamp, sarray("min", "max"), varray());
	bind_method(Vector3, snapped, sarray("step"), varray());
	bind_method(Vector3, rotated, sarray("by_axis", "phi"), varray());
	bind_method(Vector3, lerp, sarray("to", "weight"), varray());
	bind_method(Vector3, slerp, sarray("to", "weight"), varray());
	bind_method(Vector3, cubic_interpolate, sarray("b", "pre_a", "post_b", "weight"), varray());
	bind_method(Vector3, move_toward, sarray("to", "delta"), varray());
	bind_method(Vector3, dot, sarray("with"), varray());
	bind_method(Vector3, cross, sarray("with"), varray());
	bind_method(Vector3, outer, sarray("with"), varray());
	bind_method(Vector3, abs, sarray(), varray());
	bind_method(Vector3, floor, sarray(), varray());
	bind_method(Vector3, ceil, sarray(), varray());
	bind_method(Vector3, round, sarray(), varray());
	bind_method(Vector3, posmod, sarray("mod"), varray());
	bind_method(Vector3, posmodv, sarray("modv"), varray());
	bind_method(Vector3, project, sarray("b"), varray());
	bind_method(Vector3, slide, sarray("n"), varray());
	bind_method(Vector3, bounce, sarray("n"), varray());
	bind_method(Vector3, reflect, sarray("n"), varray());
	bind_method(Vector3, sign, sarray(), varray());
	bind_method(Vector3, octahedron_encode, sarray(), varray());
	bind_static_method(Vector3, octahedron_decode, sarray("uv"), varray());

	/* Vector3i */

	bind_method(Vector3i, min_axis_index, sarray(), varray());
	bind_method(Vector3i, max_axis_index, sarray(), varray());
	bind_method(Vector3i, length, sarray(), varray());
	bind_method(Vector3i, length_squared, sarray(), varray());
	bind_method(Vector3i, sign, sarray(), varray());
	bind_method(Vector3i, abs, sarray(), varray());
	bind_method(Vector3i, clamp, sarray("min", "max"), varray());

	/* Plane */

	bind_method(Plane, normalized, sarray(), varray());
	bind_method(Plane, center, sarray(), varray());
	bind_method(Plane, is_equal_approx, sarray("to_plane"), varray());
	bind_method(Plane, is_point_over, sarray("plane"), varray());
	bind_method(Plane, distance_to, sarray("point"), varray());
	bind_method(Plane, has_point, sarray("point", "epsilon"), varray(CMP_EPSILON));
	bind_method(Plane, project, sarray("point"), varray());
	bind_methodv(Plane, intersect_3, &Plane::intersect_3_bind, sarray("b", "c"), varray());
	bind_methodv(Plane, intersects_ray, &Plane::intersects_ray_bind, sarray("from", "dir"), varray());
	bind_methodv(Plane, intersects_segment, &Plane::intersects_segment_bind, sarray("from", "to"), varray());

	/* Quaternion */

	bind_method(Quaternion, length, sarray(), varray());
	bind_method(Quaternion, length_squared, sarray(), varray());
	bind_method(Quaternion, normalized, sarray(), varray());
	bind_method(Quaternion, is_normalized, sarray(), varray());
	bind_method(Quaternion, is_equal_approx, sarray("to"), varray());
	bind_method(Quaternion, inverse, sarray(), varray());
	bind_method(Quaternion, angle_to, sarray("to"), varray());
	bind_method(Quaternion, dot, sarray("with"), varray());
	bind_method(Quaternion, slerp, sarray("to", "weight"), varray());
	bind_method(Quaternion, slerpni, sarray("to", "weight"), varray());
	bind_method(Quaternion, cubic_slerp, sarray("b", "pre_a", "post_b", "weight"), varray());
	bind_method(Quaternion, get_euler, sarray(), varray());
	bind_method(Quaternion, get_axis, sarray(), varray());
	bind_method(Quaternion, get_angle, sarray(), varray());

	/* Color */

	bind_method(Color, to_argb32, sarray(), varray());
	bind_method(Color, to_abgr32, sarray(), varray());
	bind_method(Color, to_rgba32, sarray(), varray());
	bind_method(Color, to_argb64, sarray(), varray());
	bind_method(Color, to_abgr64, sarray(), varray());
	bind_method(Color, to_rgba64, sarray(), varray());
	bind_method(Color, to_html, sarray("with_alpha"), varray(true));

	bind_method(Color, clamp, sarray("min", "max"), varray(Color(0, 0, 0, 0), Color(1, 1, 1, 1)));
	bind_method(Color, inverted, sarray(), varray());
	bind_method(Color, lerp, sarray("to", "weight"), varray());
	bind_method(Color, lightened, sarray("amount"), varray());
	bind_method(Color, darkened, sarray("amount"), varray());
	bind_method(Color, blend, sarray("over"), varray());
	bind_method(Color, get_luminance, sarray(), varray());

	bind_method(Color, is_equal_approx, sarray("to"), varray());

	bind_static_method(Color, hex, sarray("hex"), varray());
	bind_static_method(Color, hex64, sarray("hex"), varray());
	bind_static_method(Color, html, sarray("rgba"), varray());
	bind_static_method(Color, html_is_valid, sarray("color"), varray());
	bind_static_method(Color, find_named_color, sarray("name"), varray());
	bind_static_method(Color, get_named_color_count, sarray(), varray());
	bind_static_method(Color, get_named_color_name, sarray("idx"), varray());
	bind_static_method(Color, get_named_color, sarray("idx"), varray());
	bind_static_method(Color, from_string, sarray("str", "default"), varray());
	bind_static_method(Color, from_hsv, sarray("h", "s", "v", "alpha"), varray(1.0));
	bind_static_method(Color, from_rgbe9995, sarray("rgbe"), varray());

	/* RID */

	bind_method(RID, get_id, sarray(), varray());

	/* NodePath */

	bind_method(NodePath, is_absolute, sarray(), varray());
	bind_method(NodePath, get_name_count, sarray(), varray());
	bind_method(NodePath, get_name, sarray("idx"), varray());
	bind_method(NodePath, get_subname_count, sarray(), varray());
	bind_method(NodePath, get_subname, sarray("idx"), varray());
	bind_method(NodePath, get_concatenated_subnames, sarray(), varray());
	bind_method(NodePath, get_as_property_path, sarray(), varray());
	bind_method(NodePath, is_empty, sarray(), varray());

	/* Callable */

	bind_method(Callable, is_null, sarray(), varray());
	bind_method(Callable, is_custom, sarray(), varray());
	bind_method(Callable, is_standard, sarray(), varray());
	bind_method(Callable, is_valid, sarray(), varray());
	bind_method(Callable, get_object, sarray(), varray());
	bind_method(Callable, get_object_id, sarray(), varray());
	bind_method(Callable, get_method, sarray(), varray());
	bind_method(Callable, hash, sarray(), varray());
	bind_method(Callable, unbind, sarray("argcount"), varray());

	bind_custom(Callable, call, _VariantCall::func_Callable_call, true, Variant);
	bind_custom(Callable, call_deferred, _VariantCall::func_Callable_call_deferred, false, Variant);
	bind_custom(Callable, rpc, _VariantCall::func_Callable_rpc, false, Variant);
	bind_custom1(Callable, rpc_id, _VariantCall::func_Callable_rpc_id, Variant::INT, "peer_id");
	bind_custom(Callable, bind, _VariantCall::func_Callable_bind, true, Callable);

	/* Signal */

	bind_method(Signal, is_null, sarray(), varray());
	bind_method(Signal, get_object, sarray(), varray());
	bind_method(Signal, get_object_id, sarray(), varray());
	bind_method(Signal, get_name, sarray(), varray());

	bind_method(Signal, connect, sarray("callable", "flags"), varray(0));
	bind_method(Signal, disconnect, sarray("callable"), varray());
	bind_method(Signal, is_connected, sarray("callable"), varray());
	bind_method(Signal, get_connections, sarray(), varray());

	bind_custom(Signal, emit, _VariantCall::func_Signal_emit, false, Variant);

	/* Transform2D */

	bind_method(Transform2D, inverse, sarray(), varray());
	bind_method(Transform2D, affine_inverse, sarray(), varray());
	bind_method(Transform2D, get_rotation, sarray(), varray());
	bind_method(Transform2D, get_origin, sarray(), varray());
	bind_method(Transform2D, get_scale, sarray(), varray());
	bind_method(Transform2D, get_skew, sarray(), varray());
	bind_method(Transform2D, orthonormalized, sarray(), varray());
	bind_method(Transform2D, rotated, sarray("phi"), varray());
	bind_method(Transform2D, scaled, sarray("scale"), varray());
	bind_method(Transform2D, translated, sarray("offset"), varray());
	bind_method(Transform2D, basis_xform, sarray("v"), varray());
	bind_method(Transform2D, basis_xform_inv, sarray("v"), varray());
	bind_method(Transform2D, interpolate_with, sarray("xform", "weight"), varray());
	bind_method(Transform2D, is_equal_approx, sarray("xform"), varray());
	bind_method(Transform2D, set_rotation, sarray("rotation"), varray());
	bind_method(Transform2D, set_scale, sarray("scale"), varray());
	bind_method(Transform2D, set_skew, sarray("skew"), varray());
	bind_method(Transform2D, looking_at, sarray("target"), varray(Vector2()));

	/* Basis */

	bind_method(Basis, inverse, sarray(), varray());
	bind_method(Basis, transposed, sarray(), varray());
	bind_method(Basis, orthonormalized, sarray(), varray());
	bind_method(Basis, determinant, sarray(), varray());
	bind_methodv(Basis, rotated, static_cast<Basis (Basis::*)(const Vector3 &, real_t) const>(&Basis::rotated), sarray("axis", "phi"), varray());
	bind_method(Basis, scaled, sarray("scale"), varray());
	bind_method(Basis, get_scale, sarray(), varray());
	bind_method(Basis, get_euler, sarray("order"), varray(Basis::EULER_ORDER_YXZ));
	bind_method(Basis, tdotx, sarray("with"), varray());
	bind_method(Basis, tdoty, sarray("with"), varray());
	bind_method(Basis, tdotz, sarray("with"), varray());
	bind_method(Basis, get_orthogonal_index, sarray(), varray());
	bind_method(Basis, slerp, sarray("to", "weight"), varray());
	bind_method(Basis, is_equal_approx, sarray("b"), varray());
	bind_method(Basis, get_rotation_quaternion, sarray(), varray());
	bind_static_method(Basis, looking_at, sarray("target", "up"), varray(Vector3(0, 1, 0)));
	bind_static_method(Basis, from_scale, sarray("scale"), varray());
	bind_static_method(Basis, from_euler, sarray("euler", "order"), varray(Basis::EULER_ORDER_YXZ));

	/* AABB */

	bind_method(AABB, abs, sarray(), varray());
	bind_method(AABB, get_center, sarray(), varray());
	bind_method(AABB, get_volume, sarray(), varray());
	bind_method(AABB, has_no_volume, sarray(), varray());
	bind_method(AABB, has_no_surface, sarray(), varray());
	bind_method(AABB, has_point, sarray("point"), varray());
	bind_method(AABB, is_equal_approx, sarray("aabb"), varray());
	bind_method(AABB, intersects, sarray("with"), varray());
	bind_method(AABB, encloses, sarray("with"), varray());
	bind_method(AABB, intersects_plane, sarray("plane"), varray());
	bind_method(AABB, intersection, sarray("with"), varray());
	bind_method(AABB, merge, sarray("with"), varray());
	bind_method(AABB, expand, sarray("to_point"), varray());
	bind_method(AABB, grow, sarray("by"), varray());
	bind_method(AABB, get_support, sarray("dir"), varray());
	bind_method(AABB, get_longest_axis, sarray(), varray());
	bind_method(AABB, get_longest_axis_index, sarray(), varray());
	bind_method(AABB, get_longest_axis_size, sarray(), varray());
	bind_method(AABB, get_shortest_axis, sarray(), varray());
	bind_method(AABB, get_shortest_axis_index, sarray(), varray());
	bind_method(AABB, get_shortest_axis_size, sarray(), varray());
	bind_method(AABB, get_endpoint, sarray("idx"), varray());
	bind_methodv(AABB, intersects_segment, &AABB::intersects_segment_bind, sarray("from", "to"), varray());
	bind_methodv(AABB, intersects_ray, &AABB::intersects_ray_bind, sarray("from", "dir"), varray());

	/* Transform3D */

	bind_method(Transform3D, inverse, sarray(), varray());
	bind_method(Transform3D, affine_inverse, sarray(), varray());
	bind_method(Transform3D, orthonormalized, sarray(), varray());
	bind_method(Transform3D, rotated, sarray("axis", "phi"), varray());
	bind_method(Transform3D, scaled, sarray("scale"), varray());
	bind_method(Transform3D, translated, sarray("offset"), varray());
	bind_method(Transform3D, looking_at, sarray("target", "up"), varray(Vector3(0, 1, 0)));
	bind_method(Transform3D, sphere_interpolate_with, sarray("xform", "weight"), varray());
	bind_method(Transform3D, interpolate_with, sarray("xform", "weight"), varray());
	bind_method(Transform3D, is_equal_approx, sarray("xform"), varray());

	/* Dictionary */

	bind_method(Dictionary, size, sarray(), varray());
	bind_method(Dictionary, is_empty, sarray(), varray());
	bind_method(Dictionary, clear, sarray(), varray());
	bind_method(Dictionary, has, sarray("key"), varray());
	bind_method(Dictionary, has_all, sarray("keys"), varray());
	bind_method(Dictionary, erase, sarray("key"), varray());
	bind_method(Dictionary, hash, sarray(), varray());
	bind_method(Dictionary, keys, sarray(), varray());
	bind_method(Dictionary, values, sarray(), varray());
	bind_method(Dictionary, duplicate, sarray("deep"), varray(false));
	bind_method(Dictionary, get, sarray("key", "default"), varray(Variant()));

	/* Array */

	bind_method(Array, size, sarray(), varray());
	bind_method(Array, is_empty, sarray(), varray());
	bind_method(Array, clear, sarray(), varray());
	bind_method(Array, hash, sarray(), varray());
	bind_method(Array, push_back, sarray("value"), varray());
	bind_method(Array, push_front, sarray("value"), varray());
	bind_method(Array, append, sarray("value"), varray());
	bind_method(Array, append_array, sarray("array"), varray());
	bind_method(Array, resize, sarray("size"), varray());
	bind_method(Array, insert, sarray("position", "value"), varray());
	bind_method(Array, remove_at, sarray("position"), varray());
	bind_method(Array, fill, sarray("value"), varray());
	bind_method(Array, erase, sarray("value"), varray());
	bind_method(Array, front, sarray(), varray());
	bind_method(Array, back, sarray(), varray());
	bind_method(Array, find, sarray("what", "from"), varray(0));
	bind_method(Array, rfind, sarray("what", "from"), varray(-1));
	bind_method(Array, find_last, sarray("value"), varray());
	bind_method(Array, count, sarray("value"), varray());
	bind_method(Array, has, sarray("value"), varray());
	bind_method(Array, pop_back, sarray(), varray());
	bind_method(Array, pop_front, sarray(), varray());
	bind_method(Array, pop_at, sarray("position"), varray());
	bind_method(Array, sort, sarray(), varray());
	bind_method(Array, sort_custom, sarray("func"), varray());
	bind_method(Array, shuffle, sarray(), varray());
	bind_method(Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(Array, bsearch_custom, sarray("value", "func", "before"), varray(true));
	bind_method(Array, reverse, sarray(), varray());
	bind_method(Array, duplicate, sarray("deep"), varray(false));
	bind_method(Array, slice, sarray("begin", "end", "step", "deep"), varray(INT_MAX, 1, false));
	bind_method(Array, filter, sarray("method"), varray());
	bind_method(Array, map, sarray("method"), varray());
	bind_method(Array, reduce, sarray("method", "accum"), varray(Variant()));
	bind_method(Array, max, sarray(), varray());
	bind_method(Array, min, sarray(), varray());

	/* Byte Array */
	bind_method(PackedByteArray, size, sarray(), varray());
	bind_method(PackedByteArray, is_empty, sarray(), varray());
	bind_method(PackedByteArray, set, sarray("index", "value"), varray());
	bind_method(PackedByteArray, push_back, sarray("value"), varray());
	bind_method(PackedByteArray, append, sarray("value"), varray());
	bind_method(PackedByteArray, append_array, sarray("array"), varray());
	bind_method(PackedByteArray, remove_at, sarray("index"), varray());
	bind_method(PackedByteArray, insert, sarray("at_index", "value"), varray());
	bind_method(PackedByteArray, fill, sarray("value"), varray());
	bind_method(PackedByteArray, resize, sarray("new_size"), varray());
	bind_method(PackedByteArray, has, sarray("value"), varray());
	bind_method(PackedByteArray, reverse, sarray(), varray());
	bind_method(PackedByteArray, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedByteArray, sort, sarray(), varray());
	bind_method(PackedByteArray, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedByteArray, duplicate, sarray(), varray());

	bind_function(PackedByteArray, get_string_from_ascii, _VariantCall::func_PackedByteArray_get_string_from_ascii, sarray(), varray());
	bind_function(PackedByteArray, get_string_from_utf8, _VariantCall::func_PackedByteArray_get_string_from_utf8, sarray(), varray());
	bind_function(PackedByteArray, get_string_from_utf16, _VariantCall::func_PackedByteArray_get_string_from_utf16, sarray(), varray());
	bind_function(PackedByteArray, get_string_from_utf32, _VariantCall::func_PackedByteArray_get_string_from_utf32, sarray(), varray());
	bind_function(PackedByteArray, hex_encode, _VariantCall::func_PackedByteArray_hex_encode, sarray(), varray());
	bind_function(PackedByteArray, compress, _VariantCall::func_PackedByteArray_compress, sarray("compression_mode"), varray(0));
	bind_function(PackedByteArray, decompress, _VariantCall::func_PackedByteArray_decompress, sarray("buffer_size", "compression_mode"), varray(0));
	bind_function(PackedByteArray, decompress_dynamic, _VariantCall::func_PackedByteArray_decompress_dynamic, sarray("max_output_size", "compression_mode"), varray(0));

	bind_function(PackedByteArray, decode_u8, _VariantCall::func_PackedByteArray_decode_u8, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_s8, _VariantCall::func_PackedByteArray_decode_s8, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_u16, _VariantCall::func_PackedByteArray_decode_u16, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_s16, _VariantCall::func_PackedByteArray_decode_s16, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_u32, _VariantCall::func_PackedByteArray_decode_u32, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_s32, _VariantCall::func_PackedByteArray_decode_s32, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_u64, _VariantCall::func_PackedByteArray_decode_u64, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_s64, _VariantCall::func_PackedByteArray_decode_s64, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_half, _VariantCall::func_PackedByteArray_decode_half, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_float, _VariantCall::func_PackedByteArray_decode_float, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, decode_double, _VariantCall::func_PackedByteArray_decode_double, sarray("byte_offset"), varray());
	bind_function(PackedByteArray, has_encoded_var, _VariantCall::func_PackedByteArray_has_encoded_var, sarray("byte_offset", "allow_objects"), varray(false));
	bind_function(PackedByteArray, decode_var, _VariantCall::func_PackedByteArray_decode_var, sarray("byte_offset", "allow_objects"), varray(false));
	bind_function(PackedByteArray, decode_var_size, _VariantCall::func_PackedByteArray_decode_var_size, sarray("byte_offset", "allow_objects"), varray(false));

	bind_function(PackedByteArray, to_int32_array, _VariantCall::func_PackedByteArray_decode_s32_array, sarray(), varray());
	bind_function(PackedByteArray, to_int64_array, _VariantCall::func_PackedByteArray_decode_s64_array, sarray(), varray());
	bind_function(PackedByteArray, to_float32_array, _VariantCall::func_PackedByteArray_decode_float_array, sarray(), varray());
	bind_function(PackedByteArray, to_float64_array, _VariantCall::func_PackedByteArray_decode_double_array, sarray(), varray());

	bind_functionnc(PackedByteArray, encode_u8, _VariantCall::func_PackedByteArray_encode_u8, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_s8, _VariantCall::func_PackedByteArray_encode_s8, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_u16, _VariantCall::func_PackedByteArray_encode_u16, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_s16, _VariantCall::func_PackedByteArray_encode_s16, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_u32, _VariantCall::func_PackedByteArray_encode_u32, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_s32, _VariantCall::func_PackedByteArray_encode_s32, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_u64, _VariantCall::func_PackedByteArray_encode_u64, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_s64, _VariantCall::func_PackedByteArray_encode_s64, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_half, _VariantCall::func_PackedByteArray_encode_half, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_float, _VariantCall::func_PackedByteArray_encode_float, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_double, _VariantCall::func_PackedByteArray_encode_double, sarray("byte_offset", "value"), varray());
	bind_functionnc(PackedByteArray, encode_var, _VariantCall::func_PackedByteArray_encode_var, sarray("byte_offset", "value", "allow_objects"), varray(false));

	/* Int32 Array */

	bind_method(PackedInt32Array, size, sarray(), varray());
	bind_method(PackedInt32Array, is_empty, sarray(), varray());
	bind_method(PackedInt32Array, set, sarray("index", "value"), varray());
	bind_method(PackedInt32Array, push_back, sarray("value"), varray());
	bind_method(PackedInt32Array, append, sarray("value"), varray());
	bind_method(PackedInt32Array, append_array, sarray("array"), varray());
	bind_method(PackedInt32Array, remove_at, sarray("index"), varray());
	bind_method(PackedInt32Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedInt32Array, fill, sarray("value"), varray());
	bind_method(PackedInt32Array, resize, sarray("new_size"), varray());
	bind_method(PackedInt32Array, has, sarray("value"), varray());
	bind_method(PackedInt32Array, reverse, sarray(), varray());
	bind_method(PackedInt32Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedInt32Array, to_byte_array, sarray(), varray());
	bind_method(PackedInt32Array, sort, sarray(), varray());
	bind_method(PackedInt32Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedInt32Array, duplicate, sarray(), varray());

	/* Int64 Array */

	bind_method(PackedInt64Array, size, sarray(), varray());
	bind_method(PackedInt64Array, is_empty, sarray(), varray());
	bind_method(PackedInt64Array, set, sarray("index", "value"), varray());
	bind_method(PackedInt64Array, push_back, sarray("value"), varray());
	bind_method(PackedInt64Array, append, sarray("value"), varray());
	bind_method(PackedInt64Array, append_array, sarray("array"), varray());
	bind_method(PackedInt64Array, remove_at, sarray("index"), varray());
	bind_method(PackedInt64Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedInt64Array, fill, sarray("value"), varray());
	bind_method(PackedInt64Array, resize, sarray("new_size"), varray());
	bind_method(PackedInt64Array, has, sarray("value"), varray());
	bind_method(PackedInt64Array, reverse, sarray(), varray());
	bind_method(PackedInt64Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedInt64Array, to_byte_array, sarray(), varray());
	bind_method(PackedInt64Array, sort, sarray(), varray());
	bind_method(PackedInt64Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedInt64Array, duplicate, sarray(), varray());

	/* Float32 Array */

	bind_method(PackedFloat32Array, size, sarray(), varray());
	bind_method(PackedFloat32Array, is_empty, sarray(), varray());
	bind_method(PackedFloat32Array, set, sarray("index", "value"), varray());
	bind_method(PackedFloat32Array, push_back, sarray("value"), varray());
	bind_method(PackedFloat32Array, append, sarray("value"), varray());
	bind_method(PackedFloat32Array, append_array, sarray("array"), varray());
	bind_method(PackedFloat32Array, remove_at, sarray("index"), varray());
	bind_method(PackedFloat32Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedFloat32Array, fill, sarray("value"), varray());
	bind_method(PackedFloat32Array, resize, sarray("new_size"), varray());
	bind_method(PackedFloat32Array, has, sarray("value"), varray());
	bind_method(PackedFloat32Array, reverse, sarray(), varray());
	bind_method(PackedFloat32Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedFloat32Array, to_byte_array, sarray(), varray());
	bind_method(PackedFloat32Array, sort, sarray(), varray());
	bind_method(PackedFloat32Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedFloat32Array, duplicate, sarray(), varray());

	/* Float64 Array */

	bind_method(PackedFloat64Array, size, sarray(), varray());
	bind_method(PackedFloat64Array, is_empty, sarray(), varray());
	bind_method(PackedFloat64Array, set, sarray("index", "value"), varray());
	bind_method(PackedFloat64Array, push_back, sarray("value"), varray());
	bind_method(PackedFloat64Array, append, sarray("value"), varray());
	bind_method(PackedFloat64Array, append_array, sarray("array"), varray());
	bind_method(PackedFloat64Array, remove_at, sarray("index"), varray());
	bind_method(PackedFloat64Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedFloat64Array, fill, sarray("value"), varray());
	bind_method(PackedFloat64Array, resize, sarray("new_size"), varray());
	bind_method(PackedFloat64Array, has, sarray("value"), varray());
	bind_method(PackedFloat64Array, reverse, sarray(), varray());
	bind_method(PackedFloat64Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedFloat64Array, to_byte_array, sarray(), varray());
	bind_method(PackedFloat64Array, sort, sarray(), varray());
	bind_method(PackedFloat64Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedFloat64Array, duplicate, sarray(), varray());

	/* String Array */

	bind_method(PackedStringArray, size, sarray(), varray());
	bind_method(PackedStringArray, is_empty, sarray(), varray());
	bind_method(PackedStringArray, set, sarray("index", "value"), varray());
	bind_method(PackedStringArray, push_back, sarray("value"), varray());
	bind_method(PackedStringArray, append, sarray("value"), varray());
	bind_method(PackedStringArray, append_array, sarray("array"), varray());
	bind_method(PackedStringArray, remove_at, sarray("index"), varray());
	bind_method(PackedStringArray, insert, sarray("at_index", "value"), varray());
	bind_method(PackedStringArray, fill, sarray("value"), varray());
	bind_method(PackedStringArray, resize, sarray("new_size"), varray());
	bind_method(PackedStringArray, has, sarray("value"), varray());
	bind_method(PackedStringArray, reverse, sarray(), varray());
	bind_method(PackedStringArray, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedStringArray, to_byte_array, sarray(), varray());
	bind_method(PackedStringArray, sort, sarray(), varray());
	bind_method(PackedStringArray, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedStringArray, duplicate, sarray(), varray());

	/* Vector2 Array */

	bind_method(PackedVector2Array, size, sarray(), varray());
	bind_method(PackedVector2Array, is_empty, sarray(), varray());
	bind_method(PackedVector2Array, set, sarray("index", "value"), varray());
	bind_method(PackedVector2Array, push_back, sarray("value"), varray());
	bind_method(PackedVector2Array, append, sarray("value"), varray());
	bind_method(PackedVector2Array, append_array, sarray("array"), varray());
	bind_method(PackedVector2Array, remove_at, sarray("index"), varray());
	bind_method(PackedVector2Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedVector2Array, fill, sarray("value"), varray());
	bind_method(PackedVector2Array, resize, sarray("new_size"), varray());
	bind_method(PackedVector2Array, has, sarray("value"), varray());
	bind_method(PackedVector2Array, reverse, sarray(), varray());
	bind_method(PackedVector2Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedVector2Array, to_byte_array, sarray(), varray());
	bind_method(PackedVector2Array, sort, sarray(), varray());
	bind_method(PackedVector2Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedVector2Array, duplicate, sarray(), varray());

	/* Vector3 Array */

	bind_method(PackedVector3Array, size, sarray(), varray());
	bind_method(PackedVector3Array, is_empty, sarray(), varray());
	bind_method(PackedVector3Array, set, sarray("index", "value"), varray());
	bind_method(PackedVector3Array, push_back, sarray("value"), varray());
	bind_method(PackedVector3Array, append, sarray("value"), varray());
	bind_method(PackedVector3Array, append_array, sarray("array"), varray());
	bind_method(PackedVector3Array, remove_at, sarray("index"), varray());
	bind_method(PackedVector3Array, insert, sarray("at_index", "value"), varray());
	bind_method(PackedVector3Array, fill, sarray("value"), varray());
	bind_method(PackedVector3Array, resize, sarray("new_size"), varray());
	bind_method(PackedVector3Array, has, sarray("value"), varray());
	bind_method(PackedVector3Array, reverse, sarray(), varray());
	bind_method(PackedVector3Array, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedVector3Array, to_byte_array, sarray(), varray());
	bind_method(PackedVector3Array, sort, sarray(), varray());
	bind_method(PackedVector3Array, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedVector3Array, duplicate, sarray(), varray());

	/* Color Array */

	bind_method(PackedColorArray, size, sarray(), varray());
	bind_method(PackedColorArray, is_empty, sarray(), varray());
	bind_method(PackedColorArray, set, sarray("index", "value"), varray());
	bind_method(PackedColorArray, push_back, sarray("value"), varray());
	bind_method(PackedColorArray, append, sarray("value"), varray());
	bind_method(PackedColorArray, append_array, sarray("array"), varray());
	bind_method(PackedColorArray, remove_at, sarray("index"), varray());
	bind_method(PackedColorArray, insert, sarray("at_index", "value"), varray());
	bind_method(PackedColorArray, fill, sarray("value"), varray());
	bind_method(PackedColorArray, resize, sarray("new_size"), varray());
	bind_method(PackedColorArray, has, sarray("value"), varray());
	bind_method(PackedColorArray, reverse, sarray(), varray());
	bind_method(PackedColorArray, slice, sarray("begin", "end"), varray(INT_MAX));
	bind_method(PackedColorArray, to_byte_array, sarray(), varray());
	bind_method(PackedColorArray, sort, sarray(), varray());
	bind_method(PackedColorArray, bsearch, sarray("value", "before"), varray(true));
	bind_method(PackedColorArray, duplicate, sarray(), varray());

	/* Register constants */

	int ncc = Color::get_named_color_count();
	for (int i = 0; i < ncc; i++) {
		_VariantCall::add_variant_constant(Variant::COLOR, Color::get_named_color_name(i), Color::get_named_color(i));
	}

	_VariantCall::add_constant(Variant::VECTOR3, "AXIS_X", Vector3::AXIS_X);
	_VariantCall::add_constant(Variant::VECTOR3, "AXIS_Y", Vector3::AXIS_Y);
	_VariantCall::add_constant(Variant::VECTOR3, "AXIS_Z", Vector3::AXIS_Z);

	_VariantCall::add_variant_constant(Variant::VECTOR3, "ZERO", Vector3(0, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "ONE", Vector3(1, 1, 1));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "INF", Vector3(INFINITY, INFINITY, INFINITY));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "LEFT", Vector3(-1, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "RIGHT", Vector3(1, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "UP", Vector3(0, 1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "DOWN", Vector3(0, -1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "FORWARD", Vector3(0, 0, -1));
	_VariantCall::add_variant_constant(Variant::VECTOR3, "BACK", Vector3(0, 0, 1));

	_VariantCall::add_constant(Variant::VECTOR3I, "AXIS_X", Vector3i::AXIS_X);
	_VariantCall::add_constant(Variant::VECTOR3I, "AXIS_Y", Vector3i::AXIS_Y);
	_VariantCall::add_constant(Variant::VECTOR3I, "AXIS_Z", Vector3i::AXIS_Z);

	_VariantCall::add_variant_constant(Variant::VECTOR3I, "ZERO", Vector3i(0, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "ONE", Vector3i(1, 1, 1));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "LEFT", Vector3i(-1, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "RIGHT", Vector3i(1, 0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "UP", Vector3i(0, 1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "DOWN", Vector3i(0, -1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "FORWARD", Vector3i(0, 0, -1));
	_VariantCall::add_variant_constant(Variant::VECTOR3I, "BACK", Vector3i(0, 0, 1));

	_VariantCall::add_constant(Variant::VECTOR2, "AXIS_X", Vector2::AXIS_X);
	_VariantCall::add_constant(Variant::VECTOR2, "AXIS_Y", Vector2::AXIS_Y);

	_VariantCall::add_constant(Variant::VECTOR2I, "AXIS_X", Vector2i::AXIS_X);
	_VariantCall::add_constant(Variant::VECTOR2I, "AXIS_Y", Vector2i::AXIS_Y);

	_VariantCall::add_variant_constant(Variant::VECTOR2, "ZERO", Vector2(0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "ONE", Vector2(1, 1));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "INF", Vector2(INFINITY, INFINITY));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "LEFT", Vector2(-1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "RIGHT", Vector2(1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "UP", Vector2(0, -1));
	_VariantCall::add_variant_constant(Variant::VECTOR2, "DOWN", Vector2(0, 1));

	_VariantCall::add_variant_constant(Variant::VECTOR2I, "ZERO", Vector2i(0, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2I, "ONE", Vector2i(1, 1));
	_VariantCall::add_variant_constant(Variant::VECTOR2I, "LEFT", Vector2i(-1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2I, "RIGHT", Vector2i(1, 0));
	_VariantCall::add_variant_constant(Variant::VECTOR2I, "UP", Vector2i(0, -1));
	_VariantCall::add_variant_constant(Variant::VECTOR2I, "DOWN", Vector2i(0, 1));

	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_XYZ", Basis::EULER_ORDER_XYZ);
	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_XZY", Basis::EULER_ORDER_XZY);
	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_YXZ", Basis::EULER_ORDER_YXZ);
	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_YZX", Basis::EULER_ORDER_YZX);
	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_ZXY", Basis::EULER_ORDER_ZXY);
	_VariantCall::add_constant(Variant::BASIS, "EULER_ORDER_ZYX", Basis::EULER_ORDER_ZYX);

	_VariantCall::add_variant_constant(Variant::TRANSFORM2D, "IDENTITY", Transform2D());
	_VariantCall::add_variant_constant(Variant::TRANSFORM2D, "FLIP_X", Transform2D(-1, 0, 0, 1, 0, 0));
	_VariantCall::add_variant_constant(Variant::TRANSFORM2D, "FLIP_Y", Transform2D(1, 0, 0, -1, 0, 0));

	Transform3D identity_transform = Transform3D();
	Transform3D flip_x_transform = Transform3D(-1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0);
	Transform3D flip_y_transform = Transform3D(1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0);
	Transform3D flip_z_transform = Transform3D(1, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0);
	_VariantCall::add_variant_constant(Variant::TRANSFORM3D, "IDENTITY", identity_transform);
	_VariantCall::add_variant_constant(Variant::TRANSFORM3D, "FLIP_X", flip_x_transform);
	_VariantCall::add_variant_constant(Variant::TRANSFORM3D, "FLIP_Y", flip_y_transform);
	_VariantCall::add_variant_constant(Variant::TRANSFORM3D, "FLIP_Z", flip_z_transform);

	Basis identity_basis = Basis();
	Basis flip_x_basis = Basis(-1, 0, 0, 0, 1, 0, 0, 0, 1);
	Basis flip_y_basis = Basis(1, 0, 0, 0, -1, 0, 0, 0, 1);
	Basis flip_z_basis = Basis(1, 0, 0, 0, 1, 0, 0, 0, -1);
	_VariantCall::add_variant_constant(Variant::BASIS, "IDENTITY", identity_basis);
	_VariantCall::add_variant_constant(Variant::BASIS, "FLIP_X", flip_x_basis);
	_VariantCall::add_variant_constant(Variant::BASIS, "FLIP_Y", flip_y_basis);
	_VariantCall::add_variant_constant(Variant::BASIS, "FLIP_Z", flip_z_basis);

	_VariantCall::add_variant_constant(Variant::PLANE, "PLANE_YZ", Plane(Vector3(1, 0, 0), 0));
	_VariantCall::add_variant_constant(Variant::PLANE, "PLANE_XZ", Plane(Vector3(0, 1, 0), 0));
	_VariantCall::add_variant_constant(Variant::PLANE, "PLANE_XY", Plane(Vector3(0, 0, 1), 0));

	_VariantCall::add_variant_constant(Variant::QUATERNION, "IDENTITY", Quaternion(0, 0, 0, 1));
}

void Variant::_register_variant_methods() {
	_register_variant_builtin_methods(); //needs to be out due to namespace
}

void Variant::_unregister_variant_methods() {
	//clear methods
	memdelete_arr(builtin_method_names);
	memdelete_arr(builtin_method_info);
	memdelete_arr(_VariantCall::constant_data);
}
