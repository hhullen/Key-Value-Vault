#ifndef SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
#define SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_

#include <lib/vault/data_structure/vault_data.h>

#include <iostream>

namespace hhullen {

using Str = std::string;
using Err = std::exception;
using std::pair;
using std::vector;

class IVault {
 public:
  virtual ~IVault() {}
  virtual pair<bool, Err> Set(Str key, VaultData value, size_t ex) = 0;
  virtual pair<VaultData, Err> Get(Str key) const = 0;
  virtual bool IsExists(Str key) const = 0;
  virtual bool Delete(Str key) = 0;
  virtual pair<bool, Err> Update(Str key, VaultData value, size_t ex) = 0;
  virtual vector<Str> GetKeys() const = 0;
  virtual pair<bool, Err> Rename(Str key_old, Str key_new) = 0;
  virtual pair<size_t, Err> GetTTL(Str key) const = 0;
  virtual vector<Str> Find(VaultData value) const = 0;
  virtual vector<VaultData> ShowAll() const = 0;
  virtual pair<size_t, Err> Upload(Str file_path) = 0;
  virtual pair<size_t, Err> Export(Str file_path) const = 0;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_INTERFACE_VAULT_INTERFACE_H_
