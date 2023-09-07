#include "vault_engine.h"

namespace hhullen {

void VaultEngine::Init(VaultType vault_type) {
  if (vault_type == VaultType::BIN_TREE) {
    // vault = VaultPtr(new SelfBalancingBinarySearchTree());
  } else if (vault_type == VaultType::HASH_TABLE) {
    // vault = VaultPtr(new HashTable());
  }
}

}  // namespace hhullen
