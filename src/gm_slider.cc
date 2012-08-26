/*
 *  gm_Slider.cc
 *  GUIMUP volume slider
 *  (c) 2008-2012 Johan Spee
 *
 *  This file is part of Guimup
 *
 *  GUIMUP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GUIMUP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "gm_slider.h"

gm_Slider::gm_Slider()
{
	width = 100;
	height = 8;
	current_volume = 50;
	bg_color.set("#000000");
	fg_color.set("#FFFFFF");
	render_indicator();
	override_background_color(bg_color, Gtk::STATE_FLAG_NORMAL);
	prev_pos = 0;
	pbar_img.set(pxb);
	pbar_img.set_alignment(0, 0);
	pbar_img.set_double_buffered(true);
	add(pbar_fixed);
	pbar_fixed.put(pbar_img, 0, 1);
}

int gm_Slider::get_value()
{
	return current_volume;
}

void gm_Slider::set_value(int vol)
{
	current_volume = vol;
	double fraction = (double)vol/100.0;
	int pos = (int)(fraction * width);
	if (!pos > 0)
		pos = 1; // 'scale_simple' doesn't like 0's
	if (pos > width -1)
		pos = width-1; // 1 px border at the end
	if (pos != prev_pos)
	{
		prev_pos = pos;
		pxb_scaled = pxb->scale_simple(pos, height-2, Gdk::INTERP_BILINEAR);
		pbar_img.set(pxb_scaled);
	}
}

void gm_Slider::set_size_request( int w, int h )
{
	width = w;
	height = h;
	Gtk::EventBox::set_size_request(w, h);
}

// FIRST CALL set_size_request() !!
void gm_Slider::set_colors( ustring bg, ustring fg ) // input: #hex format
{
	fg_color.set(fg);
	bg_color.set(bg);
	override_background_color(bg_color, Gtk::STATE_FLAG_NORMAL);
	render_indicator(); // (needs width and height)
}

void gm_Slider::render_indicator()
{
    int Xmin = 0;
    int Ymin = 0;
	int Xmax;
	if (width > 100 )
		Xmax = width/2;
	else
		Xmax = width;	
	int Ymax;
	if (height > 2 )
		Ymax = height-2;
	else
		Ymax = height;

	Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, Xmax, Ymax);
	Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(surface);
    
    Cairo::RefPtr< Cairo::LinearGradient > gradient = Cairo::LinearGradient::create (Xmin, Ymin, Xmax, Ymin);
    
    gradient->add_color_stop_rgba (0, bg_color.get_red(), bg_color.get_green(), bg_color.get_blue(), 1.00); // 1.00 is the alpha value
    gradient->add_color_stop_rgba (1, fg_color.get_red(), fg_color.get_green(), fg_color.get_blue(), 0.75); // 0.75 is the alpha value

    // Draw a rectangle and fill with gradient
    context->set_source (gradient); 
    context->rectangle (Xmin, Ymin, Xmax, Ymax);
    context->fill();

	pxb = Gdk::Pixbuf::create(surface, 0, 0, Xmax, Ymax);
}


gm_Slider::~gm_Slider()
{
}
