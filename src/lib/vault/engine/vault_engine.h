#ifndef SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
#define SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_

#include <lib/bin_tree/bin_tree.h>
#include <lib/hash_table/hash_table.h>
#include <lib/vault/vault_interface.h>

#include <memory>
#include <optional>

namespace hhullen {
using std::optional;

class VaultEngine {
  using VaultPtr = std::unique_ptr<IVault>;

 public:
  VaultEngine() {}

  enum VaultType { BIN_TREE, HASH_TABLE };
  void Init(VaultType vault_type);

  void ExecuteSet(vector<Str> arguments) { std::cout << "SET execution\n"; }
  void ExecuteGet(vector<Str> arguments) {}
  void ExecuteExists(vector<Str> arguments) {}
  void ExecuteDelete(vector<Str> arguments) {}
  void ExecuteUpdate(vector<Str> arguments) {}
  void ExecuteKeys(vector<Str> arguments) {}
  void ExecuteRename(vector<Str> arguments) {}
  void ExecuteTTL(vector<Str> arguments) {}
  void ExecuteFind(vector<Str> arguments) {}
  void ExecuteShowAll(vector<Str> arguments) {}
  void ExecuteUpload(vector<Str> arguments) {}
  void ExecuteExport(vector<Str> arguments) {}

  optional<Str> Yield() {
    // return Str("Die! Mutherfucker Die!");
    return {};
  }

 private:
  VaultPtr vault;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
