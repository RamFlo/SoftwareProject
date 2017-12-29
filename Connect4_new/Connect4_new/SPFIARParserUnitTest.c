#include "unit_test_util.h"
#include "SPFIARParser.h"

static bool spParserCheckIsInt() {
	ASSERT_TRUE(spParserIsInt("1"));
	ASSERT_TRUE(spParserIsInt("11"));
	ASSERT_TRUE(spParserIsInt("111"));
	ASSERT_TRUE(spParserIsInt("-1"));
	ASSERT_TRUE(spParserIsInt("-11"));
	ASSERT_TRUE(spParserIsInt("-111"));
	ASSERT_TRUE(spParserIsInt("01"));
	ASSERT_TRUE(spParserIsInt("011"));
	ASSERT_TRUE(spParserIsInt("0111"));
	ASSERT_TRUE(spParserIsInt("-01"));
	ASSERT_TRUE(spParserIsInt("-011"));
	ASSERT_TRUE(spParserIsInt("-0111"));
	return true;
}
static bool spParserCheckParseLine() {
	SPCommand cmd;
	cmd = spParserPraseLine("add_disc 17");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("add_disc	17\n");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("	add_disc 17\n");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("aDd_disc 17");
	ASSERT_TRUE(cmd.cmd == SP_INVALID_LINE && !cmd.validArg);
	return true;
}
/*int main() {
	SPCommand cmd;
	char command[SP_MAX_LINE_LENGTH];
	char *p = command;
	const char *cp;
	fgets(command, SP_MAX_LINE_LENGTH, stdin);
	cp = p;
	cmd = spParserPraseLine(command);


	//RUN_TEST(spParserCheckIsInt);
	//RUN_TEST(spParserCheckParseLine);

	
	return 0;
}*/
