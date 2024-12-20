/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef SURFACE_H
#define SURFACE_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "TextSurface.h"
#include "Color.h"
#include "Image.h"
#include "vec2.h"

#include "SDL_gfxPrimitives.h"

//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------
const int WIDTH = 640;
const int HEIGHT = 480;
const int BPP = 0;
const int FLAGS = SDL_ANYFORMAT;

class SingletonSurface; // Use for making it a friend
class Image;

inline int min( int a, int b )	{ return ( a<b ? a : b ); }


class Surface{

public:
	Surface( int=WIDTH, int=HEIGHT, int=BPP, int=SDL_ANYFORMAT );	
	~Surface() { SDL_FreeSurface( surface ); }

	void free();

	int  get_bpp();
    void lock();
	void unlock();
	void update_rect( int=0, int=0, int=0, int=0 );
	void flip();

	inline Uint32 map_rgb( const Color & color );

	Uint32	get_pixel( int, int );
	Uint8*	get_pixel_addr( int, int );
	
	//------------------------------------------------------------------------
	// Pixel drawing
	//------------------------------------------------------------------------
	void	put_pixel( int, int, const Color & );
	void	put_pixel( int, int, Uint32 );
	void	put_pixel( int, int, int, int, int );
	void	draw_pixel( int, int, const Color & );

	//------------------------------------------------------------------------
	// Circle drawing
	//------------------------------------------------------------------------
	void	put_circle( int, int, int, Uint32 );
	void	put_circle( int, int, int, const Color & );
	void	put_circle( int, int, int, int, int, int );
	void	draw_circle( int x, int y, int r, Uint32 pixel );
	void	draw_circle( int x, int y, int r, const Color & );
	void	draw_circle( int x, int y, int r, int, int, int );
    
    void	put_circle( vec2i & p, int r, int r_, int b_, int g_)
    {
        put_circle(p.x(), p.y(), r, r_, b_, g_);
    }
    void	put_circle( vec2i & p, int r, const Color & color)
    {
        put_circle(p.x(), p.y(), r, color);
    }
    void	draw_circle( vec2i & p, int r, int r_, int b_, int g_)
    {
        put_circle(p.x(), p.y(), r, r_, b_, g_);
    }
    void	draw_circle( vec2i & p, int r, const Color & color)
    {
        put_circle(p.x(), p.y(), r, color);
    }

    void	put_unfilled_circle(int, int, int, Uint32);
    void    put_unfilled_circle(int, int, int, const Color &);
	void	put_unfilled_circle(int, int, int, int, int, int);
    void	put_unfilled_circle( vec2i & p, int r, int r_, int b_, int g_)
    {
        put_unfilled_circle(p.x(), p.y(), r, r_, b_, g_);
    }
    void	put_unfilled_circle( vec2i & p, int r, const Color & color)
    {
        put_unfilled_circle(p.x(), p.y(), r, color);
    }
	//------------------------------------------------------------------------
	// Line drawing
	//------------------------------------------------------------------------
	void	put_line( int x0, int y0, int x1, int y1, Uint32 );
	void	put_line( int x0, int y0, int x1, int y1, const Color & );
	void	put_line( int x0, int y0, int x1, int y1, int, int, int );
    void    put_line(const vec2i & p0,const vec2i & p1, int r, int b, int g )
        {
            put_line(p0.x(), p0.y(), p1.x(), p1.y(), r, b, g);
        }

    void    put_line(const vec2i & p0, const vec2i & p1, const Color & color )
        {
            put_line(p0.x(), p0.y(), p1.x(), p1.y(), color);
        }
	void	draw_line( int x0, int y0, int x1, int y1, const Color & );
	void	draw_line( int x0, int y0, int x1, int y1, Uint32 );
	void	draw_line( int x0, int y0, int x1, int y1, int, int, int );

	//------------------------------------------------------------------------
	// Text drawing
	//------------------------------------------------------------------------
	void	put_text( TextSurface & text, int x=0, int y=0 );
	void	put_text( const char str[] = "Hello, World!",
					  int  x = 0, int y = 0,
					  int  R=255, int G=255, int B=255,
					  const char fontfamily[] = "arial.ttf", int  size = 12 );

	//------------------------------------------------------------------------
	// Image drawing
	//------------------------------------------------------------------------
	void put_image(SDL_Surface *image, SDL_Rect *srcRect, SDL_Rect *dstRect);
	void put_image(const Image & image, const Rect & src, const Rect & dst)
	{
		SDL_Rect _dst = {Sint16(dst.x), Sint16(dst.y),
                         Uint16(dst.w), Uint16(dst.h)};
		SDL_Rect _src = {Sint16(src.x), Sint16(src.y),
                         Uint16(src.w), Uint16(src.h)};
		put_image(image.image, &_src, &_dst);
	}
	void put_image(const Image & image, const Rect & dst)
	{
		SDL_Rect _dst = {Sint16(dst.x), Sint16(dst.y),
                         Uint16(dst.w), Uint16(dst.h)};
		put_image(image.image, NULL, &_dst);
	}

	//------------------------------------------------------------------------
	// Rectangle drawing
	//------------------------------------------------------------------------
	void put_rect( int x, int y, int w, int h, const Color & );
    void put_rect( vec2i v, int w, int h, const Color & color)
    {
        put_rect( v.x(), v.y(), w, h, color);
    }
	void put_rect(const Rect & r, const Color & c)
	{
		put_rect(r.x, r.y, r.w, r.h, c);
	}
	void put_rect( int x, int y, int w, int h, int r, int g, int b )
	{
		Color c = {Uint8(r), Uint8(g), Uint8(b)};
		put_rect(x, y, w, h, c);
	}
	void fill(const Color & c)
	{
		SDL_Rect r = surface->clip_rect;
		put_rect(r.x, r.y, r.w, r.h, c);
	}

	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	int put_polygon(int x[], int y[], int size, int r, int g, int b, bool fill = true)
	{
		Sint16 * _x = new Sint16[size];
		Sint16 * _y = new Sint16[size];

		for (int i = 0; i < size; i++)
		{
			_x[i] = x[i];
			_y[i] = y[i];
		}
		Sint16 * vx = (Sint16 *)_x;
		Sint16 * vy = (Sint16 *)_y;
		
		if (fill)
			return filledPolygonColor(surface, vx, vy, size, ((r & 0xFF) << 24) | ((g & 0xFF) <<16) | ((b & 0xFF) << 8) | 0xFF);
		else
			return polygonColor(surface, vx, vy, size, ((r & 0xFF) << 24) | ((g & 0xFF) <<16) | ((b & 0xFF) << 8) | 0xFF);
	}

	//------------------------------------------------------------------------
	// Returns pointer to SDL_Surface (only for testing)
	//------------------------------------------------------------------------
	SDL_Surface * get() { return surface; }

private:
	int _w, _h;
	friend class SingletonSurface;
//	Surface( int=WIDTH, int=HEIGHT, int=BPP, int=SDL_ANYFORMAT );
	SDL_Surface * surface;
	Uint32 bpp;
	void   (*put_pixel_fptr) ( SDL_Surface*, int, int, Uint32 );
};

#endif
