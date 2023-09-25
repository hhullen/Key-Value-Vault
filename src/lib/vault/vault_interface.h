#ifndef SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
#define SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_

#include <lib/vault/data_structure/vault_data.h>
#include <lib/vault/engine/channel/channel.h>

#include <iostream>

namespace hhullen {

using Str = std::string;
using std::pair;

class IVault {
 public:
  virtual ~IVault() {}
  virtual Str Set(const Str& key, const VaultData& value) = 0;
  virtual pair<VaultData, Str> Get(const Str& key) = 0;
  virtual bool IsExists(const Str& key) = 0;
  virtual bool Delete(const Str& key) = 0;
  virtual Str Update(const Str& key, const VaultData& value) = 0;
  virtual void GetKeys(Channel<Str>& out) = 0;
  virtual Str Rename(const Str& key_old, const Str& key_new) = 0;
  virtual pair<size_t, Str> GetTTL(const Str& key) = 0;
  virtual void Find(Channel<Str>& out, const VaultData& value) = 0;
  virtual void ShowAll(Channel<Str>& out) = 0;
  virtual pair<size_t, Str> Upload(const Str& file_path) = 0;
  virtual pair<size_t, Str> Export(const Str& file_path) = 0;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
