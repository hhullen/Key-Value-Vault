#include "bin_tree.h"

namespace hhullen {

Err SelfBalancingBinarySearchTree::Set(const Str& key, const VaultData& value) {
  if (GetIfExistsOrAlive(key) != container_.End()) {
    return Err("value with the key \"" + key + "\" already exists.");
  }
  container_.Emplace({key, value});
  return Err();
}

pair<VaultData, Err> SelfBalancingBinarySearchTree::Get(const Str& key) {
  Container::Iterator iter = GetIfExistsOrAlive(key);
  if (iter == container_.End()) {
    return {VaultData(), Err("(null)")};
  }
  return {(*iter).second, Err()};
}

bool SelfBalancingBinarySearchTree::IsExists(const Str& key) {
  return GetIfExistsOrAlive(key) != container_.End();
}

bool SelfBalancingBinarySearchTree::Delete(const Str& key) {
  if (GetIfExistsOrAlive(key) == container_.End()) {
    return false;
  }
  container_.Delete(key);
  return true;
}

Err SelfBalancingBinarySearchTree::Update(const Str& key,
                                          const VaultData& value) {
  if (GetIfExistsOrAlive(key) == container_.End()) {
    return Err("value with the key \"" + key + "\" does not exists.");
  }
  container_.Emplace({key, value});
  return Err();
}

void SelfBalancingBinarySearchTree::GetKeys(Channel<Str>& out) {
  Container::Iterator iter = container_.Begin();
  for (size_t i = 1; iter != container_.End();) {
    size_t timemark = static_cast<size_t>((*iter).second.GetDeathTimeMark());
    if (IsExpired(timemark)) {
      iter = container_.Delete((*iter).first);
      continue;
    }
    out.Send(Str(to_string(i) + ") ") + (*iter).first);
    ++iter;
    ++i;
  }
}

Err SelfBalancingBinarySearchTree::Rename(const Str& key_old,
                                          const Str& key_new) {
  Container::Iterator iter = GetIfExistsOrAlive(key_old);
  if (iter == container_.End()) {
    return Err("value with the key \"" + key_old + "\" does not exists.");
  }
  if (GetIfExistsOrAlive(key_new) != container_.End()) {
    return Err("value with the key \"" + key_new + "\" already exists.");
  }
  container_.Emplace({key_new, (*iter).second});
  container_.Delete(key_old);
  return Err();
}

pair<size_t, Err> SelfBalancingBinarySearchTree::GetTTL(const Str& key) {
  Container::Iterator iter = GetIfExistsOrAlive(key);
  if (iter == container_.End()) {
    return {0, Err("(null)")};
  }
  size_t timemark_ttl = (*iter).second.GetDeathTimeMark();
  if (timemark_ttl == 0) {
    return {0, Err("(infinite)")};
  }
  size_t timemark_now = static_cast<size_t>(Timer().TimepointSec());
  return {timemark_ttl - timemark_now, Err()};
}

void SelfBalancingBinarySearchTree::Find(Channel<Str>& out,
                                         const VaultData& value) {
  Container::Iterator iter = container_.Begin();
  for (size_t i = 1; iter != container_.End();) {
    size_t timemark = (*iter).second.GetDeathTimeMark();
    if (IsExpired(timemark)) {
      iter = container_.Delete((*iter).first);
      continue;
    }
    if ((*iter).second == value) {
      out.Send(Str(to_string(i) + ") ") + (*iter).first);
      ++i;
    }
    ++iter;
  }
}

void SelfBalancingBinarySearchTree::ShowAll(Channel<Str>& out) {
  Container::Iterator iter = container_.Begin();
  if (iter != container_.End()) {
    size_t timemark = (*iter).second.GetDeathTimeMark();
    if (!IsExpired(timemark)) {
      out.Send(VaultData::kHeader);
    }
  }
  for (size_t i = 1; iter != container_.End();) {
    size_t timemark = (*iter).second.GetDeathTimeMark();
    if (IsExpired(timemark)) {
      iter = container_.Delete((*iter).first);
      continue;
    }
    out.Send(Str("> " + to_string(i) + "\t") + (*iter).second.GetRowAsString());
    ++i;
    ++iter;
  }
}

pair<size_t, Err> SelfBalancingBinarySearchTree::Upload(const Str& file_path) {
  ifstream file(file_path);
  if (!file.is_open()) {
    return {0, "can not open file \"" + file_path + "\"."};
  }
  Str line;
  size_t counter = 0, order = container_.Size();
  for (; getline(file, line, '\n'); ++counter, ++order) {
    StrList splitted = StrPlus::Split(line);
    VaultData payload;
    Err err = payload.ReadPayload(splitted, 0);
    if (err.HasError()) {
      return {counter, Err("uploading have been interrupted: " + err.What())};
    }
    container_.Emplace({Str("key" + to_string(order)), payload});
  }
  if (counter == 0) {
    return {0, "can not upload from file \"" + file_path + "\"."};
  }
  return {counter, Err()};
}

pair<size_t, Err> SelfBalancingBinarySearchTree::Export(const Str& file_path) {
  ofstream file(file_path);
  if (!file.is_open()) {
    return {0, "can not open file \"" + file_path + "\"."};
  }
  Container::Iterator iter = container_.Begin();
  size_t counter = 0;
  for (; iter != container_.End(); ++iter, ++counter) {
    file << (*iter).second.GetRowAsString(' ') << "\n";
  }
  return {counter, Err()};
}

// /*
//     private methods
// */

SelfBalancingBinarySearchTree::Container::Iterator
SelfBalancingBinarySearchTree::GetIfExistsOrAlive(const Str& key) {
  Container::Iterator iter = container_.Find(key);
  if (iter == container_.End()) {
    return iter;
  }
  size_t timemark = (*iter).second.GetDeathTimeMark();
  if (IsExpired(timemark)) {
    container_.Delete(key);
    return container_.End();
  }
  return iter;
}

bool SelfBalancingBinarySearchTree::IsExpired(size_t timemark) {
  return timemark != 0 &&
         timemark < static_cast<size_t>(Timer().TimepointSec());
}

}  // namespace hhullen
