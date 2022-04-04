//
// "$Id:  $"
//
// Tree test program for the Fast Light Tool Kit (FLTK).
//
//              Copyright 2003 by O'ksi'D.
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
//  Report bugs to oksid@bluewin.ch
//


#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tree.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Sub-class an Fl_Tree to implement virtual function needed to
// handle specific data
class Directory : public Fl_Tree
{
public:

  // define a class describing our data (directory listing)
  class Entry {
  public:
    char *name;
    int nbd;
    struct dirent **dirent;
    Entry *parent;

    Entry(char *n, char is, Entry *p) {
      name = n;
      dirent = NULL; 
      parent = p;
      nbd = is;
    }

    ~Entry() {
      free(name);
      if (dirent) {
        while (nbd > 0) free(dirent[--nbd]);
        free(dirent);
      }
    }
  };
  
  // constructor
  Directory(int X, int Y, int W, int H) :  Fl_Tree (X,Y,W,H) {
    char *n = strdup("/");
    struct dirent **dir = NULL;
    int nb = fl_filename_list(n, &dir, fl_alphasort);

    // initialize our tree with the root element
    root(new Entry(n, nb, NULL));

    // set the callback called when the user clicks on elements
    callback(my_item_cb);
  }

  // the callback
  static void my_item_cb(Fl_Widget *w, void *data) {
    Directory *d = (Directory*) w;

    // get the id of the element of which the user has clicked
    int n = d->get_selection();
  
    // get a pointer to it
    Entry *e = d->get_entry(n);

    if (Fl::event_clicks()) {
      // is it already openned ?
      if (d->item_flags(n) & FLAG_OPEN) {
        // yes, close it
        d->item_close(n);
      } else {
        // no, open it
        d->item_open(n);
      }
      printf("double click on %s\n", e->name);
      Fl::event_clicks(0);
    } else {
      printf("single click on %s\n", e->name);
    }
  }

  // returns a pointer to the n element
  Entry *get_entry(int n) {return (Entry*)Fl_Tree::get_item(n);}

  // compute the full path name
  char *item_get_path(int n, int c) {
    Entry *e = get_entry(n);
    char *par[1024];
    static char b1[1024];
    Entry *p = e;
    int i = 0;
    while (p) {
      par[i++] = p->name; 
      p = p->parent;
    }
    b1[0] = 0;
    while (i > 0) {
      strcat(b1, par[--i]);
#ifndef WIN32
      strcat(b1, "/");
#endif
    }
    if (c >= 1) strcat(b1, e->dirent[c-1]->d_name);
    return b1;
  }

/////////////////////////////////////////////////////////////////
// Fonctions that must be implemented !
/////////////////////////////////////////////////////////////////

  // draw the element n, a position X,Y with clipping CX,CY,CW,CH
  virtual void item_draw(int n,int X,int Y, 
	int CX,int CY, int CW,int CH) 
  {
    // get a pointer to the n element
    Entry *e = get_entry(n);
    fl_font(0, 14);

    // draw it in red if it is selected
    fl_color((item_flags(n) & FLAG_SELECTED) ? FL_RED : FL_GREEN);
    fl_draw(e->name, X + 2 , Y + fl_height() - fl_descent()); 

    // is the element the currently selected one ?
    if (get_selection() == n) {
      // yes, draw a square around it
      dot_rect(X + 2, Y, item_width(n), item_height(n));   
    }
  }

  // compute the height and width of the element
  virtual void item_measure(int n) {
    Entry *e = get_entry(n);
    fl_font(0, 14);

    // set the element size
    item_size(n, (int)fl_width(e->name), fl_height());
  }

  // quick function to see if the element is a node or a leave.
  // called every time that Fl_Tree needs to know if the element
  // can be openned to show children elements
  virtual int item_has_children(int n) {
    Entry *e = get_entry(n);
    // if nbd < 0 then it is a leave
    return e->nbd;
  }

  // slower function to find the number of child elements of the element.
  // called when the element is openned
  virtual int item_nb_children(int n) {
    Entry *e = get_entry(n);
    char *b1;
    // is it a directory and we have not yet scanned it ?
    if (e->nbd >= 0 && e->dirent ==  NULL) {
      // yes !
      dirent **dir = NULL;
      // get its full filesystem path
      b1 = item_get_path(n, 0);

      // scan it and keep the result
      e->nbd = fl_filename_list(b1, &dir, fl_alphasort);
      e->dirent = dir;
    }

    // is it a directory with at least . and .. ? 
    if (e->dirent && e->nbd >= 2) {
      // yes !
      return e->nbd;
    }
    // NO ! this is . or ..
    return -1;
  }

  // get a pointer to the child c (starting from 1) of element n.
  // called one time after item_nb_children() for each children
  virtual void *item_get_child(int n, int c) {
    Entry *e = get_entry(n);
    Entry *m;
    char *b1;
    char is_dir = -1;

    // get the full path of the child
    b1 = item_get_path(n, c);

    // check if this is a directory
    if (fl_filename_isdir(b1)) {
      is_dir = 0;
    }

    // create a new object for the child
    m = new Entry(strdup(e->dirent[c-1]->d_name), is_dir, e); 

    // prevent scanning of . and ..
#ifdef WIN32
    if (!strcmp(m->name, "./") || !strcmp(m->name, "../")) m->nbd = -1;
#else
    if (!strcmp(m->name, ".") || !strcmp(m->name, "..")) m->nbd = -1;
#endif
    return m;  
  }

  // free the element
  // called every time that the element is not anymore in an openned node
  virtual void item_free(int n) {
    Entry *e = get_entry(n);
    delete(e);
  }
 
};

// program entry point
int main(int argc, char** argv) {
  Fl_Window window(300,400);
  Directory dir(0,0, 300, 400);
  window.resizable(window);
  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id:  $".
//
