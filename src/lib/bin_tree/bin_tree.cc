#include "bin_tree.h"

namespace hhullen {

Str SelfBalancingBinarySearchTree::Set(Str key, VaultData value) {
  if (container_.Contains(key)) {
    return Str("value with the key \"" + key + "\" already exists.");
  }
  container_.Emplace({key, value});
  return Str();
}

pair<VaultData, Str> SelfBalancingBinarySearchTree::Get(Str key) {
  Container::Iterator iter = container_.Find(key);
  if (iter == container_.End()) {
    return pair<VaultData, Str>(VaultData(), Str("(null)"));
  }
  return pair<VaultData, Str>((*iter).second, Str());
}

bool SelfBalancingBinarySearchTree::IsExists(Str key) const { return true; }

bool SelfBalancingBinarySearchTree::Delete(Str key) { return true; }

hhullen::Str SelfBalancingBinarySearchTree::Update(Str key, VaultData value) {
  return Str();
}

void SelfBalancingBinarySearchTree::GetKeys(Channel<Str>& out) const {}

hhullen::Str SelfBalancingBinarySearchTree::Rename(Str key_old, Str key_new) {
  return Str();
}

pair<size_t, Str> SelfBalancingBinarySearchTree::GetTTL(Str key) const {
  return pair<size_t, Str>();
}

void SelfBalancingBinarySearchTree::Find(Channel<Str>& out,
                                         VaultData value) const {}

void SelfBalancingBinarySearchTree::ShowAll(Channel<Str>& out) const {}

pair<size_t, Str> SelfBalancingBinarySearchTree::Upload(Str file_path) {
  return pair<size_t, Str>();
}

pair<size_t, Str> SelfBalancingBinarySearchTree::Export(Str file_path) const {
  return pair<size_t, Str>();
}

}  // namespace hhullen
