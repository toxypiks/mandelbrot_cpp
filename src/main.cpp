#include <stdio.h>
#include <chrono>
#include <iostream>
#include "toojpeg.h"
#include <fstream>
#include <thread>
#include <future>

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
  size_t start_x;
  size_t length_x;
  size_t start_y;
  size_t length_y;
  size_t resolution_x;
  size_t resolution_y;
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
/*
  size_t start_x;
  size_t length_x;
  size_t start_y;
  size_t length_y;
*/

unsigned int* mandelbrot_thread_fct(values data) {

  unsigned int* result =(unsigned int*)malloc(data.resolution_y * data.resolution_x * sizeof(unsigned int));
  
  for (unsigned int y = 0; y < data.resolution_y; y++){
    for (unsigned int x = 0; x < data.resolution_x; x++){
      double x_real = data.length_x * x/data.resolution_x + data.start_x;
      double y_real = data.length_y * y/data.resolution_y + data.start_y;
      unsigned int value = mandelbrot_point(x_real, y_real, 2.0, 255);
      result[y * data.resolution_x + x] = value;
    }
  }
  return result;
}

// write a single byte compressed by tooJpeg
void myOutputFct(unsigned char byte) {
  myFile << byte;
}

void mandelbrot(){
  // get 1000x1000 pixels of 3 values RGB on the heap
  unsigned char* image_pixel = new unsigned char[1000*1000*3];

  values mandelbrot_data1{ 0, 1000, 0, 250, 1000, 250 };
  values mandelbrot_data2{ 0, 1000, 250, 250, 1000, 250 };
  values mandelbrot_data3{ 0, 1000, 500, 250, 1000, 250 };
  values mandelbrot_data4{ 0, 1000, 750, 250, 1000, 250 };
  
  std::future<unsigned int*> f1 = std::async(mandelbrot_thread_fct,mandelbrot_data1);
  std::future<unsigned int*> f2 = std::async(mandelbrot_thread_fct,mandelbrot_data2);
  std::future<unsigned int*> f3 = std::async(mandelbrot_thread_fct,mandelbrot_data3);
  std::future<unsigned int*> f4 = std::async(mandelbrot_thread_fct,mandelbrot_data4);

  unsigned int* result1 = f1.get();
  unsigned int* result2 = f2.get();
  unsigned int* result3 = f3.get();
  unsigned int* result4 = f4.get();

  //todo alle results in pixmap kopieren
  
  //  bool ok = TooJpeg::writeJpeg(myOutputFct , image_pixel, 1000, 1000,
  //                    true, 90, false, "toojpeg mandelbrot example");
  //printf("error? %d \n",ok);
  delete[] result1;
  delete[] result2;
  delete[] result3;
  delete[] result4;
}


int main() {
  auto start = std::chrono::steady_clock::now();
  mandelbrot();
  auto ende = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ende - start).count();
  std::cout << "Zeitdauer: " << duration << std::endl;
}


