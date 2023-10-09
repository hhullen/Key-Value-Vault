#ifndef SRC_LIB_HASH_TABLE_HASH_TABLE_H_
#define SRC_LIB_HASH_TABLE_HASH_TABLE_H_

#include <lib/vault/vault_interface.h>

namespace hhullen {

class HashTable : public IVault {
  // using Container = ...

 public:
  Err Set(const Str& key, const VaultData& value) {}
  pair<VaultData, Err> Get(const Str& key) {}
  bool IsExists(const Str& key) {}
  bool Delete(const Str& key) {}
  Err Update(const Str& key, const VaultData& value) {}
  void GetKeys(Channel<Str>& out) {}
  Err Rename(const Str& key_old, const Str& key_new) {}
  pair<size_t, Err> GetTTL(const Str& key) {}
  void Find(Channel<Str>& out, const VaultData& value) {}
  void ShowAll(Channel<Str>& out) {}
  pair<size_t, Err> Upload(const Str& file_path) {}
  pair<size_t, Err> Export(const Str& file_path) {}

 private:
  //   Container container_;

  //   Container::Iterator GetIfExistsOrAlive(const Str& key);
  //   bool IsExpired(size_t timemark);
};

}  // namespace hhullen

#endif  // SRC_LIB_HASH_TABLE_HASH_TABLE_H_
