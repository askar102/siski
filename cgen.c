int loop(int i);
int main();

int loop(int i) {
	int t0;
	int t1;
	int t2;
	int t3;
	int t4;
	int t5;
	int t6;
loop:
	t0 = i;
	t1 = 10;
	t2 = t0 > t1;
	if (!t2) goto L0;
	goto end_loop;
L0:
L1:
	t3 = i;
	t4 = 1;
	t5 = t3 + t4;
	i = t5;
	goto loop;
end_loop:
	t6 = i;
	return t6;
}

int main() {
	int res;
	int t10;
	int t8;
	int t9;
	t9 = 1;
	t8 = loop(	t9);
	res = t8;
	t10 = res;
	return t10;
}

