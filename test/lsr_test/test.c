int f(unsigned int x, unsigned int y) {
  // 1-bit register.
  unsigned int a = x >> 1;

  // 1-bit unsigned immediate.
  unsigned int b = 100 >> 1;

  // 1-bit long immediate.
  unsigned int c = 6000 >> 1;

  // Register-register.
  unsigned int d = x >> y;

  // Register-unsigned-immediate.
  unsigned int e = x >> 4;

  // Register-signed-immediate.
  // Shift by a negative value is undefined, and LLVM removes these.
  unsigned int f = x >> -104;

  // Register-long-immediate.
  // Shift by a value greater than the width is undefined, and LLVM removes 
  // these.
  unsigned int g = x >> 6000;

  return 0;
}

int main() {
  return 0;
}
