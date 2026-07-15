#include <stdint.h>

int32_t loop(int32_t i);
int32_t test_type(int32_t a, uint8_t b);
uint16_t test_return();
void test_void();
int8_t test_const_return();
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
	int32_t t12;
	int32_t ret;
	t11 = 10;
	ret = t11;
	t12 = ret;
	return t12;
}

int8_t test_const_return() {
	int32_t t13;
	int32_t t14;
	t13 = 1;
	t14 = -t13;
	return t14;
}

int32_t main() {
	int32_t t15;
	int32_t t16;
	int32_t t17;
	int32_t t18;
	int32_t t19;
	int32_t t20;
	uint8_t t21;
	void t22;
	int32_t t23;
	int32_t res;
	uint8_t lol;
	t15 = 10;
	t16 = 10;
	t17 = t15 + t16;
	res = t17;
	t18 = 10;
	lol = t18;
	t20 = res;
	t21 = lol;
	t19 = test_type(	t20	, t21);
	res = t19;
	t22 = test_void();
	t23 = res;
	return t23;
}

