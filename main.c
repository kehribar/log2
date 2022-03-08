// ----------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2022 <ihsan@kehribar.me>
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to dealin the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lut.h"

// ----------------------------------------------------------------------------
static inline int32_t log2_reference(const uint32_t x)
{
  return (int32_t)(log2f(x) * 65536.0);
}

// ----------------------------------------------------------------------------
static inline uint32_t lutRead_interp(const uint32_t value)
{
  // ...
  uint32_t ind = value >> 21;
  uint32_t ind_frac = value - (ind << 21);
  uint32_t ratio_16b = ind_frac >> 5;

  // ...
  int32_t y0 = lut_log2[(ind + 0)];
  int32_t y1 = lut_log2[(ind + 1)];
  int64_t diff = y1 - y0;
  
  // ...
  return (y0 + ((diff * ratio_16b) >> 16));
}

// ----------------------------------------------------------------------------
static inline uint32_t lutRead(const uint32_t value)
{
  return lut_log2[value >> 21];
}

// ----------------------------------------------------------------------------
static inline int32_t log2_fast_interp(const uint32_t x)
{ 
  // ...  
  uint32_t xlz = __builtin_clz(x);
  uint32_t xbits = 31 - xlz;
  uint32_t xremainder = x & ~(1 << xbits);
  uint32_t xremainder_big = xremainder << xlz;
  
  // ...
  uint32_t result = 0;
  result += (xbits << 16);
  result += lutRead_interp(xremainder_big);
  return result;  
}

// ----------------------------------------------------------------------------
static inline int32_t log2_fast(const uint32_t x)
{ 
  // ...  
  uint32_t xlz = __builtin_clz(x);
  uint32_t xbits = 31 - xlz;
  uint32_t xremainder = x & ~(1 << xbits);
  uint32_t xremainder_big = xremainder << xlz;

  // ...
  uint32_t result = 0;
  result += (xbits << 16);
  result += lutRead(xremainder_big);
  return result;  
}

// ----------------------------------------------------------------------------
int main()
{
  const uint32_t start = 10;
  const uint32_t end = (1UL << 32) - 16384;
  const uint32_t step = 10;

  // ...
  FILE* fp = fopen("./doc/results.txt", "w+");

  // ...
  printf("\r\n");
  printf("Running ...\r\n");

  // ...
  int32_t max_err = 0;
  int32_t max_err_interp = 0;
  for(uint32_t x=start;x<end;x+=step)
  {
    // ...
    int32_t err = abs(log2_fast(x) - log2_reference(x));
    if(err > max_err)
    {
      max_err = err;
    }

    // ...
    int32_t err_interp = abs(log2_fast_interp(x) - log2_reference(x));
    if(err_interp > max_err_interp)
    {
      max_err_interp = err_interp;
    }

    // ...
    if((x % 1000) == 0)
    {
      fprintf(fp, "%u,%u,%u,%u\r\n", 
        x, log2_fast(x), log2_fast_interp(x), log2_reference(x)
      );
    }    
  }

  // ...
  printf("\r\n");
  printf("Max error: %5d / 65536\r\n", max_err);
  printf("Max error: %5d / 65536 (interpolated version)\r\n", max_err_interp);

  // ...
  printf("\r\n");
  printf("Program exits.\r\n");

  // ...
  fclose(fp);
  return 0;
}