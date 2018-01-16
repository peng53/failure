#ifndef TEST_PATTERNS
#define TEST_PATTERNS
namespace tpats {
	char p1[26] =
		"oxxoo"
		"ooxoo"
		"oooxo"
		"xxooo"
		"oooxx"; // 0,0 all should change
	char p2[26] =
		"xxoxx"
		"xxoxx"
		"ooooo"
		"xxoxx"
		"xxoxx";
		// 12,12 all should change
		// 0,0
		// 4,4
		// 20,20
		// 24,24
		// Should change 1 corner only
	char p3[26] =
		"oxoxo"
		"oxoxo"
		"oxoxo"
		"oxoxo"
		"ooooo"; // 0,0. all should change
	char p4[26] =
		"ooooo"
		"oxxxo"
		"oxoxo"
		"xxoxo"
		"ooooo"; // 0,0. all should change
	char p5[26] =
		"xxoxo"
		"xooxo"
		"oxoxo"
		"oxoxo"
		"oooxo"; // 12,12. far right should not change
	char p6[26] =
		"abcde"
		"abcde"
		"abcde"
		"abcde"
		"abcde"; // b. c. d. e. a. should end at all a.
	char p7[26] =
		"abcde"
		"bbcde"
		"cccde"
		"dddde"
		"eeeee"; // b. c. d. e. a. should end at all a.
	char p8[26] =
		"abcde"
		"cdeab"
		"abcde"
		"cdeab"
		"abcde"; // inspect changes.
}

#endif
