#include "doctest.h"
#include "util/bitset.hh"

TEST_CASE("Bitset") {
	SUBCASE("count(), set() + overload, reset(), size()") {
		bitset<6> b;

		CHECK(b.size() == 6);
		CHECK(b.count() == 0);

		b.set(0);
		b.set(1, true);
		CHECK(b.count() == 2);

		b.set(0, false);
		CHECK(b.count() == 1);

		b.set();
		CHECK(b.count() == 6);

		b.reset();
		CHECK(b.count() == 0);
	}

	SUBCASE("all(), any(), none(), flip() + overload") {
		bitset<28> b;

		CHECK(b.all() == false);
		CHECK(b.any() == false);
		CHECK(b.none() == true);

		b.set(0);
		CHECK(b.all() == false);
		CHECK(b.any() == true);
		CHECK(b.none() == false);

		b.set(0, false);
		b.flip();
		CHECK(b.all() == true);
		CHECK(b.any() == true);
		CHECK(b.none() == false);

		b.flip(8);
		CHECK(b.all() == false);
		CHECK(b.count() == 27);
	}

	SUBCASE("to_uintx()") {
		bitset<8> b;
		b.flip();
		CHECK(b.to_uintx() == (1u << 8) - 1);

		bitset<3> c;
		c.flip();
		CHECK(c.to_uintx() == (1u << 3) - 1);

		bitset<19> d;
		d.flip();
		CHECK(d.to_uintx() == (1u << 19) - 1);
	}
}

TEST_CASE("Bitset Reference") {
	bitset<12> b;
	b[0] = true;
	b[3] = b[0];
	CHECK(b[0] == true);
	CHECK(b[1] == false);
	CHECK(b[2] == b[1]);
	CHECK(b[2] != ~b[1]);
	CHECK(b[3]);
	b[3].flip();
	CHECK(!b[3]);
	CHECK(b.count() == 1);
}
