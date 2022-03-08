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
static inline uint32_t pow2_reference(const uint32_t x)
{
  return (uint32_t)(powf(2.0, (float)x / 65536.0));
}

// ----------------------------------------------------------------------------
static inline uint32_t lutRead_interp(const uint32_t value)
{
  // ...
  uint32_t ind = value >> 6;
  uint32_t ind_frac = value - (ind << 6);
  uint32_t ratio_6b = ind_frac;

  // ...
  uint32_t y0 = lut_pow2[(ind + 0)];
  uint32_t y1 = lut_pow2[(ind + 1)];
  int64_t diff = y1 - y0;
  
  // ...
  return (y0 + ((diff * ratio_6b) >> 6));
}

// ----------------------------------------------------------------------------
static inline uint32_t lutRead(const uint32_t value)
{
  return lut_pow2[value >> 6];
}

// ----------------------------------------------------------------------------
static inline uint32_t pow2_fast_interp(const uint32_t x)
{
  // ...
  uint32_t N = x >> 16;
  uint32_t remainder = x & 0x0000FFFF;
  uint64_t scale = lutRead_interp(remainder);  
  return (uint32_t)((scale << N) >> 16);
}

// ----------------------------------------------------------------------------
static inline uint32_t pow2_fast(const uint32_t x)
{
  // ...
  uint32_t N = x >> 16;
  uint32_t remainder = x & 0x0000FFFF;
  uint64_t scale = lutRead(remainder);  
  return (uint32_t)((scale << N) >> 16);
}

// ----------------------------------------------------------------------------
int main()
{
  const uint32_t start = 0;
  const uint32_t end = (32 * 65536) - 1;
  const uint32_t step = 1;

  // ...
  FILE* fp = fopen("./doc/results.txt", "w+");

  // ...
  printf("\r\n");
  printf("Running ...\r\n");

  // ...
  for(uint32_t x=start;x<end;x+=step)
  {
    fprintf(fp, "%u,%u,%u,%u\r\n", 
      x, pow2_fast(x), pow2_fast_interp(x), pow2_reference(x)
    );
  }

  // ...
  printf("\r\n");
  printf("Program exits.\r\n");

  // ...
  fclose(fp);
  return 0;
}