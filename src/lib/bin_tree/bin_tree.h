#ifndef SRC_LIB_BIN_TREE_BIN_TREE_H_
#define SRC_LIB_BIN_TREE_BIN_TREE_H_

#include <include/timer/timer.h>
#include <lib/vault/vault_interface.h>

#include "container/binary_tree.h"

namespace hhullen {

template <class Key, class Value>
class PairKeyRetractor {
 public:
  const Key& operator()(const pair<Key, Value>& value) { return value.first; }
};

class SelfBalancingBinarySearchTree : public IVault {
 public:
  using Container =
      BinTree<pair<Str, VaultData>, Str, PairKeyRetractor<Str, VaultData>>;

  Str Set(const Str& key, const VaultData& value);
  pair<VaultData, Str> Get(const Str& key);
  bool IsExists(const Str& key);
  bool Delete(const Str& key);
  Str Update(const Str& key, const VaultData& value);
  void GetKeys(Channel<Str>& out);
  Str Rename(const Str& key_old, const Str& key_new);
  pair<size_t, Str> GetTTL(const Str& key);
  void Find(Channel<Str>& out, const VaultData& value);
  void ShowAll(Channel<Str>& out);
  pair<size_t, Str> Upload(const Str& file_path);
  pair<size_t, Str> Export(const Str& file_path);

 private:
  Container container_;

  Container::Iterator GetIfExistsOrAlive(const Str& key);
  bool IsExpired(size_t timemark);
};

}  // namespace hhullen

#endif  // SRC_LIB_BIN_TREE_BIN_TREE_H_
