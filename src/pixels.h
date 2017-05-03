#ifndef CEL_PIXELS_H
#define CEL_PIXELS_H

#include <cstdlib>  // for size_t
#include <functional> // for std::function

#include <GL/glew.h>    // must include before glfw
#include <GLFW/glfw3.h> // only for defining the Canvas type

/**
 * pixiels.h -- This module provides a wrapper for a naive set of OpenGL API.
 * Bascially, you can use it to create a (or more) OpenGL window, send a pixel
 * buffer to the window context (stored in the Grapihcs Card memory), and
 * then render the pixiels as-it-is on the screen.
 *
 * TODO: implement usefull call-back function setting api, and remove
 *    gl_window_should_open.
 * TODO: make this thread-safe
 */

namespace CEL {

/*
 * Use WindowID to identify different drawable window. This interface supports
 * multiple windows (thanks to GLFW3)
 */
typedef GLFWwindow* WindowID;

/*
 * gl_init -- Initialize the wrapper.
 * Return 0 if successed. It is safe to call this multiple times.
 */
int gl_init();

/*
 * gl_create_window -- Open a window, which is ready for drawing pixels.
 *   width: width of the window
 *   height: height of the window
 *   title: the title of the window
 */
WindowID gl_open_window(std::size_t width, std::size_t height,
  const char* title);

/*
 * gl_get_buffer_size -- Retrive the most up-to-datebuffer size for the given
 * window. Useful for preparing a pixel buffer.
 */
void gl_get_buffer_size(WindowID window,
  std::size_t &width, std::size_t &height);

/*
 * gl_draw -- Render the pixel buffer on the OpenGL windows and display it.
 */
void gl_draw(WindowID window, char unsigned *pixels,
  std::size_t buffer_w, std::size_t buffer_h);

/*
 * gl_set_key_callback -- Set the callback function when keys are pressed
 * by the user. Useful into making interaction.
 * TODO: implement me
 */
void gl_set_key_callback(WindowID window);

/*
 * gl_set_scroll_callback -- Similar to above. Unique for a window.
 */
typedef std::function<void (double, double)> ScrollFunc;
void gl_set_scroll_callback(WindowID window, ScrollFunc func);

/*
 * gl_poll_events -- Poll the events and call the callback functions.
 */
void gl_poll_events();

/*
 * gl_wait_events -- Wait for an event to return. Useful for updating static
 * scene.
 */
void gl_wait_events();

/*
 * gl_window_is_open -- Return true if the window is set to be closed (by
 * checking the state of an internal closing flag)
 */
bool gl_window_should_open(WindowID window);

/*
 * gl_destroy_window -- Release the resource of the window object.
 * Return -1 if the window is not valid (or already closed)
 */
int gl_close_window(WindowID window);

/*
 * gl_terminate -- Finish using the pixel library. Will release resouces.
 */
void gl_terminate();

} // namespace CEL

#endif // CEL_PIXELS_H
