int foo(int huy = 20) {
    return 1;
}

int main() {
    if (10 < 11) {
        return 10 + 10;
    }
    else
    {
        foo(30);
        int res = 1;
        return res;
    }
}