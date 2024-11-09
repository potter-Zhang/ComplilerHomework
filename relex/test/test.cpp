#include "pch.h"
#include "../relex/DFA.h"

TEST(RegexTest, Simple_1) {
	Tree tree;
	tree.create("(a|b)ccd*#");
	tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("acc", [](){}));
    EXPECT_FALSE(dfa.recognize("bc", []() {}));
    EXPECT_TRUE(dfa.recognize("accddd", []() {}));
    EXPECT_TRUE(dfa.recognize("accdd", []() {}));
}

TEST(RegexTest, Simple_2) {
    Tree tree;
    tree.create("t*(a|b)(c|e)ff#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("bcff", []() {}));
    EXPECT_FALSE(dfa.recognize("bcf", []() {}));
    EXPECT_TRUE(dfa.recognize("ttttaeff", []() {}));
    EXPECT_FALSE(dfa.recognize("ttbefff", []() {}));
}

TEST(RegexTest, Simple_3) {
    Tree tree;
    tree.create("(t*|a|bc|eff)#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("a", []() {}));
    EXPECT_TRUE(dfa.recognize("eff", []() {}));
    EXPECT_TRUE(dfa.recognize("ttttttttt", []() {}));
    EXPECT_FALSE(dfa.recognize("ttttta", []() {}));
}

TEST(RegexTest, Simple_4) {
    Tree tree;
    tree.create("(tv)*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("tv", []() {}));
    EXPECT_TRUE(dfa.recognize("tvtvtv", []() {}));
    EXPECT_FALSE(dfa.recognize("tvt", []() {}));
    EXPECT_FALSE(dfa.recognize("tvtvtt", []() {}));
}

TEST(RegexTest, Simple_5) {
    Tree tree;
    tree.create("(tv|t*a)(tv|t*a)#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_FALSE(dfa.recognize("tv", []() {}));
    EXPECT_FALSE(dfa.recognize("tvtvtv", []() {}));
    EXPECT_TRUE(dfa.recognize("tttttttttttattttttttta", []() {}));
    EXPECT_TRUE(dfa.recognize("tvtttttttta", []() {}));
    EXPECT_FALSE(dfa.recognize("ttttttttta", []() {}));
    EXPECT_TRUE(dfa.recognize("tvtttttttta", []() {}));
}

TEST(RegexTest, Simple_6) {
    Tree tree;
    tree.create("(t*t*t|a)#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("tttttttttttttttttt", []() {}));
    EXPECT_FALSE(dfa.recognize("aa", []() {}));
    EXPECT_TRUE(dfa.recognize("a", []() {}));
    EXPECT_FALSE(dfa.recognize("", []() {}));
    EXPECT_TRUE(dfa.recognize("t", []() {}));
    
}

TEST(RegexTest, Simple_7) {
    Tree tree;
    tree.create("(a*|b*)*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("abaabbaaaaabbb", []() {}));
    EXPECT_FALSE(dfa.recognize("c", []() {}));
    EXPECT_TRUE(dfa.recognize("", []() {}));
    EXPECT_TRUE(dfa.recognize("bbbbbbb", []() {}));
    EXPECT_TRUE(dfa.recognize("aababababbbbbbaaaababaaabbb", []() {}));

}

TEST(RegexTest, Simple_8) {
    Tree tree;
    tree.create("a*ba*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("ab", []() {}));
    EXPECT_FALSE(dfa.recognize("aaaaaaaaaaaaabbaaaaaa", []() {}));
    EXPECT_TRUE(dfa.recognize("b", []() {}));
    EXPECT_TRUE(dfa.recognize("aaaaaaaaaaaaaabaaaaaaaaaa", []() {}));
    EXPECT_TRUE(dfa.recognize("aaaaaaab", []() {}));

}

TEST(RegexTest, Duplicated_1) {
    Tree tree;
    tree.create("((a|b)|(a|b))#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("a", []() {}));
    EXPECT_FALSE(dfa.recognize("", []() {}));
    EXPECT_TRUE(dfa.recognize("b", []() {}));
    
}

TEST(RegexTest, Digits) {
    Tree tree;
    tree.create("(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("17649217402175", []() {}));
    EXPECT_TRUE(dfa.recognize("1", []() {}));
    EXPECT_TRUE(dfa.recognize("10", []() {}));
    EXPECT_FALSE(dfa.recognize("01", []() {}));
    EXPECT_TRUE(dfa.recognize("23794816", []() {}));

}


TEST(RegexTest, Alphabets) {
    Tree tree;
    tree.create("(D|d)(A|a)(T|t)(E|e)#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("Date", []() {}));
    EXPECT_TRUE(dfa.recognize("DATe", []() {}));
    EXPECT_TRUE(dfa.recognize("daTE", []() {}));
    EXPECT_FALSE(dfa.recognize("dated", []() {}));
    EXPECT_TRUE(dfa.recognize("DATE", []() {}));
}


TEST(RegexTest, Symbols_1) {
    Tree tree;
    tree.create("\\(1+2\\)=*3#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("(1+2)===3", []() {}));
    EXPECT_TRUE(dfa.recognize("(1+2)=3", []() {}));
    EXPECT_FALSE(dfa.recognize("1+2)=3", []() {}));
    EXPECT_FALSE(dfa.recognize("1+2=3", []() {}));
    EXPECT_FALSE(dfa.recognize("(1+2=3", []() {}));
}


TEST(RegexTest, Symbols_2) {
    Tree tree;
    tree.create("(1|2)\\(\\*\\)3*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("1(*)", []() {}));
    EXPECT_TRUE(dfa.recognize("2(*)333", []() {}));
    EXPECT_FALSE(dfa.recognize("111333", []() {}));
    EXPECT_FALSE(dfa.recognize("3", []() {}));
    EXPECT_FALSE(dfa.recognize("", []() {}));
}

TEST(RegexTest, Empty_1) {
    Tree tree;
    tree.create("1?2#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_FALSE(dfa.recognize("1", []() {}));
    EXPECT_FALSE(dfa.recognize("11", []() {}));
    EXPECT_TRUE(dfa.recognize("2", []() {}));
    EXPECT_FALSE(dfa.recognize("3", []() {}));
}

TEST(RegexTest, Comment_1) {
    Tree tree;
    tree.create("//.*#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
    dfa.minimize();
    EXPECT_TRUE(dfa.recognize("//ahewkjfhawefa", []() {}));
    EXPECT_FALSE(dfa.recognize("/lakwejf", []() {}));
    EXPECT_TRUE(dfa.recognize("///////////", []() {}));
    EXPECT_TRUE(dfa.recognize("//        ", []() {}));
}

TEST(RegexTest, Comment_2) {
    Tree tree;
    tree.create("//.*//#");
    tree.calc_followpos();
    DFA dfa;
    dfa.create(tree);
   // dfa.minimize();
    EXPECT_FALSE(dfa.recognize("//a// ", []() {}));
    EXPECT_TRUE(dfa.recognize("// //", []() {}));
    EXPECT_TRUE(dfa.recognize("///////////aaa////vvv////////", []() {}));
    EXPECT_FALSE(dfa.recognize("aaaaaaaaaabbbbbabaaaaaaaaaaaaaaaaaaaaaaaaaaab", []() {}));
}