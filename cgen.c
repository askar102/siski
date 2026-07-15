#include <stdint.h>

int32_t loop(int32_t i);
int32_t test_type(int32_t a, uint8_t b);
uint16_t test_return();
void test_void();
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

int32_t test_type(int32_t a, uint8_t b) {
	int32_t t8;
	t8 = 10;
	return t8;
}

uint16_t test_return() {
	uint16_t t10;
	int32_t t9;
	uint16_t ret;
	t9 = 10;
	ret = t9;
	t10 = ret;
	return t10;
}

void test_void() {
	int32_t t11;
	int32_t ret;
	t11 = 10;
	ret = t11;
	return ;
}

int32_t main() {
	int32_t t12;
	int32_t t13;
	int32_t t14;
	int32_t t15;
	int32_t t16;
	int32_t t17;
	int32_t t18;
	uint8_t var1;
	int32_t var2;
	int32_t var3;
	int32_t var4;
	t12 = 1;
	var1 = t12;
	t13 = 20;
	var2 = t13;
	t14 = 10;
	var3 = t14;
	var4 = ;
	t15 = var2;
	t16 = var3;
	t17 = t15 + t16;
	var4 = t17;
	t18 = 10;
	return t18;
}

