#include <stdio.h>
#include <chrono>
#include <iostream>
#include "toojpeg.h"
#include <fstream>

std::ofstream myFile("mandelbrot.jpg", std::ios_base::out | std::ios_base::binary);

// write a single byte compressed by tooJpeg
void myOutputFct(unsigned char byte) {
  myFile << byte;
} 


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

void mandelbrot(){
  // get 1000x1000 pixels of 3 values RGB on the heap
  unsigned char* image_pixel = new unsigned char[1000*1000*3];

  unsigned int row_size = 3 * 1000;
  unsigned int column_size = 3;

  for (unsigned int y_coord = 0; y_coord < 1000; y_coord++){
    for (unsigned int x_coord = 0; x_coord < 1000; x_coord++){
      double x = x_coord/500.0 - 1.5;
      double y = y_coord/500.0 - 1;
      unsigned int value = mandelbrot_point(x,y,2.0,255);
      // offeset = 3* (1000)*y_coord + x_coord)
      // pixel(offset +3))
      image_pixel[row_size*y_coord + column_size*x_coord + 0 ] = value; // r
      image_pixel[row_size*y_coord + column_size*x_coord + 1 ] = value; // g
      image_pixel[row_size*y_coord + column_size*x_coord + 2 ] = value; // b
    }
  }

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
