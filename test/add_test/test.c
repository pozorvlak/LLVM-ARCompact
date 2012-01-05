int main() {
  // Register-register.
  int a = 1;
  int b = 2;
  int c = a + b;

  // Register-unsigned-immediate.
  int d = a + 4;
  
  // Register-signed-immediate.
  int e = a + -104;

  // Register-long-immediate.
  int f = b + 6000;
}
