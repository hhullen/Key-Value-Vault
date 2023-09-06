#ifndef SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
#define SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_

#include <lib/vault_interface/vault_interface.h>

#include <memory>

namespace hhullen {
using VaultPtr = std::unique_ptr<IVault>;

class VaultEngine {
 public:
  VaultEngine();

 private:
  VaultPtr vault;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
