#pragma once

#include "../colors.hh"
#include <concepts>

template<typename T>
concept GraphicsDriver = requires(T t, int x, int y, const Color &color) {
	{
		t.draw_pixel(x, y, color)
	} -> std::same_as<void>;
	{
		t.fill(color)
	} -> std::same_as<void>;
};

template<typename T>
concept GraphicsMiddleware = requires(T t, int x, int y, const T::ColorT color) {
	{
		t.draw_pixel(x, y, color)
	} -> std::same_as<void>;
	{
		t.fill(color)
	} -> std::same_as<void>;
};
