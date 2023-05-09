#include <stdio.h>
#include <chrono>
#include <iostream>
#include "toojpeg.h"
#include <fstream>
#include <thread>

std::ofstream myFile("mandelbrot.jpg", std::ios_base::out | std::ios_base::binary);

class mandelbrot_window_parameter{
 public:
  void zoom_in(unsigned int x, unsigned int y);
  void zoom_out(unsigned int x, unsigned int y);
 private: 
  double x_min_bound;
  double x_max_bound;
  double y_min_bound;
  double y_max_bound;
  size_t resolution;
};

void mandelbrot_window_parameter::zoom_in(unsigned int x, unsigned int y) {
  double delta_x_bound = x_max_bound - x_min_bound;
  double delta_y_bound = y_max_bound - y_min_bound;
  double real_x = (delta_x_bound * x)/1000 - x_min_bound;
  double real_y = (delta_y_bound * (1000 - y))/1000 - y_min_bound;
  double new_delta_x = delta_x_bound/4.0;
  double new_x_min_bound = real_x - new_delta_x;
  double new_x_max_bound = real_x + new_delta_x;
  double new_delta_y = delta_y_bound/4.0;
  double new_y_min_bound = real_y - new_delta_y; 
  double new_y_max_bound = real_y + new_delta_y;

  x_min_bound = new_x_min_bound;
  x_max_bound = new_x_max_bound;
  y_min_bound = new_y_min_bound;
  y_max_bound = new_y_max_bound;

}
  
// auflösung 1000x1000
// Todo: Auflösung mit als parameter angeben
struct values{
  unsigned char* image_pixel;
  size_t start_x;
  size_t length_x;
  size_t start_y;
  size_t length_y;
  size_t resolution_x;
  size_t resolution_y;  
};


12 13|14 15
 8  9|10 11
-----+------
 4  5| 6  7
 0  1| 2  3

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
/*
  size_t start_x;
  size_t length_x;
  size_t start_y;
  size_t length_y;
*/

void mandelbrot_thread_fct(values data) {

  // unsigned int row_size = 3 * 1000;
  // unsigned int column_size = 3;

  for (unsigned int y = 0; y < data.resolution_y; y++){
    for (unsigned int x = 0; x < data.resolution_x; x++){
      double x_real = data.length_x * x/data.resolution_x + data.start_x;
      double y_real = data.length_y * y/data.resolution_y + data.start_y;
      unsigned int value = mandelbrot_point(x_real, y_real, 2.0, 255);

      /* ToDo refactor access to image
      data.image_pixel[row_size * y_coord + column_size * x_coord + 0 ] = value; // r
      data.image_pixel[row_size * y_coord + column_size * x_coord + 1 ] = value; // g
      data.image_pixel[row_size * y_coord + column_size * x_coord + 2 ] = value; // b
      */
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

  values mandelbrot_data1{image_pixel, 0, 1000, 0, 250, 1000, 250 };
  values mandelbrot_data2{image_pixel, 0, 1000, 250, 250, 1000, 250 };
  values mandelbrot_data3{image_pixel, 0, 1000, 500, 250, 1000, 250 };
  values mandelbrot_data4{image_pixel, 0, 1000, 750, 250, 1000, 250 };
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


