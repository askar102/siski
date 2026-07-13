int foo(int a = 10, int b) {
    return a + b;
}

int main() {
    int r = 0;
    bar(1, 2, -3);
    goto skip;
    r = 999;
    skip:
    return r;
}