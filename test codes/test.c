int test() {
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int i = 0, sum = 0;

	for (i = 0; i < 10; i++) {
		sum += a[i];
	}

	return sum;
}

int main() {
	return test();
}
