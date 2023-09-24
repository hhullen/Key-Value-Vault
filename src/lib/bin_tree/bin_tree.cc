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

bool SelfBalancingBinarySearchTree::IsExists(Str key) {
  return container_.Contains(key);
}

bool SelfBalancingBinarySearchTree::Delete(Str key) {
  if (!container_.Contains(key)) {
    return false;
  }
  container_.Delete(key);
  return true;
}

hhullen::Str SelfBalancingBinarySearchTree::Update(Str key, VaultData value) {
  if (!container_.Contains(key)) {
    return Str("value with the key \"" + key + "\" does not exists.");
  }
  container_.Emplace({key, value});
  return Str();
}

void SelfBalancingBinarySearchTree::GetKeys(Channel<Str>& out) {
  Container::Iterator iter = container_.Begin();
  for (; iter != container_.End(); ++iter) {
    out.Send((*iter).first);
  }
}

hhullen::Str SelfBalancingBinarySearchTree::Rename(Str key_old, Str key_new) {
  Container::Iterator iter = container_.Find(key_old);
  if (iter == container_.End()) {
    return Str("value with the key \"" + key_old + "\" does not exists.");
  }
  if (container_.Contains(key_new)) {
    return Str("value with the key \"" + key_new + "\" already exists.");
  }
  container_.Emplace({key_new, (*iter).second});
  container_.Delete(key_old);
  return Str();
}

pair<size_t, Str> SelfBalancingBinarySearchTree::GetTTL(Str key) {
  return pair<size_t, Str>();
}

void SelfBalancingBinarySearchTree::Find(Channel<Str>& out, VaultData value) {}

void SelfBalancingBinarySearchTree::ShowAll(Channel<Str>& out) {}

pair<size_t, Str> SelfBalancingBinarySearchTree::Upload(Str file_path) {
  return pair<size_t, Str>();
}

pair<size_t, Str> SelfBalancingBinarySearchTree::Export(Str file_path) {
  return pair<size_t, Str>();
}

}  // namespace hhullen
