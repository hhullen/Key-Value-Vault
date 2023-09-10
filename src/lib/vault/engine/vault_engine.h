#ifndef SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
#define SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_

#include <lib/bin_tree/bin_tree.h>
#include <lib/hash_table/hash_table.h>
#include <lib/vault/engine/channel/channel.h>
#include <lib/vault/vault_interface.h>

#include <memory>
#include <optional>

namespace hhullen {
using std::optional;

class VaultEngine {
  using VaultPtr = std::unique_ptr<IVault>;

 public:
  VaultEngine() {}
  ~VaultEngine() {}

  enum VaultType { BIN_TREE, HASH_TABLE };
  void Init(VaultType vault_type);

  void ExecuteSet(VaultData::Row &arguments) {
    output_stream_.Send("SEND METHOD WORKS");
  }
  void ExecuteGet(VaultData::Row &arguments) {}
  void ExecuteExists(VaultData::Row &arguments) {}
  void ExecuteDelete(VaultData::Row &arguments) {}
  void ExecuteUpdate(VaultData::Row &arguments) {}
  void ExecuteKeys(VaultData::Row &arguments) {}
  void ExecuteRename(VaultData::Row &arguments) {}
  void ExecuteTTL(VaultData::Row &arguments) {}
  void ExecuteFind(VaultData::Row &arguments) {}
  void ExecuteShowAll(VaultData::Row &arguments) {}
  void ExecuteUpload(VaultData::Row &arguments) {}
  void ExecuteExport(VaultData::Row &arguments) {}

  optional<Str> Yield() {
    if (output_stream_) {
      return output_stream_.Get();
    }
    return {};
  }

  void StopStreaming() { output_stream_.Close(); }

 private:
  VaultPtr vault;
  Channel<Str> output_stream_;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_ENGINE_VAULT_ENGINE_H_
