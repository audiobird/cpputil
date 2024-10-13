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
	//Font::Font *font;

public:
	void print_char(this auto &&self, const Font::Font &font, char ch, int x, int y, unsigned zoom, ColorT color) {
		ch -= 32;
		if (ch >= font.num_chars)
			return;

		const auto offset = ch * font.width;

		for (auto i = 0u; i < font.width; i++) {
			for (auto j = 0u; j < font.height; j++) {
				if (!(font[offset + i] & (1 << j)))
					continue;
				// zoom
				for (auto xx = 0; xx < zoom; xx++) {
					for (auto yy = 0; yy < zoom; yy++) {
						const auto xxx = x + (i * zoom) + xx;
						const auto yyy = y + (j * zoom) + yy;
						self.draw_pixel(xxx, yyy, color);
					}
				}
			}
		}
	}

	void
	print_char_centered(this auto &&self, const Font::Font &font, char ch, int x, int y, unsigned zoom, ColorT color) {
		const auto offsetx = font.width * zoom / 2;
		const auto offsety = font.height * zoom / 2;
		self.print_char(font, ch, x - offsetx, y - offsety, zoom, color);
	}

	void print(this auto &&self,
			   const Font::Font &font,
			   const std::string_view string,
			   int x,
			   int y,
			   unsigned zoom,
			   ColorT color) {
		auto ptr = string.data();
		while (*ptr != '\0') {
			self.print_char(font, *ptr, x, y, zoom, color);
			ptr++;
			x += (font.width * zoom) + zoom;
		}
	}

	void printf(this auto &&self,
				const Font::Font &font,
				const std::string_view string,
				int x,
				int y,
				unsigned zoom,
				ColorT color,
				auto... t) {

		snprintf(self.buff, char_buffer_max, string.data(), t...);
		self.print(font, self.buff, x, y, zoom, color);
	}

	void print_centered(this auto &&self,
						const Font::Font &font,
						const std::string_view string,
						int x,
						int y,
						unsigned zoom,
						ColorT color) {
		const auto offsetx = self.string_width(font, zoom, string) / 2;
		const auto offsety = self.char_height(font, zoom) / 2;
		self.print(font, string, x - offsetx, y - offsety, zoom, color);
	}

	void printf_centered(this auto &&self,
						 const Font::Font &font,
						 const std::string_view string,
						 int x,
						 int y,
						 unsigned zoom,
						 ColorT color,
						 auto... t) {

		snprintf(self.buff, char_buffer_max, string.data(), t...);
		self.print_centered(font, self.buff, x, y, zoom, color);
	}

	void print_number(this auto &&self, const Font::Font &font, int number, int x, int y, unsigned zoom, ColorT color) {
		sprintf(self.buff, "%i", number);
		self.print(font, self.buff, x, y, zoom, color);
	}

	void print_number_centered(
		this auto &&self, const Font::Font &font, int number, int x, int y, unsigned zoom, ColorT color) {
		sprintf(self.buff, "%i", number);
		self.print_centered(font, self.buff, x, y, zoom, color);
	}

private:
	unsigned string_width(this auto &&self, const Font::Font &font, unsigned zoom, const std::string_view string) {
		const auto l = string.size();
		const auto char_w = font.width * zoom;
		const auto space_w = zoom;
		return char_w * l + space_w * (l - 1);
	}

	unsigned char_height(this auto &&self, const Font::Font &font, unsigned zoom) {
		return font.height * zoom;
	}

	char buff[char_buffer_max];
};
