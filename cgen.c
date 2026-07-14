#include <stdint.h>

int32_t loop(int32_t i);
int32_t main();

int32_t loop(int32_t i) {
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;
	int32_t t6;
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

int32_t main() {
	int32_t t10;
	uint8_t t11;
	int32_t t12;
	int32_t t13;
	int32_t t8;
	int32_t t9;
	uint8_t lol;
	int32_t res;
	t8 = 10;
	lol = t8;
	t9 = 10;
	t11 = lol;
	t10 = loop(	t11);
	t12 = t9 + t10;
	res = t12;
	t13 = res;
	return t13;
}

