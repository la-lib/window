/**
 * @file la_window.h
 * @brief Easy-to-use window library for C/C++.
 *
 * This file is part of the la-window project by the
 * la-lib organization <https://github.com/la-lib>
 * For more information visit <https://lalib.eu>
 *
 * @section license
 *
 * Copyright (C) 2024 la-lib and contributors.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of either:
 * 
 * 1) the GNU General Public License as published by the Free Software Foundation,
 *    either version 3 of the License, or (at your option) any later version, or
 * 
 * 2) the GNU Lesser General Public License as published by the Free Software Foundation,
 *    either version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License or the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */


// ------------------- Usage -------------------
/*
  To use the library:
   - you need once define 'LA_WINDOW_IMPLEMENTATION' 
     before including the header in one of your source files.
*/


#ifndef __LA_WIN_HEADER_GUARD
#define __LA_WIN_HEADER_GUARD

#include <assert.h> // For assert
#include <stdlib.h> // For malloc, free

#pragma region Declaration

/**
* @brief The error code for the library.
* 
* This variable stores the error code for the library.
* It is set to 0 if no error occurred, and a non-zero value
* if an error occurred during the library's operation.
* 
* @note The error code is global and shared across all windows.
* 
* @see enum law_ErrorCode
* @see law_getErrorMsg(unsigned int)
*/
unsigned int law_error = 0;

