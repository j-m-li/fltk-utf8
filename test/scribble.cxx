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
#include <FL/Fl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_Tree.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char *png_data;
int png_w;
int png_h;
unsigned char *gif_data;
int gif_w;
int gif_h;
Fl_PNG_Image *pi;

class Scribble : public Fl_Window
{
public:
  class Dot {
  public:
    int x;
    int y;
    Dot(int X, int Y) {
      x = X;
      y = Y;
    }
  };

  class Obj {
  public:
    Dot **dots;
    int alloc;
    int nb_dots;
    int width;
    int color;

    Obj(int w, int c) {
      dots = (Dot**) malloc(sizeof(Dot*) * 8);
      alloc = 8;
      nb_dots = 0;
      width = w;
      color = c;
    }

    ~Obj() {
      while (nb_dots) delete(dots[--nb_dots]);
      free(dots);
    }

    void add_dot(int X, int Y) {
      if (alloc <= nb_dots + 1) {
        alloc *= 2;
        dots = (Dot**) realloc(dots, sizeof(Dot*) * alloc);
      }
      dots[nb_dots++] = new Dot(X, Y);
    }
  };

  Obj **objs;
  int alloc;
  int nb_objs;
  
  Scribble(int W, int H) :  Fl_Window (W,H) {
    objs = (Obj**) malloc(sizeof(Dot*) * 8);
    alloc = 8;
    nb_objs = 0; 
    box(FL_FLAT_BOX);
    color(FL_WHITE);
  }
  
  ~Scribble() {
    while (nb_objs) delete(objs[--nb_objs]);
    free(objs);
  }

  void add_obj(int X, int Y) {
    static unsigned char c = 0;
    static int w = 1;
    if (w == 1) w = 3;
    else w = 1;
    c++;


    if (nb_objs + 1 >= alloc) {
      alloc *= 2;
      objs = (Obj**) realloc(objs, sizeof(Obj*) * alloc);
    }
    objs[nb_objs++] = new Obj(w, c); 
    add_dot(X, Y);
  }

  void add_dot(int X, int Y) {
    objs[nb_objs-1]->add_dot(X, Y);
  }

  void draw() {
    Fl_Window::draw();

    if (nb_objs < 1) return;

    if (damage() == FL_DAMAGE_CHILD) {
      Obj *o = objs[nb_objs-1];
      Dot *d;
      if (o->nb_dots < 1) return;
      d = o->dots[o->nb_dots-1];
      fl_color(o->color);
      fl_line_style(FL_SOLID, o->width);
      if (o->nb_dots == 1) {
         fl_point(d->x, d->y);
      } else {
        Dot *d1 = o->dots[o->nb_dots-2];
        fl_line(d1->x, d1->y, d->x, d->y);
      }
    } else {
      int i;
      
      for (i = 0;i < nb_objs; i++) {
        Obj *o = objs[i];
        int ii = o->nb_dots - 1;
        fl_color(o->color);
        fl_line_style(FL_SOLID, o->width);

        if (ii == 0) {
          Dot *d = o->dots[0];
          fl_point(d->x, d->y);
        }
        for (;ii >= 1;ii--) {
          Dot *d1 = o->dots[ii - 1];
          Dot *d2 = o->dots[ii];
          fl_line(d1->x, d1->y, d2->x, d2->y);
        } 
      }
    }
    fl_draw_image(png_data, 0, 0, png_w, png_h, 4,0);
    fl_draw_image(gif_data, w() - gif_w, 0, gif_w, gif_h, 4,0);
    pi->draw(0, h() - pi->h(), pi->w(), pi->h());
  }

  int handle(int e) {
    switch (e) {
    case FL_PUSH:
      add_obj(Fl::event_x(), Fl::event_y());
      damage(FL_DAMAGE_CHILD);
      return 1;
    case FL_DRAG:
      add_dot(Fl::event_x(), Fl::event_y());
      damage(FL_DAMAGE_CHILD);
      return 1;
    case FL_RELEASE:
      add_dot(Fl::event_x(), Fl::event_y());
      damage(FL_DAMAGE_CHILD);
      return 1;
    }
    return Fl_Window::handle(e);
  }
};

int main(int argc, char** argv) {
  Fl_GIF_Image gi("giftest.gif");
  printf("%d %d\n", gi.w(), gi.h());
  gif_w = gi.w();
  gif_h = gi.h();
  gif_data = (unsigned char *) malloc(gif_w * gif_h * 4);
  gi.to_rgba(gif_data);

  pi = new Fl_PNG_Image("pngtest.png");

  Fl_PNG_Image pn("pngtest.png");
  png_w = pn.w();
  png_h = pn.h();
  png_data = (unsigned char *) malloc(png_w * png_h * 4);
  pn.to_rgba(png_data);

  Scribble window(300,400);
  window.resizable(window);
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id:  $".
//
