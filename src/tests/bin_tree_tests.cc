#include <gtest/gtest.h>
#include <lib/bin_tree/bin_tree.h>

TEST(BinTtree, Constructor) {
  EXPECT_NO_THROW(hhullen::SelfBalancingBinarySearchTree tree);
}

TEST(BinTtree, Set_method_correct_payload) {
  hhullen::VaultData payload;
  payload.SetField(0, "Vasilisa");
  payload.SetField(1, "Kadyk");
  payload.SetField(2, "1998");
  payload.SetField(3, "Uganda");
  payload.SetField(4, "451");
  hhullen::SelfBalancingBinarySearchTree tree;
  hhullen::Str err = tree.Set("key1", payload);
  //   EXPECT_TRUE(err == "");
  //   err = tree.Set("key2", payload);
  //   EXPECT_TRUE(err == "");
  //   err = tree.Set("key3", payload);
  //   EXPECT_TRUE(err == "");
}