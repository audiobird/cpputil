#include "doctest.h"
#include "util/bitset.hh"

TEST_CASE("Bitset") {
	bitset<8> b;

	b[0] = true;
	b[3] = true;
	CHECK(b[0] == true);
	CHECK(b[1] == false);
	CHECK(b[2] == false);
	CHECK(b[3] == true);
}
