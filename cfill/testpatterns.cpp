#ifndef TEST_PATTERNS
#define TEST_PATTERNS
namespace tpats {
	using std::array;
	array<char,25> p1 = {
		79,88,88,79,79,
		79,79,88,79,79,
		79,79,79,88,79,
		88,88,79,79,79,
		79,79,79,88,88
	};
	const array<array<char,25>*,1> as = {&p1};
}

#endif
