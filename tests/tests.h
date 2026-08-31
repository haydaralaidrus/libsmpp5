#ifndef LIBSMPP5_TESTS_H
#define LIBSMPP5_TESTS_H

#define CHECK(cond)                                                         \
	do {                                                                    \
		if (!(cond)) {                                                      \
			fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			return 1;                                                       \
		}                                                                   \
	} while (0)

#endif
