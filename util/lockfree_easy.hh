#pragma once

#include <array>
#include <atomic>

template<typename T>
class LockfreeBufferSPSC {
	std::array<T, 2> buffer{};
	std::atomic<uint8_t> readers_buffer{};
	uint8_t writers_buffer{1};

public:
	class Writer : public std::reference_wrapper<T> {
		friend class LockfreeBufferSPSC;
		LockfreeBufferSPSC &f;

	public:
		Writer() = delete;

		void flush() {
			f.swap_buffers();
		}

	private:
		Writer(LockfreeBufferSPSC &f)
			: f{f}
			, std::reference_wrapper<T>::reference_wrapper{f.buffer[f.writers_buffer]} {
		}
	};

	LockfreeBufferSPSC() = default;
	LockfreeBufferSPSC(LockfreeBufferSPSC const &) = delete;
	LockfreeBufferSPSC &operator=(LockfreeBufferSPSC const &) = delete;
	LockfreeBufferSPSC(LockfreeBufferSPSC &&) = delete;
	LockfreeBufferSPSC &operator=(LockfreeBufferSPSC &&) = delete;

	Writer as_writer() {
		return Writer{*this};
	}

	const T &as_reader() const {
		return buffer[readers_buffer];
	}

private:
	void swap_buffers() {
		writers_buffer = readers_buffer.fetch_xor(1);
		buffer[writers_buffer] = buffer[writers_buffer ^ 1];
	}
};
