//
// "$Id:  $"
//
// Mouse drawing test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//


#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

extern uchar *fl_read_image(uchar *p, int   X, int   Y, int   w, int   h, int   alpha);
 
class Offscreen : public Fl_Widget
{
public:
  char *txt;
  uchar *buf;
  int bw, bh;
  Offscreen(int X, int Y, int W, int H) :  Fl_Widget (X,Y,W,H) {
	buf = NULL;  
  }

  void set_text(char *txt) {
	Fl_Offscreen id;
	uchar *rgb;
	int i;
	
	fl_font(FL_HELVETICA_BOLD, 24);
	bh = (int)fl_width(txt);
	bw = fl_height();

	// draw text to an off screen pixmap
	id = fl_create_offscreen(bh, bw);
	fl_begin_offscreen(id);
	fl_color(FL_WHITE);
	fl_rectf(0, 0, w(), h());
	fl_font(FL_HELVETICA_BOLD, 25);
	fl_color(FL_BLACK);
	fl_draw(txt, 0, fl_height() - fl_descent());

	// get an rgb image from the offscreen pixmap
	rgb = fl_read_image(NULL, 0, 0, bh, bw, 0);
	fl_end_offscreen();
	fl_delete_offscreen(id);

	buf = new uchar[bw * bh * 3];
	
	// transform as needed
	for (i = 0; i < bh; i++) {
		int j;
		for (j = 0; j < bw; j++) {
			buf[(i * bw + j) * 3] = rgb[(j * bh + i) * 3];
			buf[(i * bw + j) * 3 + 1] = rgb[(j * bh + i) * 3 + 1];
			buf[(i * bw + j) * 3 + 2] = rgb[(j * bh + i) * 3 + 2];
		}
	}

	delete[] rgb;
  }
  
  void draw() {
    // draw the transformed image
    if (!buf) set_text(txt);
    fl_draw_image(buf,x(),y(), bw, bh);
  }


};

int main(int argc, char** argv) {
  Fl_Window window(300,400);
  Offscreen o(0, 0, 300, 400); 
  o.txt = "hello";
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id:  $".
//
