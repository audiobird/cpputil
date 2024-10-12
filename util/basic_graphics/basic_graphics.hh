#pragma once

#include <cstdio>

//CRTP Graphics Class

template<typename ColorT>
class SimpleGraphics {
public:
	void fill(this auto &&self, ColorT color) {
		for (auto x = 0u; x < self.width; ++x) {
			for (auto y = 0u; y < self.height; ++y) {
				self.draw_pixel(x, y, color);
			}
		}
	}

	void draw_h_line(this auto &&self, int x, int y, int w, ColorT color) {
		for (auto i = 0u; i < w; i++)
			self.draw_pixel(x + i, y, color);
	}

	void draw_v_line(this auto &&self, int x, int y, int h, ColorT color) {
		for (auto i = 0u; i < h; i++)
			self.draw_pixel(x, y + i, color);
	}

	void draw_rect(this auto &&self, int x, int y, int w, int h, ColorT color) {
		self.draw_h_line(x, y, w, color);
		self.draw_h_line(x, y + h - 1, w, color);
		self.draw_v_line(x, y, h, color);
		self.draw_v_line(x + w - 1, y, h, color);
	}

	void draw_rect_center(this auto &&self, int x, int y, int w, int h, ColorT color) {
		x -= w / 2;
		y -= h / 2;
		self.draw_rect(x, y, w, h, color);
	}

	void fill_rect(this auto &&self, int x, int y, int w, int h, ColorT color) {
		for (auto i = 0u; i <= w; i++) {
			self.draw_v_line(x + i, y, h, color);
		}
	}

	void fill_rect_center(this auto &&self, int x, int y, int w, int h, ColorT color) {
		x -= w / 2;
		y -= h / 2;
		self.fill_rect(x, y, w, h, color);
	}

	void draw_round_rect(this auto &&self, int x, int y, int w, int h, int r, ColorT color) {
		auto max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		self.draw_h_line(x + r, y, w - r, color);		  // Top
		self.draw_h_line(x + r, y + h - 1, w - r, color); // Bottom
		self.draw_v_line(x, y + r, h - r, color);		  // Left
		self.draw_v_line(x + w - 1, y + r, h - r, color); // Right

		self.draw_circle_helper(x + r, y + r, r, 1, color);
		self.draw_circle_helper(x + w - r - 1, y + r, r, 2, color);
		self.draw_circle_helper(x + w - r - 1, y + h - r - 1, r, 4, color);
		self.draw_circle_helper(x + r, y + h - r - 1, r, 8, color);
	}

	void draw_round_rect_center(this auto &&self, int x, int y, int w, int h, int r, ColorT color) {
		x -= w / 2;
		y -= h / 2;
		self.draw_round_rect(x, y, w, h, r, color);
	}

	void fill_round_rect(this auto &&self, int x, int y, int w, int h, int r, ColorT color) {
		int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		self.fill_rect(x + r, y, w - 2 * r, h, color);
		self.fill_circle_helper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
		self.fill_circle_helper(x + r, y + r, r, 2, h - 2 * r - 1, color);
	}

	void fill_round_rect_center(this auto &&self, int x, int y, int w, int h, int r, ColorT color) {
		x -= w / 2;
		y -= h / 2;
		self.fill_round_rect(x, y, w, h, r, color);
	}

private:
	void draw_circle_helper(this auto &&self, int x, int y, int r, int cornername, ColorT color) {
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
				self.draw_pixel(x + x_, y + y_, color);
				self.draw_pixel(x + y_, y + x_, color);
			}
			if (cornername & 0x2) {
				self.draw_pixel(x + x_, y - y_, color);
				self.draw_pixel(x + y_, y - x_, color);
			}
			if (cornername & 0x8) {
				self.draw_pixel(x - y_, y + x_, color);
				self.draw_pixel(x - x_, y + y_, color);
			}
			if (cornername & 0x1) {
				self.draw_pixel(x - y_, y - x_, color);
				self.draw_pixel(x - x_, y - y_, color);
			}
		}
	}

	void fill_circle_helper(this auto &&self, int x, int y, int r, int corners, int delta, ColorT color) {

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
					self.draw_v_line(x + x_, y - y_, 2 * y_ + delta, color);
				if (corners & 2)
					self.draw_v_line(x - x_, y - y_, 2 * y_ + delta, color);
			}
			if (y_ != py) {
				if (corners & 1)
					self.draw_v_line(x + py, y - px, 2 * px + delta, color);
				if (corners & 2)
					self.draw_v_line(x - py, y - px, 2 * px + delta, color);
				py = y_;
			}
			px = x_;
		}
	}
};
