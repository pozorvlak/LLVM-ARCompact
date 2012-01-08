int f(int x, int y) {
  // Register-register.
  int a = x + y;

  // Register-unsigned-immediate.
  int b = x + 4;

  // Register-signed-immediate.
  int c = x + -104;

  // Register-long-immediate.
  int d = x + 6000;

  return d;
}

int main() {
  return 0;
}
