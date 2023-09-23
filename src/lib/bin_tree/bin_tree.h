#ifndef SRC_LIB_BIN_TREE_BIN_TREE_H_
#define SRC_LIB_BIN_TREE_BIN_TREE_H_

#include <lib/vault/vault_interface.h>

#include "container/binary_tree.h"

namespace hhullen {

template <class Key, class Value>
class PairKeyRetractor {
 public:
  const Key& operator()(const pair<Key, Value>& value) { return value.first; }
};

class SelfBalancingBinarySearchTree : public IVault {
  using Container =
      BinTree<pair<Str, VaultData>, Str, PairKeyRetractor<Str, VaultData>>;

 public:
  Str Set(Str key, VaultData value);
  pair<VaultData, Str> Get(Str key);
  bool IsExists(Str key) const;
  bool Delete(Str key);
  Str Update(Str key, VaultData value);
  void GetKeys(Channel<Str>& out) const;
  Str Rename(Str key_old, Str key_new);
  pair<size_t, Str> GetTTL(Str key) const;
  void Find(Channel<Str>& out, VaultData value) const;
  void ShowAll(Channel<Str>& out) const;
  pair<size_t, Str> Upload(Str file_path);
  pair<size_t, Str> Export(Str file_path) const;

 private:
  Container container_;
};

}  // namespace hhullen

#endif  // SRC_LIB_BIN_TREE_BIN_TREE_H_
