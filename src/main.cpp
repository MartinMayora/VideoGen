#include <GLFW/glfw3.h>
#include <stdio.h>

bool load_frame(const char* filename, int* width, int* height, unsigned char** data);

int main(int argc, const char** argv) {
  GLFWwindow* window;
  if (!glfwInit()) {
    printf("couldn't initialize GLFW\n");
    return 1;
  }
  window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
  if (!window) {
    printf("couldn't Open Window\n");
    return 1; 
  }


  int frame_width, frame_height;
  unsigned char* frame_data;
  if (!load_frame("/mnt/c/Users/marti/Desktop/C++/VideoGen/VideoGen/Videos/Tarik.mp4", &frame_width, &frame_height, &frame_data)){
    printf("no se pudo cargar video\n");
    return 1;
  }

  //Generar la textura
glfwMakeContextCurrent(window);
  GLuint tex_handle;
  glGenTextures(1, &tex_handle);
  glBindTexture(GL_TEXTURE_2D, tex_handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               frame_data);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // seteo vista ortofrafica
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // renderizar lo que queramos
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 0);glVertex2i(0, 0);
        glTexCoord2d(1, 0);glVertex2i(100, 0);
        glTexCoord2d(1, 1);glVertex2i(100, 100);
        glTexCoord2d(0, 1);glVertex2i(0, 100);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glfwSwapBuffers(window);
    glfwWaitEvents();
  }
  glfwTerminate();
  return 0;
}