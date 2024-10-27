#pragma once

#include "font_graphics.hh"
#include "util/basic_graphics/crtp_driver.hh"
//CRTP Graphics Class

template<GraphicsMiddleware G>
class SimpleGraphics : public G {
#ifndef PRINTF_BUFFER_MAX
#define PRINTF_BUFFER_MAX 64u
#endif

	static constexpr auto char_buffer_max = std::max<unsigned>(PRINTF_BUFFER_MAX, sizeof("-2147483648"));
	const Font::Font *font = &Font::Font6x7x95;
	int zoom{1};
	unsigned x{0};
	unsigned y{0};
	char buff[char_buffer_max];

public:
	void draw_h_line(int x, int y, int w, G::ColorT color) {
		for (auto i = 0; i < w; i++)
			G::draw_pixel(x + i, y, color);
	}

	void draw_v_line(int x, int y, int h, G::ColorT color) {
		for (auto i = 0; i < h; i++)
			G::draw_pixel(x, y + i, color);
	}

	void draw_rect(int x, int y, int w, int h, G::ColorT color) {
		draw_h_line(x, y, w, color);
		draw_h_line(x, y + h - 1, w, color);
		draw_v_line(x, y, h, color);
		draw_v_line(x + w - 1, y, h, color);
	}

	void draw_rect_center(int x, int y, int w, int h, G::ColorT color) {
		x -= w / 2;
		y -= h / 2;
		draw_rect(x, y, w, h, color);
	}

	void fill_rect(int x, int y, int w, int h, G::ColorT color) {
		for (auto i = 0; i < w; i++) {
			draw_v_line(x + i, y, h, color);
		}
	}

	void fill_rect_center(int x, int y, int w, int h, G::ColorT color) {
		x -= w / 2;
		y -= h / 2;
		fill_rect(x, y, w, h, color);
	}

	void draw_round_rect(int x, int y, int w, int h, int r, G::ColorT color) {
		auto max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		draw_h_line(x + r, y, w - r, color);		 // Top
		draw_h_line(x + r, y + h - 1, w - r, color); // Bottom
		draw_v_line(x, y + r, h - r, color);		 // Left
		draw_v_line(x + w - 1, y + r, h - r, color); // Right

		draw_circle_helper(x + r, y + r, r, 1, color);
		draw_circle_helper(x + w - r - 1, y + r, r, 2, color);
		draw_circle_helper(x + w - r - 1, y + h - r - 1, r, 4, color);
		draw_circle_helper(x + r, y + h - r - 1, r, 8, color);
	}

	void draw_round_rect_center(int x, int y, int w, int h, int r, G::ColorT color) {
		x -= w / 2;
		y -= h / 2;
		draw_round_rect(x, y, w, h, r, color);
	}

	void fill_round_rect(int x, int y, int w, int h, int r, G::ColorT color) {
		int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		fill_rect(x + r, y, w - 2 * r, h, color);
		fill_circle_helper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
		fill_circle_helper(x + r, y + r, r, 2, h - 2 * r - 1, color);
	}

	void fill_round_rect_center(int x, int y, int w, int h, int r, G::ColorT color) {
		x -= w / 2;
		y -= h / 2;
		fill_round_rect(x, y, w, h, r, color);
	}

	void set_font(const Font::Font &font) {
		this->font = &font;
	}

	void set_zoom(unsigned zoom) {
		this->zoom = zoom;
	}

	void set_cursor(unsigned x, unsigned y) {
		this->x = x;
		this->y = y;
	}

	void print_char(char ch, G::ColorT color) {
		ch -= 32;
		if (ch >= font->num_chars)
			return;

		const auto offset = ch * font->offset;

		for (auto j = 0u; j < font->width; j++) {
			for (auto i = 0u; i < font->height; i++) {
				if (!((*font)[offset + i] & (1 << (7 - j))))
					continue;
				// zoom
				for (auto xx = 0; xx < zoom; xx++) {
					for (auto yy = 0; yy < zoom; yy++) {
						const auto xxx = x + (j * zoom) + xx;
						const auto yyy = y + (i * zoom) + yy;
						G::draw_pixel(xxx, yyy, color);
					}
				}
			}
		}
		x += (font->width * zoom);
	}

