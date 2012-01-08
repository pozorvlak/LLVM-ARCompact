// This attempts to test both the calling of functions, and 
// arguments going in and out of those functions.

int f(int x) {
  return x + 2;
}

int g() {
  int y = 2;
  return f(y);
}

int main() {
  int x = 5;
  int y = g();
  return x + y;
}
