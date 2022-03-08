# -----------------------------------------------------------------------------
# 
# 
# -----------------------------------------------------------------------------
import numpy as np

# ...
TABLE_LENGTH = 1024

# ...
fp = open("../lut.h", "w+")

# ...
fp.write("// ")
for x in range(0, 76): fp.write("-")
fp.write("\n")
fp.write("//\n")
fp.write("// ")
for x in range(0, 76): fp.write("-")
fp.write("\n")
fp.write("// Lookup table for:\n")
fp.write("//   int(np.power(2.0, (x / %d)) * 65536.0)\n" % TABLE_LENGTH)
fp.write("// ")
for x in range(0, 76): fp.write("-")
fp.write("\n")

# ...
fp.write("#include <stdint.h>\r\n")

# ...
fp.write("\r\n")
fp.write("// ----------------------------------------------------------------------------\r\n")
fp.write("const int32_t lut_pow2[%d] = {\r\n" % (TABLE_LENGTH + 1))
fp.write("  ")

# ...
for i in range(0, TABLE_LENGTH):
  
  # ...
  val = int(np.power(2.0, (i / TABLE_LENGTH)) * 65536.0)

  # ...
  fp.write("%11d, " % val)
  
  # ...
  if((i % 5) == 4):
    fp.write("\r\n  ")

# ...
fp.write("%11d" % (2 * 65535));
fp.write("\r\n};\r\n")

# ...
fp.close()
