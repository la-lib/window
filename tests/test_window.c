#define LA_WINDOW_IMPLEMENTATION
#include "../la_window.h"

static void on_close(law_Window window, law_Data* win_data) {
  win_data->running = 0;
  law_exit(0);
}

static void another_maximize(law_Window window, law_Data* win_data);

static void on_maximize(law_Window window, law_Data* win_data) {
  law_maximize(window);
  law_setTitle(window, L"Maximized Window");
  win_data->event.window.maximize = another_maximize;
}

static void another_maximize(law_Window window, law_Data* win_data) {
  law_maximize(window);
  law_setTitle(window, L"Maximized(another) Window");
  win_data->event.window.maximize = on_maximize;
}

int main(int argc, char *argv[]) {

  // Create the window
  law_Window win = law_create(400, 100, L"Window 1", NULL);
  if (!win) return 1;

  law_show(win);

  law_Data* windata = law_getData(win);
  windata->event.window.close = on_close;
  windata->event.window.maximize = on_maximize;


  while (windata->running) {
    law_update(NULL);
  }

  law_destroy(win);
  
  return 0;
}