#ifdef __cplusplus
extern "C" {
#endif

// Pointer to the window
typedef void* law_Window;

/** 
 * @brief Create a window.
 * @param width The width of the window,
 * @param height The height of the window,
 * @param title The title of the window (optional),
 * @param parent The parent window (optional).
 * @return The window (`law_Window`). 
 */
law_Window law_create(int width, int height, const wchar_t* title, law_Window parent);

/**
 * @brief Destroy the window and free resources.
 * @param window The window. */
void law_destroy(law_Window window);

/** 
 * @brief Set the title of the window.
 * @param window The window,
 * @param title The title of the window. */
void law_setTitle(law_Window window, const wchar_t* title);

/** 
 * @brief Get the title of the window.
 * @param window The window.
 * @return The title of the window. */
const wchar_t* law_getTitle(law_Window window);

/**
 * @brief Set the size of the window.
 * @param window The window,
 * @param width The width of the window,
 * @param height The height of the window. */
void law_setSize(law_Window window, int width, int height);

/**
 * @brief Get the size of the window.
 * @param window The window,
 * @param width The width of the window,
 * @param height The height of the window. */
void law_getSize(law_Window window, int* width, int* height);

/** 
 * @brief Set the position of the window.
 * @param window The window,
 * @param x The x position of the window,
 * @param y The y position of the window. */
void law_setPos(law_Window window, int x, int y);

/**
 * @brief Get the position of the window.
 * @param window The window,
 * @param x The x position of the window,
 * @param y The y position of the window. */
void law_getPos(law_Window window, int* x, int* y);

/**
 * @brief Show the window.
 * @param window The window. */
void law_hide(law_Window window);

/**
 * @brief Hide the window.
 * @param window The window. */
void law_show(law_Window window);

/**
 * @brief Minimize the window.
 * @param window The window. */
void law_minimize(law_Window window);

/**
 * @brief Maximize the window.
 * @param window The window. */
void law_maximize(law_Window window);

// ------------------- Monitors -------------------
#pragma region _monitors

typedef void* law_Monitor;

/**
 * @brief Get the primary monitor.
 * @return The primary monitor. */
law_Monitor law_getPrimaryMonitor();

#pragma endregion _monitors


// ------------------- Events -------------------
#pragma region _events

typedef struct law_Data law_Data;

typedef void (*__law_FuncWinData)(law_Window, law_Data*);                 // void func(law_Window window, law_Data data)
typedef void (*__law_FuncWinDataInt)(law_Window, law_Data*, int);         // void func(law_Window window, law_Data data, int)
typedef void (*__law_FuncWinDataIntInt)(law_Window, law_Data*, int, int); // void func(law_Window window, law_Data data, int, int)
typedef void (*__law_FuncWinDataUintIntIntInt)(law_Window, law_Data*, unsigned int, int, int, int); // void func(law_Window window, law_Data data, int, int, int)
typedef void (*__law_FuncWinDataStr)(law_Window, law_Data*, const char*); // void func(law_Window window, law_Data data, const char*)


#pragma region _exit_func
void (*__law_exit_func)(int) = NULL;

/**
 * @brief Set function to be called when application quits.
 *
 * @param function_pointer Pointer to the function to be called when 
 * the application quits. Has the signature `void func(int)`, 
 * where the integer parameter is the exit code.
 *
 * @note Exit code `0` usually represents a normal exit, while 
 * non-zero values signify an error or abnormal termination.
 */
void law_setAppExit(void(*function_pointer)(int)) {
   __law_exit_func = function_pointer;
}

/**
* @brief Get function to be called when application quits.
* @return If not set, returns `NULL`. Otherwise, returns the function pointer.
* 
* @note Exit code `0` usually represents a normal exit, while
* non-zero values signify an error or abnormal termination.
*/
void(*law_getAppExit())(int) {
  return __law_exit_func;
}

void law_exit(int exit_code);

#pragma endregion _exit_func

// Window events
typedef struct /*law_WindowEvents*/ {
  __law_FuncWinData destroy;      // The window has been destroyed and resources are being freed
  __law_FuncWinData close;        // The window is being closed by the user or system request
  __law_FuncWinDataIntInt resize; // The window size has changed (resized)
  __law_FuncWinDataIntInt move;   // The window has been moved to a different position on the screen
  __law_FuncWinData focus;        // The window has gained input focus
  __law_FuncWinData unfocus;      // The window has lost input focus
  __law_FuncWinData redraw;       // The window needs to be redrawn (e.g., after a resize or exposure)
  __law_FuncWinData minimize;     // The window has been minimized (iconified)
  __law_FuncWinData maximize;     // The window has been maximized (expanded)
  __law_FuncWinData show;         // The window is now visible on the screen
  __law_FuncWinData hide;         // The window is now hidden from the screen
  __law_FuncWinDataStr file_drop; // (currently not implemented on any platform) A file has been dropped into the window from an external source
  __law_FuncWinDataIntInt touch;  // (currently not implemented on any platform) A touch event occurred within the window
} law_WindowEvents;

// Keyboard events
typedef struct /*law_KeyboardEvents*/ {
  __law_FuncWinDataInt down; // A key has been pressed while the window is focused
  __law_FuncWinDataInt up;   // A key has been released while the window is focused
} law_KeyboardEvents;

// Mouse events
typedef struct /*law_MouseEvents*/ {
  __law_FuncWinDataIntInt move; // The mouse cursor has moved within the window's client area
  __law_FuncWinDataInt down;    // A mouse button has been pressed within the window
  __law_FuncWinDataInt up;      // A mouse button has been released within the window
  __law_FuncWinDataInt wheel;   // The mouse wheel has been scrolled within the window
} law_MouseEvents;

#define LAW_MOUSE_LEFT 1   // Mouse left button
#define LAW_MOUSE_RIGHT 2  // Mouse right button
#define LAW_MOUSE_MIDDLE 3 // Mouse middle button (scroll wheel click)
#define LAW_MOUSE_X1 4     // Mouse X1 button (additional mouse button)
#define LAW_MOUSE_X2 5     // Mouse X2 button (additional mouse button)

/**
 * @brief The events structure for the window.
 * 
 * 
 * @note Use `law_getEvents(law_Window)` to get the events structure for the window. 
 * */
typedef struct /*law_Events*/ {
  /**
   * @brief The window events.
   * 
   * Contains functions to handle events related to the window itself.  */
  law_WindowEvents window; 
  
  /**
   * @brief The keyboard events.
   * 
   * Contains functions to handle keyboard events related to the window.
   * @note Use `LAW_KEY_*` for keys (e.g., `LAW_KEY_A`, `LAW_KEY_B`, etc.). */
  law_KeyboardEvents key;
  
  /**
   * @brief The mouse events.
   * 
   * Contains functions to handle mouse events related to the window.
   * @note Use `LAW_MOUSE_*` for mouse buttons (e.g., `LAW_MOUSE_LEFT`, `LAW_MOUSE_RIGHT`, etc.). */
  law_MouseEvents mouse;

  /**
  * @brief (currently implemented on Windows only) The pen down event.
  * 
  * A pen event occurred within the window.
  * 
  * @param window The window where the pen event occurred.
  * @param id The ID of the pen (unsigned int)
  * @param pressure The pressure of the pen (0-1024) (int)
  * @param tilt_x The tilt of the pen on the x-axis (int)
  * @param tilt_y The tilt of the pen on the y-axis (int)
  * 
  * @note Windows API added support for pen events in Windows 8.
  */
  __law_FuncWinDataUintIntIntInt pen;
} law_Events;

/**
 * @brief Structure containing the parameters for a window.
 * 
 * The `law_Data` structure provides direct access to essential
 * window-related parameters, allowing users to modify or interact with
 * the window's event handling and user-specific data. This structure 
 * eliminates the need for explicit getter and setter functions by 
 * offering direct access to its members.
 */
struct law_Data {
  /**
   * @brief The events structure associated with the window.
   *
   * This member contains all the event handling mechanisms for the window,
   * such as window-specific events, keyboard events, mouse events, and
   * the quit event handler. You can directly access and modify this
   * structure to handle or customize events as needed.
   */
  law_Events event;

  /**
  * @brief Flag indicating whether the window is closed.
  *
  * This member is set to non-zero number when the window is closed.
  * 0 is true (window is open), and non-zero is false (window is closed).
  * If window is hidden, but not destroyed, this flag will still be set to 0.
  */
  int running;

  /**
   * @brief Pointer to user-defined data associated with the window.
   *
   * This member allows you to attach any custom data to the window instance.
   * It can be used to store application-specific information, settings,
   * or states relevant to your application. You can directly assign
   * and retrieve this pointer without the need for set/get functions.
   *
   * @attention Ensure window is not destroyed before accessing this pointer.
   *
   * @note Ensure the data type matches your application's requirements
   * when accessing or modifying this pointer. For example, if you are
   * using a custom structure `WindowSettings`, you might cast it as:
   * `WindowSettings* settings = (WindowSettings*)params->user_data;`
   */
  void* user_data;
};

/**
 * @brief Retrieve the parameters structure for the window.
 *
 * This function provides access to the `law_Data` structure associated
 * with a specific window. The structure contains direct references to the
 * window's event handling mechanism and user-defined data, allowing for
 * customization and interaction with the window’s event system.
 *
 * @param window The window for which the parameters are to be retrieved.
 * @return A const pointer to the `law_Data` structure associated with the window.
 *         Provides direct access to both the event structure and
 *         user-defined data for the window.
 */
law_Data* law_getData(law_Window window);

/**
 * @brief Process window events.
 * @param window The window or NULL to process all windows. */
void law_update(law_Window window);

/**
 * @brief Initialize the events structure with empty functions.
 * 
 * This function initializes the events structure with empty functions
 * that do nothing when called. It's useful for setting up the events
 * structure before assigning custom event handlers.
 * 
 * @attention This function should be called before using the events structure
 * @param events The events structure. */
void law_initEvents(law_Events* events) {
  events->window.destroy = NULL;
  events->window.close = NULL;
  events->window.resize = NULL;
  events->window.move = NULL;
  events->window.focus = NULL;
  events->window.unfocus = NULL;
  events->window.redraw = NULL;
  events->window.minimize = NULL;
  events->window.maximize = NULL;
  events->window.show = NULL;
  events->window.hide = NULL;
  events->window.file_drop = NULL;
  events->window.touch = NULL;

  events->key.down = NULL;
  events->key.up = NULL;

  events->mouse.move = NULL;
  events->mouse.down = NULL;
  events->mouse.up = NULL;
  events->mouse.wheel = NULL;

  events->pen = NULL;
}

#pragma endregion _events

#pragma region _errors

enum law_ErrorCode
{
  LAW_ERROR_NONE = 0,
  LAW_ERROR_CREATE_WINDOW,
  LAW_ERROR_ALLOCATE_MEMORY_FOR_PARAMS,
  LAW_ERROR_REGISTER_WINDOW_CLASS, // for Windows OS only
};

const char* law_getErrorMsg(unsigned int error_code);

#pragma endregion _errors

#pragma endregion Declaration

#pragma region Implementation




// ------------------- Windows Implementation -------------------
#pragma region win32
#ifdef _WIN32  // Windows-specific includes and code

// Define 'LA_WINDOW_IMPLEMENTATION' in your source file 
// before including this header to create the implementation.
#ifdef LA_WINDOW_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* IF YOU ARE USING GOLINK, please, define <_GOLINK> macro (GoLink is a linker for Windows)
        website: https://www.godevtool.com/
        archive: http://web.archive.org/web/20240527134555/http://www.godevtool.com/
        
   Attention: You can use GetCommandLine() to get the command line arguments, -
     - but you can't use argc and argv:    

   LPSTR commandLine = GetCommandLine(); 
   MessageBox(NULL, commandLine, "Command Line", MB_OK);   
*/
#if defined(_GOLINK) || defined(_MSC_VER) && !defined(_CONSOLE) // If using GoLink or MSVC without console
  #define main(...) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif // _GOLINK

#define LA_KEY_LEFT 

#pragma region _events

// macro 'EVENT' will be undefined after wrappers below
#define EVENT ((law_Events*)GetWindowLongPtrW(window, GWLP_USERDATA))

static LRESULT CALLBACK __law_wrapperCreate(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Allocating memory for the window parameters
  law_Data* win_data = (law_Data*)malloc(sizeof(law_Data));
  if (win_data == NULL) {
    assert(0 && "Failed to allocate memory for window parameters");
    DestroyWindow((HWND)window);
    law_error = LAW_ERROR_ALLOCATE_MEMORY_FOR_PARAMS;
    return 0;
  }
  // Setting the events
  law_initEvents((law_Events*)win_data); // Initialize the events with empty functions
  win_data->running = 1; // Window is running by default
  win_data->user_data = NULL; // User data is NULL by default

  // Setting the user data
  SetWindowLongPtrW((HWND)window, GWLP_USERDATA, (LONG_PTR)win_data);
  return 0;
}
static LRESULT CALLBACK __law_wrapperDestroy(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.destroy)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.destroy((law_Window)window, win_data);

  // Freeing the memory
  free(win_data);
  
  return 0;
}
static LRESULT CALLBACK __law_wrapperClose(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.close)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.close((law_Window)window, win_data);

  return 0;
}
static LRESULT CALLBACK __law_wrapperResize(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.resize)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.resize((law_Window)window, win_data, LOWORD(lParam), HIWORD(lParam));
  return 0;
}
static LRESULT CALLBACK __law_wrapperMove(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.move)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.move((law_Window)window, win_data, LOWORD(lParam), HIWORD(lParam));
  return 0;
}
static LRESULT CALLBACK __law_wrapperFocus(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.focus)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.focus((law_Window)window, win_data);
  return 0;
}
static LRESULT CALLBACK __law_wrapperUnfocus(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.unfocus)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.unfocus((law_Window)window, win_data);
  return 0;
}
static LRESULT CALLBACK __law_wrapperRedraw(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.redraw)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->window.redraw((law_Window)window, win_data);
  return 0;
}
static LRESULT CALLBACK __law_wrapperKeyDown(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->key.down)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->key.down((law_Window)window, win_data, (int)wParam);
  return 0;
}
static LRESULT CALLBACK __law_wrapperKeyUp(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->key.up)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->key.up((law_Window)window, win_data, (int)wParam);
  return 0;
}
static LRESULT CALLBACK __law_wrapperMouseMove(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.move)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.move((law_Window)window, win_data, LOWORD(lParam), HIWORD(lParam));
  return 0;
}
static LRESULT CALLBACK __law_wrapperLButtonDown(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.down)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.down((law_Window)window, win_data, LAW_MOUSE_LEFT);
  return 0;
}
static LRESULT CALLBACK __law_wrapperRButtonDown(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.down)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.down((law_Window)window, win_data, LAW_MOUSE_RIGHT);
  return 0;
}
static LRESULT CALLBACK __law_wrapperMButtonDown(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.down)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.down((law_Window)window, win_data, LAW_MOUSE_MIDDLE);
  return 0;
}
static LRESULT CALLBACK __law_wrapperXButtonDown(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.down)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.down((law_Window)window, win_data, GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? LAW_MOUSE_X1 : LAW_MOUSE_X2);
  return 0;
}
static LRESULT CALLBACK __law_wrapperLButtonUp(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.up)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.up((law_Window)window, win_data, LAW_MOUSE_LEFT);
  return 0;
}
static LRESULT CALLBACK __law_wrapperRButtonUp(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.up)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.up((law_Window)window, win_data, LAW_MOUSE_RIGHT);
  return 0;
}
static LRESULT CALLBACK __law_wrapperMButtonUp(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.up)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.up((law_Window)window, win_data, LAW_MOUSE_MIDDLE);
  return 0;
}
static LRESULT CALLBACK __law_wrapperXButtonUp(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.up)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.up((law_Window)window, win_data,
    GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? LAW_MOUSE_X1 : LAW_MOUSE_X2);
  return 0;
}
static LRESULT CALLBACK __law_wrapperMouseWheel(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->mouse.wheel)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
  EVENT->mouse.wheel((law_Window)window, win_data, GET_WHEEL_DELTA_WPARAM(wParam));
  return 0;
}
static LRESULT CALLBACK __law_wrapperSysCommand(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  UINT command = wParam & 0xFFF0; // Masking the command to get the actual command (Windows API moment)
  // Checking the command
  if (command == SC_MINIMIZE) { // Call the minimize event
    if (!EVENT->window.minimize)
      return DefWindowProcW(window, uMsg, wParam, lParam);

    law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
    EVENT->window.minimize((law_Window)window, win_data);
    return 0;
  } 
  else if (command == SC_MAXIMIZE) 
  { // Call the maximize event
    if (!EVENT->window.maximize)
      return DefWindowProcW(window, uMsg, wParam, lParam);

    law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
    EVENT->window.maximize((law_Window)window, win_data);
    return 0;
  }
  return DefWindowProcW(window, uMsg, wParam, lParam);
}

