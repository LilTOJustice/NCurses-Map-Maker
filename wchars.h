#pragma once
#include <locale>

namespace wchars {
		static const wchar_t uparrow = L'↑';
		static const wchar_t downarrow = L'↓';
		static const wchar_t continuearrow = L'▼';
		static const wchar_t leftcontinuearrow = L'◄';
		static const wchar_t rightcontinuearrow = L'►';
		static const wchar_t dot = L'•';
		static const wchar_t heart = L'♥';
		static const wchar_t diamond = L'♦';
		static const wchar_t club = L'♣';
		static const wchar_t spade = L'♠';
		static const wchar_t light_shade = L'░';
		static const wchar_t medium_shade = L'▒';
		static const wchar_t dark_shade = L'▓';
		static const wchar_t wall_vert = L'║';
		static const wchar_t wall_horiz = L'═';
		static const wchar_t wall_upright = L'╗';
		static const wchar_t wall_upleft = L'╔';
		static const wchar_t wall_bottright = L'╝';
		static const wchar_t wall_bottleft = L'╚';
		static const wchar_t wall_intersect = L'╬';
		static const wchar_t wall_interright = L'╠';
		static const wchar_t wall_interleft = L'╣';
		static const wchar_t wall_interdown = L'╦';
		static const wchar_t wall_interup = L'╩';
		static const wchar_t enemy = L'Ϫ';
		static const wchar_t copyright = L'©';
		static const wchar_t cent = L'¢';
		static void init_wchars() { //XXX Comes before initscr
			setlocale(LC_ALL, "");
		}
}
