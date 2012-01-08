int f(int x, int y) {
  // 1-bit register.
  int a = x << 1;

  // 1-bit unsigned immediate.
  int b = 100 << 1;

  // 1-bit long immediate.
  int c = 6000 << 1;

  // Register-register.
  int d = x << y;

  // Register-unsigned-immediate.
  int e = x << 4;

  // Register-signed-immediate.
  // Shift by a negative value is undefined, and LLVM removes these.
  int f = x << -104;

  // Register-long-immediate.
  // Shift by a value greater than the width is undefined, and LLVM removes 
  // these.
  int g = y << 6000;

  return 0;
}

int main() {
  return 0;
}
