## What?

Calculates 2<sup>x</sup> where x is Q16 format fixed point fractional number. 

This algorithm, also, doesn't use any division, loop or branch instructions which makes it fast and constant run time.

## Input / Output range

* Input must be between 0 and ((32 * 65536) - 1)
* Output is greater than 0 and smaller than 2<sup>32</sup>

## Reference Implementation

```c
uint32_t pow2_reference(uint32_t x)
{
  return (uint32_t)(powf(2.0, (float)x / 65536.0));
}
```

## Core Algorithm

```c
uint32_t pow2_fast(uint32_t x)
{
  uint32_t N = x >> 16;
  uint32_t remainder = x & 0x0000FFFF;
  uint64_t scale = lutRead(remainder);  
  return (uint32_t)((scale << N) >> 16);
}
```

## Error analysis

![](./doc/error_analysis.png)

* In this particular example, 1024 length lookup table is used.
* Size of the lookup table can be increased for reduced error performance. Or, table can be read with linear interpolation for reduced error performance.
* Relative error statistics:
	- mean: *0.000328* 
	- stddev: *0.002606* 
	- variance: *0.000007*
* Relative error statistics with 64x interpolation:
	- mean: *0.000007* 
	- stddev: *0.000245* 
	- variance: *0.000000*

## Implementation

* `./scripts/lutgen_pow2.py` for lookup table generation.
* `main.c` for the *fast* and *fast & interpolated* version of the algorithm.

## Licence

```
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
// ----------------------------------------------------------------------------```