#ifndef _TEXT_DRAW_H
#define _TEXT_DRAW_H

#include <string>
#include <cassert>

#include "font.h"
#include "scenegraph.h"
#include "vertexarray.h"

class TEXT_DRAW
{
private:
	VERTEXARRAY varray;
	std::string text;
	float oldx, oldy, oldscalex, oldscaley;
	
	float RenderCharacter(VERTEXARRAY & output_array, const float tw, const float th, const float x, const float y, const float scalex, const float scaley, const FONT::CHARINFO & c);
	
public:
	TEXT_DRAW() : oldx(0), oldy(0), oldscalex(1.0), oldscaley(1.0) {}
	
	void Set(DRAWABLE & draw, const FONT & font, const std::string & newtext, const float x, const float y, const float newscalex, const float newscaley, const float r=1, const float g=1, const float b=1)
	{
		Set(draw, font, newtext, x, y, newscalex, newscaley, r,g,b,varray);
	}
	
	void Set(DRAWABLE & draw, const FONT & font, const std::string & newtext, const float x, const float y, const float newscalex, const float newscaley, const float r, const float g, const float b, VERTEXARRAY & output_array);
	
	void Revise(const FONT & font, const std::string & newtext)
	{
		Revise(font, newtext, oldx, oldy, oldscalex, oldscaley);
	}
	
	void Revise(const FONT & font, const std::string & newtext, float x, float y, float scalex, float scaley)
	{
		Revise(font, newtext, x, y, scalex, scaley, varray);
	}
	
	void Revise(const FONT & font, const std::string & newtext, float x, float y, float scalex, float scaley, VERTEXARRAY & output_array);
	
	float GetWidth(const FONT & font, const std::string & newtext, const float newscale) const;

	const std::string & GetText() const
	{
		return text;
	}
	
	const std::pair<float,float> GetCurrentScale() const {return std::pair<float,float>(oldscalex,oldscaley);}
};

///a slightly higher level class than the TEXT_DRAW Class that contains its own DRAWABLE handle
class TEXT_DRAWABLE
{
private:
	TEXT_DRAW text;
	keyed_container <DRAWABLE>::handle draw;
	const FONT * font;
	float curx, cury;
	float cr,cg,cb,ca;
	
public:
	TEXT_DRAWABLE() : font(NULL),curx(0),cury(0),cr(1),cg(1),cb(1),ca(1) {}
	
	///this function will add a drawable to parentnode and store the result
	void Init(SCENENODE & parentnode, const FONT & newfont, const std::string & newtext, const float x, const float y, const float newscalex, const float newscaley)
	{
		assert(font == NULL);
		
		draw = parentnode.GetDrawlist().text.insert(DRAWABLE());
		DRAWABLE & drawref = GetDrawable(parentnode);
		font = &newfont;
		curx = x;
		cury = y;
		text.Set(drawref, *font, newtext, x, y, newscalex, newscaley, cr,cg,cb);
		SetAlpha(parentnode, ca);
	}
	
	void Revise(const std::string & newtext)
	{
		assert(font);
		text.Revise(*font, newtext, curx, cury, text.GetCurrentScale().first, text.GetCurrentScale().second);
	}
	
	void Revise(const std::string & newtext, const float x, const float y, const float newscalex, const float newscaley)
	{
		curx = x;
		cury = y;
		text.Revise(*font, newtext, curx, cury, newscalex, newscaley);
	}
	
	void SetPosition(float newx, float newy)
	{
		assert(font);
		curx = newx;
		cury = newy;
		text.Revise(*font, text.GetText(), curx, cury, text.GetCurrentScale().first, text.GetCurrentScale().second);
	}
	
	void SetColor(SCENENODE & parentnode, const float r, const float g, const float b)
	{
		DRAWABLE & drawref = GetDrawable(parentnode);
		cr = r;
		cg = g;
		cb = b;
		drawref.SetColor(cr,cg,cb,ca);
	}
	
	void SetAlpha(SCENENODE & parentnode, const float a)
	{
		DRAWABLE & drawref = GetDrawable(parentnode);
		ca = a;
		drawref.SetColor(cr,cg,cb,ca);
	}
	
	float GetWidth() const
	{
		assert (font);
		return text.GetWidth(*font, text.GetText(), text.GetCurrentScale().first);
	}
	
	float GetWidth(const std::string & newstr) const
	{
		assert (font);
		return text.GetWidth(*font, newstr, text.GetCurrentScale().first);
	}
	
	void SetDrawOrder(SCENENODE & parentnode, float newdo)
	{
		DRAWABLE & drawref = GetDrawable(parentnode);
		drawref.SetDrawOrder(newdo);
	}
	
	void SetDrawEnable(SCENENODE & parentnode, bool newvis)
	{
		DRAWABLE & drawref = GetDrawable(parentnode);
		drawref.SetDrawEnable(newvis);
	}
	
	void ToggleDrawEnable(SCENENODE & parentnode)
	{
		DRAWABLE & drawref = GetDrawable(parentnode);
		drawref.SetDrawEnable(!drawref.GetDrawEnable());
	}
	
	DRAWABLE & GetDrawable(SCENENODE & parentnode)
	{return parentnode.GetDrawlist().text.get(draw);}
};

#endif
