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
  virtual Str Set(Str key, VaultData value) = 0;
  virtual pair<VaultData, Str> Get(Str key) = 0;
  virtual bool IsExists(Str key) const = 0;
  virtual bool Delete(Str key) = 0;
  virtual Str Update(Str key, VaultData value) = 0;
  virtual void GetKeys(Channel<Str>& out) const = 0;
  virtual Str Rename(Str key_old, Str key_new) = 0;
  virtual pair<size_t, Str> GetTTL(Str key) const = 0;
  virtual void Find(Channel<Str>& out, VaultData value) const = 0;
  virtual void ShowAll(Channel<Str>& out) const = 0;
  virtual pair<size_t, Str> Upload(Str file_path) = 0;
  virtual pair<size_t, Str> Export(Str file_path) const = 0;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
