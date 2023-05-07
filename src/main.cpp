#include <stdio.h>
#include <chrono>
#include <iostream>
#include "toojpeg.h"
#include <fstream>
#include <thread>

std::ofstream myFile("mandelbrot.jpg", std::ios_base::out | std::ios_base::binary);

// auflösung 1000x1000
// Todo: Auflösung mit als parameter angeben
struct values{
  unsigned char* image_pixel;
  size_t start_x;
  size_t length_x;
  size_t start_y;
  size_t length_y;
  size_t resolution;
};

unsigned int mandelbrot_point(
    double x,
    double y,
    double threshold,
    unsigned int iterations
) {
  double z_re = 0.0;
  double z_img = 0.0;
  for (unsigned int i = 0; i < iterations; i++) {
    double z_re_old = z_re;
    z_re = (z_re * z_re) - (z_img * z_img) + x;
    z_img = 2 * (z_re_old * z_img) + y;
    if ((z_re * z_re) + (z_img * z_img) > (threshold * threshold)) {
      return i;
    }
  }
  return 0;
}

void mandelbrot_thread_fct(values data) {

  unsigned int row_size = 3 * 1000;
  unsigned int column_size = 3;

  for (unsigned int y_coord = data.start_y; y_coord < data.start_y + data.length_y; y_coord++){
    for (unsigned int x_coord = data.start_x; x_coord < data.start_x + data.length_x; x_coord++){
      double x = x_coord/500.0 - 1.5;
      double y = y_coord/500.0 - 1;
      unsigned int value = mandelbrot_point(x,y,2.0,255);
      // offeset = 3* (1000)*y_coord + x_coord)
      // pixel(offset +3))
      data.image_pixel[row_size * y_coord + column_size * x_coord + 0 ] = value; // r
      data.image_pixel[row_size * y_coord + column_size * x_coord + 1 ] = value; // g
      data.image_pixel[row_size * y_coord + column_size * x_coord + 2 ] = value; // b
    }
  }

}

// write a single byte compressed by tooJpeg
void myOutputFct(unsigned char byte) {
  myFile << byte;
}

void mandelbrot(){
  // get 1000x1000 pixels of 3 values RGB on the heap
  unsigned char* image_pixel = new unsigned char[1000*1000*3];

  values mandelbrot_data1{image_pixel, 0, 1000, 0, 250, 0 };
  values mandelbrot_data2{image_pixel, 0, 1000, 250, 250, 0 };
  values mandelbrot_data3{image_pixel, 0, 1000, 500, 250, 0 };
  values mandelbrot_data4{image_pixel, 0, 1000, 750, 250, 0 };
  std::thread t1(mandelbrot_thread_fct,mandelbrot_data1);
  std::thread t2(mandelbrot_thread_fct,mandelbrot_data2);
  std::thread t3(mandelbrot_thread_fct,mandelbrot_data3);
  std::thread t4(mandelbrot_thread_fct,mandelbrot_data4);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  bool ok = TooJpeg::writeJpeg(myOutputFct , image_pixel, 1000, 1000,
                      true, 90, false, "toojpeg mandelbrot example");
  printf("error? %d \n",ok);
  delete[] image_pixel;
}


int main() {
  auto start = std::chrono::steady_clock::now();
  mandelbrot();
  auto ende = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ende - start).count();
  std::cout << "Zeitdauer: " << duration << std::endl;
}