static LRESULT CALLBACK __law_wrapperShow(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Show or hide the window based on the wParam value
  if (wParam) {
    if (!EVENT->window.show)
      return DefWindowProcW(window, uMsg, wParam, lParam);

    law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
    EVENT->window.show((law_Window)window, win_data);
  }
  else {
    if (!EVENT->window.hide)
      return DefWindowProcW(window, uMsg, wParam, lParam);

    law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
    EVENT->window.hide((law_Window)window, win_data);
  }
  return 0;
}

static LRESULT CALLBACK __law_wrapperFileDrop(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.file_drop)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  // TODO: Implement file drop event

  return 0;
}
static LRESULT __law_wrapperTouch(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->window.touch)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  UINT c_inputs = LOWORD(wParam);   // Number of touch inputs
  TOUCHINPUT touch_input[10];       // Array of touch inputs (max 10)

  if (GetTouchInputInfo((HTOUCHINPUT)lParam, c_inputs, touch_input, sizeof(TOUCHINPUT))) {
    for (UINT i = 0; i < c_inputs; i++) {
      int id = touch_input[i].dwID;       // Identifier (ID) touch
      int x = touch_input[i].x / 100;     // X (in pixels)
      int y = touch_input[i].y / 100;     // Y (in pixels)
      int dx = 0, dy = 0;                 // Delta x, y

      // If the offset is supported, calculate it (no direction in TOUCHINPUT)
      if (touch_input[i].dwMask & TOUCHEVENTF_MOVE) {
        dx = touch_input[i].x / 100 - x; // Calculate the change in x
        dy = touch_input[i].y / 100 - y; // Calculate the change in y
      }

      law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
      EVENT->window.touch((law_Window)window, win_data, x, y); // TODO: add id, dx, dy
    }

    CloseTouchInputHandle((HTOUCHINPUT)lParam);  // Free resources
  }
  return 0;
}

