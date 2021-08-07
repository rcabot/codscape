#pragma once
#include <string>

#include <libtcod.h>

class olcSprite
{
public:
	olcSprite() = default;

	olcSprite(const int w, const int h, const int x_pivot, const int y_pivot, const std::string& characters, const TCOD_color_t fcolor, const TCOD_color_t bcolor) : olcSprite(w, h, x_pivot, y_pivot)
	{
		for (auto i = 0; i < w * h; i++)
		{
			m_Glyphs[i] = static_cast<short>(characters[i]);
			m_fColours[i] = fcolor;
			m_bColours[i] = bcolor;
		}
	}

	olcSprite(int w, int h, int x_pivot, int y_pivot) : x_pivot_{x_pivot}, y_pivot_{y_pivot}
	{
		Create(w, h);
	}

	int nWidth = 0;
	int nHeight = 0;
	int x_pivot_ = 0;
	int y_pivot_ = 0;

private:
	short* m_Glyphs = nullptr;
	TCODColor* m_fColours = nullptr;
	TCODColor* m_bColours = nullptr;

	void Create(int w, int h)
	{
		nWidth = w;
		nHeight = h;
		m_Glyphs = new short[w * h];
		m_fColours = new TCODColor[w * h];
		m_bColours = new TCODColor[w * h];
		for (int i = 0; i < w * h; i++)
		{
			m_Glyphs[i] = L' ';
			m_fColours[i] = TCOD_black;
			m_bColours[i] = TCOD_black;
		}
	}

public:
	void SetGlyph(int x, int y, short c)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return;
		else
			m_Glyphs[y * nWidth + x] = c;
	}

	void SetFColour(int x, int y, TCODColor c)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return;
		else
			m_fColours[y * nWidth + x] = c;
	}

	void SetBColour(int x, int y, TCODColor c)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return;
		else
			m_bColours[y * nWidth + x] = c;
	}

	short GetGlyph(int x, int y)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return L' ';
		else
			return m_Glyphs[y * nWidth + x];
	}

	TCODColor GetFColour(int x, int y)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return TCOD_black;
		else
			return m_fColours[y * nWidth + x];
	}

	TCODColor GetBColour(int x, int y)
	{
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
			return TCOD_black;
		else
			return m_bColours[y * nWidth + x];
	}

	short SampleGlyph(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);
		if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
			return L' ';
		else
			return m_Glyphs[sy * nWidth + sx];
	}

	TCODColor SampleFColour(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);
		if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
			return TCOD_black;
		else
			return m_fColours[sy * nWidth + sx];
	}

	TCODColor SampleBColour(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);
		if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
			return TCOD_black;
		else
			return m_bColours[sy * nWidth + sx];
	}
};

