#ifndef SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
#define SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_

#include <vault/vault_interface.h>

#include <memory>

namespace hhullen {

class VaultEngine {
  using VaultPtr = std::unique_ptr<IVault>;

 public:
  VaultEngine() {}

  enum VaultType { BIN_TREE, HASH_TABLE };
  void Init(VaultType vault_type) {
    if (vault_type == VaultType::BIN_TREE) {
      // vault = VaultPtr(new...)
    } else if (vault_type == VaultType::HASH_TABLE) {
      // vault = VaultPtr(new...)
    }
  }

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

  pair<Str, bool> Yield() { return {Str("Die! Mutherfucker Die!"), true}; }

 private:
  VaultPtr vault;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
