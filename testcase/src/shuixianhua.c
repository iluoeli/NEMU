#include "trap.h"

int ans[] = {153, 370, 371, 407};

int cube(int n) {
	return n * n * n;
}

int main() {
	int n, n2, n1, n0;
	int k = 0;
	for(n = 100; n < 1000; n ++) {
		set_bp();/*
		if(n == 153)
			set_bp();*/
		n2 = n / 100;
		n1 = (n / 10) % 10;
		n0 = n % 10;

		if(n == cube(n2) + cube(n1) + cube(n0)) {
			set_bp();
			nemu_assert(n == ans[k]);
			k ++;
		}
	}
	set_bp();
	nemu_assert(n == 1000);
	nemu_assert(k == 4);

	return 0;
}
