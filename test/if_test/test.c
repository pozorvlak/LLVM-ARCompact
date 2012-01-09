int f(int x, int y) {
  int a = 1;
  if (x < y) {
    a = 2;
  }

  if (x <= 0) {
    a = 4;
  } else if (a > 5) {
    a = 3;
  } else {
    a = 7;
  }

  if (a >= x) {
    a += 2;
  }

  if (a == y) {
    a += 7;
  } else if (a != x) {
    a -= 6;
  }

  return a;
}

int main() {
  return 0;
}