static LRESULT CALLBACK __law_wrapperPointerUpdate(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (!EVENT->pen)
    return DefWindowProcW(window, uMsg, wParam, lParam);

  unsigned int pointer_id = GET_POINTERID_WPARAM(wParam);
  POINTER_PEN_INFO pen_info;

  if (GetPointerPenInfo(pointer_id, &pen_info)) {
    int pressure = pen_info.pressure; // pressure of the pen (0-1024)

    // Tilt of the pen
    int tilt_x = pen_info.tiltX;
    int tilt_y = pen_info.tiltY;

    law_Data* win_data = (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
    EVENT->pen((law_Window)window, win_data, pointer_id, pressure, tilt_x, tilt_y);
  }
  return 0;
}

#undef EVENT // Remove the EVENT macro



static LRESULT CALLBACK __law_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_MOUSEMOVE: return __law_wrapperMouseMove(hwnd, uMsg, wParam, lParam);
  case WM_SIZE: return __law_wrapperResize(hwnd, uMsg, wParam, lParam);
  case WM_MOVE: return __law_wrapperMove(hwnd, uMsg, wParam, lParam);
  case WM_PAINT: return __law_wrapperRedraw(hwnd, uMsg, wParam, lParam);
  case WM_KEYDOWN: return __law_wrapperKeyDown(hwnd, uMsg, wParam, lParam);
  case WM_KEYUP: return __law_wrapperKeyUp(hwnd, uMsg, wParam, lParam);
  case WM_LBUTTONDOWN: return __law_wrapperLButtonDown(hwnd, uMsg, wParam, lParam);
  case WM_RBUTTONDOWN: return __law_wrapperRButtonDown(hwnd, uMsg, wParam, lParam);
  case WM_MBUTTONDOWN: return __law_wrapperMButtonDown(hwnd, uMsg, wParam, lParam);
  case WM_XBUTTONDOWN: return __law_wrapperXButtonDown(hwnd, uMsg, wParam, lParam);
  case WM_LBUTTONUP: return __law_wrapperLButtonUp(hwnd, uMsg, wParam, lParam);
  case WM_RBUTTONUP: return __law_wrapperRButtonUp(hwnd, uMsg, wParam, lParam);
  case WM_MBUTTONUP: return __law_wrapperMButtonUp(hwnd, uMsg, wParam, lParam);
  case WM_XBUTTONUP: return __law_wrapperXButtonUp(hwnd, uMsg, wParam, lParam);
  case WM_MOUSEWHEEL: return __law_wrapperMouseWheel(hwnd, uMsg, wParam, lParam);
  case WM_SYSCOMMAND: return __law_wrapperSysCommand(hwnd, uMsg, wParam, lParam);
  case WM_SHOWWINDOW: return __law_wrapperShow(hwnd, uMsg, wParam, lParam);
  case WM_TOUCH: return __law_wrapperTouch(hwnd, uMsg, wParam, lParam);
  case WM_POINTERUPDATE: return __law_wrapperPointerUpdate(hwnd, uMsg, wParam, lParam);
  case WM_SETFOCUS: return __law_wrapperFocus(hwnd, uMsg, wParam, lParam);
  case WM_KILLFOCUS: return __law_wrapperUnfocus(hwnd, uMsg, wParam, lParam);
  case WM_DROPFILES: return __law_wrapperFileDrop(hwnd, uMsg, wParam, lParam);
  case WM_CLOSE: return __law_wrapperClose(hwnd, uMsg, wParam, lParam);
  case WM_CREATE: return __law_wrapperCreate(hwnd, uMsg, wParam, lParam);
  case WM_DESTROY: return __law_wrapperDestroy(hwnd, uMsg, wParam, lParam);
  default: return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }
}

