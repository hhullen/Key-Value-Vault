#ifndef SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
#define SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_

#include <lib/vault/data/vault_data.h>

#include <iostream>

namespace hhullen {

using Str = std::string;
using Err = std::exception;
using Pair = std::pair;
using Vector = std::vector;

class IVault {
 public:
  ~IVault() = delete;
  virtual Pair<bool, Err> Set(Str key, VaultData value, size_t ex) = 0;
  virtual Pair<VaultData, Err> Get(Str key) const = 0;
  virtual bool IsExists(Str key) const = 0;
  virtual bool Delete(Str key) = 0;
  virtual Pair<bool, Err> Update(Str key, VaultData value, size_t ex) = 0;
  virtual Vector<Str> GetStrs() const = 0;
  virtual Pair<bool, Err> Rename(Str key_old, Str key_new) = 0;
  virtual Pair<size_t, Err> GetTTL(Str key) const = 0;
  virtual Vector<Str> Find(VaultData value) const = 0;
  virtual Vector<VaultData> ShowAll() const = 0;
  virtual Pair<size_t, Err> Upload(string file_path) = 0;
  virtual Pair<size_t, Err> Export(string file_path) const = 0;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
