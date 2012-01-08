int f(signed int x, signed int y) {
  // 1-bit register.
  signed int a = x >> 1;

  // 1-bit unsigned immediate.
  signed int b = 100 >> 1;

  // 1-bit long immediate.
  signed int c = 6000 >> 1;

  // Register-register.
  signed int d = x >> y;

  // Register-unsigned-immediate.
  signed int e = x >> 4;

  // Register-signed-immediate.
  // Shift by a negative value is undefined, and LLVM removes these.
  signed int f = x >> -104;

  // Register-long-immediate.
  // Shift by a value greater than the width is undefined, and LLVM removes 
  // these.
  signed int g = x >> 6000;
}

int main() {
  return 0;
}