void law_update(law_Window window) {
  MSG msg;
  while (PeekMessageW(&msg, (HWND)window, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      if (__law_exit_func)
        __law_exit_func((int)msg.wParam);
      break;
    }
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

void law_exit(int exit_code) {
  PostQuitMessage(exit_code);
}



#pragma endregion _events

#pragma region _window

#ifndef LA_DEFAULT_WINDOW_CLASS // Used to register the window class (Windows OS only)
    #define LA_DEFAULT_WINDOW_CLASS L"cool"
#endif // LA_DEFAULT_WINDOW_CLASS

law_Window law_create(int width, int height, const wchar_t* title, law_Window parent) {
  static unsigned char class_registered = 0;
  if (!class_registered) {
    // Window class
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = __law_proc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = LA_DEFAULT_WINDOW_CLASS;

    // Registering the window class
    if (RegisterClassW(&wc) == 0) {
      assert(0 && "Failed to register window class");
      law_error = LAW_ERROR_CREATE_WINDOW;
      return NULL;
    }
    class_registered = 1;
  }

  // Creating a window
  HWND hwnd = CreateWindowExW(
    0,                                 // Optional window styles
    LA_DEFAULT_WINDOW_CLASS,           // Window class
    title,                             // Window title
    WS_OVERLAPPEDWINDOW & ~WS_VISIBLE, // Window style
    CW_USEDEFAULT, CW_USEDEFAULT,      // Position of the window
    width, height,                     // Size of the window
    (HWND)parent,                      // Parent window
    NULL,                              // Menu
    GetModuleHandleW(NULL),             // Instance handle
    NULL                               // Additional application data
  );
  if (hwnd == NULL) {
    assert(0 && "Failed to create window");
    law_error = LAW_ERROR_CREATE_WINDOW;
    return NULL;
  }

  return (law_Window)hwnd;
}

void law_destroy(law_Window window) {
  DestroyWindow((HWND)window);
}

void law_setTitle(law_Window window, const wchar_t* title) {
  SetWindowTextW((HWND)window, title);
}

const wchar_t* law_getTitle(law_Window window) {
  static wchar_t title[256]; // TODO: Use dynamic memory allocation
  GetWindowTextW((HWND)window, title, 256);
  return title;
}

void law_setSize(law_Window window, int width, int height) {
  SetWindowPos((HWND)window, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
}

void law_setPos(law_Window window, int x, int y) {
  SetWindowPos((HWND)window, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

void law_getSize(law_Window window, int* width, int* height) {
  RECT rect;
  GetClientRect((HWND)window, &rect);
  *width = rect.right - rect.left;
  *height = rect.bottom - rect.top;
}

void law_getPos(law_Window window, int* x, int* y) {
  RECT rect;
  GetWindowRect((HWND)window, &rect);
  *x = rect.left;
  *y = rect.top;
}

void law_hide(law_Window window) {
  ShowWindow((HWND)window, SW_HIDE);
}

void law_show(law_Window window) {
  ShowWindow((HWND)window, SW_SHOW);
}

void law_minimize(law_Window window) {
  ShowWindow((HWND)window, SW_MINIMIZE);
}

void law_maximize(law_Window window) {
  ShowWindow((HWND)window, SW_MAXIMIZE);
}

law_Data* law_getData(law_Window window) {
  return (law_Data*)GetWindowLongPtrW((HWND)window, GWLP_USERDATA);
}

#pragma endregion _window

#endif // LA_WINDOW_IMPLEMENTATION
#endif // _WIN32
#pragma endregion win32


const char* law_getErrorMsg(unsigned int error_code) {
  assert(error_code >= 0 && error_code < 3);

  assert(LAW_ERROR_NONE == 0);
  assert(LAW_ERROR_CREATE_WINDOW == 1);
  assert(LAW_ERROR_ALLOCATE_MEMORY_FOR_PARAMS == 2);
  assert(LAW_ERROR_REGISTER_WINDOW_CLASS == 3);

  // this variable has extra zero-terminators at the end of each string
  // and is not used in the code
  const char* __not_used__[64] = {
    "No error =)\0",             // 13 characters
    "Failed to create window\0", // 25 characters. 0 + 13 = 13 (starts at 13)
    "Failed to allocate memory for window parameters\0", // 49 characters. 13 + 25 = 38 (starts at 38)
    "Failed to register window class\0" // 33 characters. 38 + 49 = 87
  };

  const char* error_arena =
    "No error =)\0"             // 13 characters
    "Failed to create window\0" // 25 characters. 0 + 13 = 13 (starts at 13)
    "Failed to allocate memory for window parameters\0" // 49 characters. 13 + 25 = 38 (starts at 38)
    "Failed to register window class\0"; // 33 characters. 38 + 49 = 87

  const unsigned short indices[] = { 0, 13, 38, 87 };
  size_t indices_length = sizeof(indices) / sizeof(indices[0]);

  return error_code < indices_length ?
    error_arena + indices[error_code]
    :
    "Unknown error";
}

#pragma endregion Implementation

#ifdef __cplusplus
}
#endif

#endif // __LA_WIN_HEADER_GUARD