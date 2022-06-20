/*************************************************************************/
/*  test_hash_map.h                                                      */
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

#ifndef TEST_HASH_MAP_H
#define TEST_HASH_MAP_H

#include "core/templates/hash_map.h"
#include "core/templates/hash_map_u.h"

#include "tests/test_macros.h"

namespace TestHashMap {

TEST_CASE("[HashMap] Insert element") {
	HashMap<int, int> map;
	HashMap<int, int>::Iterator e = map.insert(42, 84);

	CHECK(e);
	CHECK(e->key == 42);
	CHECK(e->value == 84);
	CHECK(map[42] == 84);
	CHECK(map.has(42));
	CHECK(map.find(42));
}

TEST_CASE("[HashMap] Overwrite element") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(42, 1234);

	CHECK(map[42] == 1234);
}

TEST_CASE("[HashMap] Erase via element") {
	HashMap<int, int> map;
	HashMap<int, int>::Iterator e = map.insert(42, 84);
	map.remove(e);
	CHECK(!map.has(42));
	CHECK(!map.find(42));
}

TEST_CASE("[HashMap] Erase via key") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.erase(42);
	CHECK(!map.has(42));
	CHECK(!map.find(42));
}

TEST_CASE("[HashMap] Size") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 84);
	map.insert(123, 84);
	map.insert(0, 84);
	map.insert(123485, 84);

	CHECK(map.size() == 4);
}

TEST_CASE("[HashMap] Iteration") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 12385);
	map.insert(0, 12934);
	map.insert(123485, 1238888);
	map.insert(123, 111111);

	Vector<Pair<int, int>> expected;
	expected.push_back(Pair<int, int>(42, 84));
	expected.push_back(Pair<int, int>(123, 111111));
	expected.push_back(Pair<int, int>(0, 12934));
	expected.push_back(Pair<int, int>(123485, 1238888));

	int idx = 0;
	for (const KeyValue<int, int> &E : map) {
		CHECK(expected[idx] == Pair<int, int>(E.key, E.value));
		++idx;
	}
}

TEST_CASE("[HashMap] Const iteration") {
	HashMap<int, int> map;
	map.insert(42, 84);
	map.insert(123, 12385);
	map.insert(0, 12934);
	map.insert(123485, 1238888);
	map.insert(123, 111111);

	const HashMap<int, int> const_map = map;

	Vector<Pair<int, int>> expected;
	expected.push_back(Pair<int, int>(42, 84));
	expected.push_back(Pair<int, int>(123, 111111));
	expected.push_back(Pair<int, int>(0, 12934));
	expected.push_back(Pair<int, int>(123485, 1238888));
	expected.push_back(Pair<int, int>(123, 111111));

	int idx = 0;
	for (const KeyValue<int, int> &E : const_map) {
		CHECK(expected[idx] == Pair<int, int>(E.key, E.value));
		++idx;
	}
}

void test_hash_map_vec2i() {
	Vector<Vector2i> keys;
	for (int x = -2000; x < 2000; x++) {
		for (int y = -2000; y < 2000; y++) {
			keys.push_back(Vector2i(x, y * 2));
		}
	}

	// INSERTION
	print_line(">>>>>>> Vector2i-key");
	HashMap<Vector2i, int> map;
	uint64_t stime = OS::get_singleton()->get_ticks_usec();
	for (const Vector2i &key : keys) {
		map.insert(key, 0);
	}
	uint64_t delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap insert: " + String::num(delta / (real_t)1000.0) + "ms");

	HashMapN<Vector2i, int> new_map;
	stime = OS::get_singleton()->get_ticks_usec();
	for (const Vector2i &key : keys) {
		new_map.insert(key, 0);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN insert: " + String::num(delta / (real_t)1000.0) + "ms");

	// FIND

	stime = OS::get_singleton()->get_ticks_usec();
	int sum = 0;
	for (const Vector2i &key : keys) {
		sum += map.get(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap find: " + String::num(delta / (real_t)1000.0) + "ms");

	sum = 0;
	stime = OS::get_singleton()->get_ticks_usec();
	for (const Vector2i &key : keys) {
		sum += new_map.get(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN find: " + String::num(delta / (real_t)1000.0) + "ms");

	// ERASE
	stime = OS::get_singleton()->get_ticks_usec();
	for (const Vector2i &key : keys) {
		map.erase(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap erase: " + String::num(delta / (real_t)1000.0) + "ms");

	stime = OS::get_singleton()->get_ticks_usec();
	for (const Vector2i &key : keys) {
		new_map.erase(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN erase: " + String::num(delta / (real_t)1000.0) + "ms");
}

void test_hash_map_int() {
	Vector<int> keysi;
	for (int x = -100000; x < 100000; x++) {
		keysi.push_back(x * 13);
	}

	// INSERTION
	print_line(">>>>>>> int-key");
	HashMap<int, int> int_map;
	uint64_t stime = OS::get_singleton()->get_ticks_usec();
	for (int x : keysi) {
		int_map.insert(x, 0);
	}
	uint64_t delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap insert: " + String::num(delta / (real_t)1000.0) + "ms");

	HashMapN<int, int> new_int_map;
	stime = OS::get_singleton()->get_ticks_usec();
	for (int x : keysi) {
		new_int_map.insert(x, 0);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN insert: " + String::num(delta / (real_t)1000.0) + "ms");

	// FIND
	int sum = 0;
	stime = OS::get_singleton()->get_ticks_usec();
	for (int key : keysi) {
		sum += int_map.get(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap find: " + String::num(delta / (real_t)1000.0) + "ms");

	sum = 0;
	stime = OS::get_singleton()->get_ticks_usec();
	for (int key : keysi) {
		sum += new_int_map.get(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN find: " + String::num(delta / (real_t)1000.0) + "ms");

	// ERASE
	stime = OS::get_singleton()->get_ticks_usec();
	for (int key : keysi) {
		int_map.erase(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMap erase: " + String::num(delta / (real_t)1000.0) + "ms");

	stime = OS::get_singleton()->get_ticks_usec();
	for (int key : keysi) {
		new_int_map.erase(key);
	}
	delta = OS::get_singleton()->get_ticks_usec() - stime;
	print_line("HashMapN erase: " + String::num(delta / (real_t)1000.0) + "ms");
}

TEST_CASE("[HashMap] Speed") {
	test_hash_map_int();
	test_hash_map_int();
	test_hash_map_vec2i();
	test_hash_map_vec2i();
}

} // namespace TestHashMap

#endif // TEST_HASH_MAP_H
