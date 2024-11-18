#include "doctest.h"
#include "util/lockfree_easy.hh"
#include <array>
#include <thread>

inline constexpr auto num_loops = 1 << 16;
using Data = std::array<int, 256>;
using LockfreeData = LockfreeBufferSPSC<Data>;

TEST_CASE("Lockfree") {
	LockfreeData buffer{};

	int num_reads = {};
	int num_writes = {};

	std::thread producer([&]() {
		Data data{};
		for (int i = 0; i < num_loops; ++i) {
			data[i & (data.size() - 1)] = i;
			while (!buffer.write(data)) {
			}
			++num_writes;
		}
	});

	std::thread consumer([&]() {
		Data data{};
		bool read_at_least_once{};
		for (int i = 0; i < num_loops; ++i) {
			while (!buffer.read(data)) {
			}
			CHECK(data[i & (data.size() - 1)] == i);
			read_at_least_once = true;
			num_reads++;
		}

		CHECK(read_at_least_once == true);
		CHECK(num_reads == num_loops);
		CHECK(num_reads == num_writes);
	});

	producer.join();
	consumer.join();
}
