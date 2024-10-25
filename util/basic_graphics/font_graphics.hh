#pragma once

#include "font.hh"
#include <cstdarg>
#include <cstdio>
#include <string_view>

template<typename ColorT>
class FontGraphics {

#ifndef PRINTF_BUFFER_MAX
#define PRINTF_BUFFER_MAX 64u
#endif

	static constexpr auto char_buffer_max = std::max<unsigned>(PRINTF_BUFFER_MAX, sizeof("-2147483648"));
	const Font::Font *font = &Font::Font8x16x95;
	unsigned zoom{1};
	unsigned x{0};
	unsigned y{0};
	ColorT *bg{};

public:
	void set_font(this auto &&self, const Font::Font &font) {
		self.font = &font;
	}

	void set_zoom(this auto &&self, unsigned zoom) {
		self.zoom = zoom;
	}

	void set_cursor(this auto &&self, unsigned x, unsigned y) {
		self.x = x;
		self.y = y;
	}

	void set_bg(this auto &&self, ColorT bg) {
		self.bg = &bg;
	}

	void print_char(this auto &&self, char ch) {
		ch -= 32;
		if (ch >= self.font->num_chars)
			return;

		const auto offset = ch * self.font->width;

		for (auto i = 0u; i < self.font->width; i++) {
			for (auto j = 0u; j < self.font->height; j++) {
				auto col = self.color;
				if (!((*self.font)[offset + i] & (1 << j)))
					col = self.bg;
				// self.zoom
				for (auto xx = 0; xx < self.zoom; xx++) {
					for (auto yy = 0; yy < self.zoom; yy++) {
						const auto xxx = self.x + (i * self.zoom) + xx;
						const auto yyy = self.y + (j * self.zoom) + yy;
						self.draw_pixel(xxx, yyy, col);
					}
				}
			}
		}
		self.x += (self.font->width * self.zoom) + self.zoom;
	}

	void print_char_centered(this auto &&self, char ch) {
		self.x -= self.font->width * self.zoom / 2;
		self.y -= self.font->height * self.zoom / 2;
		self.print_char(ch);
	}

	void print(this auto &&self, const std::string_view string) {
		auto ptr = string.data();
		while (*ptr != '\0') {
			self.print_char(*ptr);
			ptr++;
		}
	}

	void printf(this auto &&self, const std::string_view string, auto... t) {
		snprintf(self.buff, char_buffer_max, string.data(), t...);
		self.print(self.buff);
	}

	void print_centered(this auto &&self, const std::string_view string) {
		self.x -= self.string_width(string) / 2;
		self.y -= self.char_height() / 2;
		self.print(string);
	}

	void printf_centered(this auto &&self, const std::string_view string, auto... t) {
		snprintf(self.buff, char_buffer_max, string.data(), t...);
		self.print_centered(self.buff);
	}

	void print_number(this auto &&self, int number) {
		sprintf(self.buff, "%i", number);
		self.print(self.buff);
	}

	void print_number_centered(this auto &&self, int number) {
		sprintf(self.buff, "%i", number);
		self.print_centered(self.buff);
	}

private:
	unsigned string_width(this auto &&self, const std::string_view string) {
		const auto l = string.size();
		const auto char_w = self.font->width * self.zoom;
		const auto space_w = self.zoom;
		return char_w * l + space_w * (l - 1);
	}

	unsigned char_height(this auto &&self) {
		return self.font->height * self.zoom;
	}
};
