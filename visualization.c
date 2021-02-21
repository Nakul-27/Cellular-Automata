#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// TODO: Look into stdarg
void panic_errno(const char *fmt, ...) {
  fprintf(stderr, "ERROR: ");

  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fprintf(stderr, ": %s\n", strerror(errno));

  exit(1);
}

const char* slurp_file(const char *file_path) {
#define SLURP_FILE_PANIC panic_errno("Could not read file `%s`", file_path);
  FILE *f = fopen(file_path, "r");
  if (f == NULL) SLURP_FILE_PANIC;
  if (fseek(f, 0, SEEK_END) < 0) SLURP_FILE_PANIC;

  long size = ftell(f);
  if (size < 0) SLURP_FILE_PANIC;

  // Need to null terminate the buffer
  char *buffer = malloc(size + 1);
  if (buffer == NULL) SLURP_FILE_PANIC;

  if (fseek(f, 0, SEEK_SET) < 0) SLURP_FILE_PANIC;

  fread(buffer, 1, size, f);

  if (ferror(f) < 0) SLURP_FILE_PANIC;

  buffer[size] = '\0';

  if (fclose(f) < 0) SLURP_FILE_PANIC;

  return buffer;
#undef SLURP_FILE_PANIC
}

GLuint compile_shader_file(const char *file_path, GLenum shader_type) {
  GLuint shader = glCreateShader(shader_type);
  const GLchar *source = slurp_file(file_path);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLchar message[1024];
    GLsizei message_size = 0;
    glGetShaderInfoLog(shader, sizeof(message), &message_size, message);
    fprintf(stderr, "%.*s", message_size, message);
    exit(1);
  }

  return shader;
}

int main() {
  // Initialize the library.
  if (!glfwInit()) {
    fprintf(stderr, "Error: Could not init GLFW");
    // exit(1);
    return -1;
  }

  // Create a windows mode window and it's OpenGL context
  GLFWwindow * const window = glfwCreateWindow(
                                SCREEN_WIDTH, 
                                SCREEN_HEIGHT,
                                "Automata", 
                                NULL, 
                                NULL);
  if (window == NULL) {
    fprintf(stderr, "Error: Could not create a window.");
    glfwTerminate();
    // exit(1);
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    // TODO: Look into because this function is not working.
    // glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Look into this because I just got it from the documentation.
    // render(window);
    
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
