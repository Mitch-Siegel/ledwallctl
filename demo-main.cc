// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
//
// This code is public domain
// (but note, once linked against the led-matrix library, this is
// covered by the GPL v2)
//
// This is a grab-bag of various demos and not very readable.
#include "led-matrix.h"

#include "pixel-mapper.h"
#include "graphics.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>

using std::min;
using std::max;

#define TERM_ERR  "\033[1;31m"
#define TERM_NORM "\033[0m"

using namespace rgb_matrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  // These are the defaults when no command-line flags are given.
  matrix_options.rows = 64;
  matrix_options.rows = 64;

  matrix_options.chain_length = 2;
  matrix_options.parallel = 1;
  matrix_options.disable_hardware_pulsing = true;
  matrix_options.hardware_mapping = "adafruit-hat";
  matrix_options.brightness = 100;

  int my_argc = 6;
  char **my_argv = (char **)malloc(my_argc * sizeof(char *));
  for(int i = 0; i < my_argc; i++)
  {
    my_argv[i] = (char *)malloc(65);
  }
  sprintf(my_argv[0], "");
  sprintf(my_argv[1], "");
  sprintf(my_argv[2], "--led-rows=64");
  sprintf(my_argv[3], "--led-cols=64");
  sprintf(my_argv[4], "--led-chain=2");
  sprintf(my_argv[5], "--led-brightness=100");
  // char *arg0 = (char *)"";
  // char *arg1 = (char *)"";
  // char *arg2 = (char *)"--led-rows=64";
  // char *arg3 = (char *)"--led-cols=64";
  // char *arg4 = (char *)"--led-chain=2";
  // char *my_argv[my_argc] = {arg0, arg1, arg2, arg3, arg4};
  if (!ParseOptionsFromFlags(&my_argc, &my_argv, &matrix_options, &runtime_opt)) {
    return 1;
  }
  // matrix_options.limit_refresh_rate_hz = 5;

  runtime_opt.gpio_slowdown = 3;

  printf("about to create rgbmatrix\n");
  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (matrix == NULL)
    return 1;
  printf("created rgbmatrix\n");

  FrameCanvas *offScreenCanvas = matrix->CreateFrameCanvas();
  // Canvas offScreenCanvas = matrix;


  printf("Size: %dx%d. Hardware gpio mapping: %s\n",
         matrix->width(), matrix->height(), matrix_options.hardware_mapping);

  

  // Set up an interrupt handler to be able to stop animations while they go
  // on. Each demo tests for while (!interrupt_received) {},
  // so they exit as soon as they get a signal.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  printf("Press <CTRL-C> to exit and reset LEDs\n");
  uint8_t r, g, b;
  r = 255;
  g = 255;
  b = 255;
  uint32_t asdf = 0;
  bool on = true;
while(!interrupt_received){
  offScreenCanvas->SetPixel(asdf & 127, asdf >> 7, 255 * on, 255 * on, 255 * on);
  if((asdf ++) > (128 * 64))
  {
    asdf = 0;
    on = !on;
  }
  /*if(r == 255)
  {
    if(g == 255)
    {
      if(b == 255)
      {
        r = 0;
        g = 0;
        b = 0;
      }
      else
      {
        b+=1;
      }
    }
    else
    {
      g+=1;
    }
  }
  else
  {
    r+=1;
  }*/
  // offScreenCanvas->Fill(r, g, b);
  if(asdf % 7 == 0)
    offScreenCanvas = matrix->SwapOnVSync(offScreenCanvas);
          


  }
  // delete offScreenCanvas;
  
  printf("Received CTRL-C. Exiting.\n");
   offScreenCanvas->Fill(0, 0, 0);

  offScreenCanvas = matrix->SwapOnVSync(offScreenCanvas);
  return 0;
}
