#include "gtest/gtest.h"
#include "vector.h"

double gen_1() { return static_cast<double>(rand() % 10); }

double gen_2(const unsigned long& i) {
  return static_cast<double>((i + 1) * (i + 1));
}

TEST(vec, constructor) {
  certa::vec a(4), b(5, 7.4), c(3, 2.7, 4.5, 9.8), d({1.0, 2.0, 3.0});
  EXPECT_EQ(a.str(), "<0,0,0,0>");
  EXPECT_EQ(b.str(), "<7.4,7.4,7.4,7.4,7.4>");
  EXPECT_EQ(c.str(), "<2.7,4.5,9.8>");
  EXPECT_EQ(d.str(), "<1,2,3>");
}

TEST(vec, size) {
  certa::vec a(4, 1.0);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(a.str(), "<1,1,1,1>");
  a.resize(5);
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.str(), "<1,1,1,1,0>");
  a.resize(10, 2.0);
  EXPECT_EQ(a.size(), 10);
  EXPECT_EQ(a.str(), "<1,1,1,1,0,2,2,2,2,2>");
  a.resize(3);
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.str(), "<1,1,1>");
}

TEST(vec, data) {
  srand(0);
  certa::vec a(4);
  EXPECT_EQ(a.str(), "<0,0,0,0>");
  a.fill(2.0);
  EXPECT_EQ(a.str(), "<2,2,2,2>");
  a.fill(gen_1);
  EXPECT_EQ(a.str(), "<3,6,7,5>");
  a.fill(gen_2);
  EXPECT_EQ(a.str(), "<1,4,9,16>");
  a.fill(1.0, 2);
  EXPECT_EQ(a.str(), "<1,4,1,1>");
  a.fill(0.0, 1, 3);
  EXPECT_EQ(a.str(), "<1,0,0,1>");
  a.clear();
  EXPECT_EQ(a.str(), "<0,0,0,0>");
}

TEST(vec, access) {
  certa::vec a(5, 9.0, 8.0, 6.0, 5.0, 3.0);
  EXPECT_EQ(a.str(), "<9,8,6,5,3>");
  EXPECT_EQ(a[0], 9.0);
  EXPECT_EQ(a[3], 5.0);
  EXPECT_EQ(a.at(2), 6.0);
  EXPECT_EQ(a.front(), 9.0);
  EXPECT_EQ(a.back(), 3.0);
  EXPECT_EQ(*(a.data() + 2), 6.0);
}

TEST(vec, stream){
  certa::vec a(5, 1,3,5,7,9);
  std::stringstream ss;
  ss << a;
  EXPECT_EQ(ss.str(), "<1,3,5,7,9>");
}

TEST(vec, comparison) {
  certa::vec a(3, 1, 2, 3), b(3, 3, 2, 1);
  EXPECT_EQ(a == b, false);
  EXPECT_EQ(a != b, true);
  EXPECT_EQ(a < b, true);
  EXPECT_EQ(a > b, false);
  EXPECT_EQ(a <= b, true);
  EXPECT_EQ(a >= b, false);
}

TEST(vec, scalar_operators){
  certa::vec a(3, 1, 3, 5);
  EXPECT_EQ(a + 2, certa::vec(3, 3, 5, 7));
  EXPECT_EQ(a - 2, certa::vec(3, -1, 1, 3));
  EXPECT_EQ(a * 2, certa::vec(3, 2, 6, 10));
  EXPECT_EQ(a / 2, certa::vec(3, 0.5, 3.0/2.0, 5.0/2.0));
}
TEST(vec, vector_operators){
  certa::vec a(3, 1, 3, 5), b(3, 6,4,2);
  EXPECT_EQ(a + b, certa::vec(3, 7, 7, 7));
  EXPECT_EQ(a - b, certa::vec(3, -5, -1, 3));
  EXPECT_EQ(a * b, 28);
  EXPECT_EQ(a / b, certa::vec(3, 1/6.0, 3/4.0, 5/2.0));
}
