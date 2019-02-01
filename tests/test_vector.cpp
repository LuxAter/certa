#include "gtest/gtest.h"
#include "vector.h"

class VectorTest : public testing::Test {
 protected:
   virtual void SetUp() {
     a.resize(4);
   }
  virtual void TearDown() {}
  certa::vec<double> a;
};

TEST_F(VectorTest, Constructor){
  EXPECT_EQ(a.str(), "<0,0,0,0>");
}