	void print_char_centered(char ch, G::ColorT color) {
		x -= font->width * zoom / 2;
		y -= font->height * zoom / 2;
		print_char(ch, color);
	}

	void print(const std::string_view string, G::ColorT color) {
		auto ptr = string.data();
		while (*ptr != '\0') {
			print_char(*ptr, color);
			ptr++;
		}
	}

	void printf(const std::string_view string, G::ColorT color, auto... t) {
		snprintf(buff, char_buffer_max, string.data(), t...);
		print(buff, color);
	}

	void print_centered(const std::string_view string, G::ColorT color) {
		x -= string_width(string) / 2;
		y -= char_height() / 2;
		print(string, color);
	}

	void printf_centered(const std::string_view string, G::ColorT color, auto... t) {
		snprintf(buff, char_buffer_max, string.data(), t...);
		print_centered(buff, color);
	}

	void print_number(int number, G::ColorT color) {
		sprintf(buff, "%i", number);
		print(buff, color);
	}

	void print_number_centered(int number, G::ColorT color) {
		sprintf(buff, "%i", number);
		print_centered(buff, color);
	}

private:
	unsigned string_width(this auto &&self, const std::string_view string) {
		const auto l = string.size();
		const auto char_w = self.font->width * self.zoom;
		return char_w * l;
	}

	unsigned char_height(this auto &&self) {
		return self.font->height * self.zoom;
	}

	void draw_circle_helper(int x, int y, int r, int cornername, G::ColorT color) {
		auto f = 1 - r;
		auto ddF_x = 1;
		auto ddF_y = -2 * r;
		auto x_ = 0;
		auto y_ = r;

		while (x_ < y_) {
			if (f >= 0) {
				y_--;
				ddF_y += 2;
				f += ddF_y;
			}
			x_++;
			ddF_x += 2;
			f += ddF_x;
			if (cornername & 0x4) {
				G::draw_pixel(x + x_, y + y_, color);
				G::draw_pixel(x + y_, y + x_, color);
			}
			if (cornername & 0x2) {
				G::draw_pixel(x + x_, y - y_, color);
				G::draw_pixel(x + y_, y - x_, color);
			}
			if (cornername & 0x8) {
				G::draw_pixel(x - y_, y + x_, color);
				G::draw_pixel(x - x_, y + y_, color);
			}
			if (cornername & 0x1) {
				G::draw_pixel(x - y_, y - x_, color);
				G::draw_pixel(x - x_, y - y_, color);
			}
		}
	}

	void fill_circle_helper(int x, int y, int r, int corners, int delta, G::ColorT color) {

		auto f = 1 - r;
		auto ddF_x = 1;
		auto ddF_y = -2 * r;
		auto x_ = 0;
		auto y_ = r;
		auto px = x_;
		auto py = y_;

		delta++;

		while (x_ < y_) {
			if (f >= 0) {
				y_--;
				ddF_y += 2;
				f += ddF_y;
			}
			x_++;
			ddF_x += 2;
			f += ddF_x;
			if (x_ < (y_ + 1)) {
				if (corners & 1)
					draw_v_line(x + x_, y - y_, 2 * y_ + delta, color);
				if (corners & 2)
					draw_v_line(x - x_, y - y_, 2 * y_ + delta, color);
			}
			if (y_ != py) {
				if (corners & 1)
					draw_v_line(x + py, y - px, 2 * px + delta, color);
				if (corners & 2)
					draw_v_line(x - py, y - px, 2 * px + delta, color);
				py = y_;
			}
			px = x_;
		}
	}
};
