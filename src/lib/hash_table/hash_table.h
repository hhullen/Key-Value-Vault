#ifndef SRC_LIB_HASH_TABLE_HASH_TABLE_H_
#define SRC_LIB_HASH_TABLE_HASH_TABLE_H_

#include <lib/vault_interface/vault_interface.h>

namespace hhullen {

class HashTable : public IVault {
 public:
  Pair<bool, Err> Set(Key key, Value value, size_t ex) override;
  Pair<Value, Err> Get(Key key) const override;
  bool IsExists(Key key) const override;
  bool Delete(Key key) override;
  Pair<bool, Err> Update(Key key, Value value, size_t ex) override;
  Vector<Key> GetKeys() const override;
  Pair<bool, Err> Rename(Key key_old, Key key_new) override;
  Pair<size_t, Err> GetTTL(Key key) const override;
  Vector<Key> Find(Value value) const override;
  Vector<Value> ShowAll() const override;
  Pair<size_t, Err> Upload(string file_path) override;
  Pair<size_t, Err> Export(string file_path) const override;

 private:
};

}  // namespace hhullen

#endif  // SRC_LIB_HASH_TABLE_HASH_TABLE_H_