#include "doctest.h"
#include "util/saveable.hh"
#include <array>
#include <vector>

class Data_V0 : public serializer {
public:
	uint8_t member_0 = 1;
};

class Data_V1 : public serializer_extension<Data_V0> {
public:
	uint64_t member_1 = -1;
	void make_compatible() {
		member_0 = 55;
	}
};

class Data_V2 : public serializer_extension<Data_V1> {
public:
	std::array<int16_t, 5> member_2 = {1, 2, 3, 4, 5};

	//void make_compatible() {
	//	//this function is optional
	//}
};

class Data_V3 : public serializer_extension<Data_V2> {
public:
	bool member_3 = true;
	void make_compatible() {
		member_0 = 13;
	}
};

static_assert(Data_V0{}.get_version() == 0);
static_assert(Data_V1{}.get_version() == 1);
static_assert(Data_V2{}.get_version() == 2);
static_assert(Data_V3{}.get_version() == 3);

TEST_CASE("Saveable test") {
	Data_V0 test_data_0{};

	std::vector<char> serialized_data{};
	const auto t = test_data_0.serialize([&serialized_data](const std::span<const char> raw) {
		serialized_data.assign(raw.begin(), raw.end());
		return true;
	});

	Data_V3 test_data_3{};
	const auto r = test_data_3.deserialize(test_data_0.get_version(), [&serialized_data](const std::span<char> raw) {
		for (auto idx = 0; auto &r : raw) {
			r = serialized_data[idx++];
		}
		return true;
	});

	CHECK(test_data_3.member_0 == 13);
}
