#include "vault_engine.h"

namespace hhullen {

void VaultEngine::Init(VaultType vault_type) {
  if (vault_type == VaultType::BIN_TREE) {
    vault_.reset(new SelfBalancingBinarySearchTree());
  } else if (vault_type == VaultType::HASH_TABLE) {
    // vault = VaultPtr(new HashTable());
  }
}

void VaultEngine::ExecuteSet(vector<Str>& arguments) {
  if (arguments.size() < 2) {
    SendError("No key or at least one value field filled.");
    return;
  }
  pair<VaultData, Err> vault_data = AssembleVaultData(arguments);
  if (vault_data.second.HasError()) {
    SendError(vault_data.second.What());
    return;
  }
  const Str& key = arguments[0];
  Err err = vault_->Set(key, vault_data.first);
  if (err.HasError()) {
    SendError(err.What());
  } else {
    SendExecutionResult("OK");
  }
}

void VaultEngine::ExecuteGet(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("no keys have been specified");
    return;
  }
  for (size_t i = 0; i < arguments.size(); ++i) {
    const Str& key = arguments[i];
    pair<VaultData, Err> found = vault_->Get(key);
    if (found.second.HasError()) {
      SendExecutionResult(found.second.What());
    } else {
      SendExecutionResult(found.first.GetRowAsString());
    }
  }
}

void VaultEngine::ExecuteExists(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("no keys have been specified");
    return;
  }
  for (size_t i = 0; i < arguments.size(); ++i) {
    const Str& key = arguments[i];
    if (vault_->IsExists(key)) {
      SendExecutionResult("true");
    } else {
      SendExecutionResult("false");
    }
  }
}

void VaultEngine::ExecuteDelete(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("no keys have been specified");
    return;
  }
  for (size_t i = 0; i < arguments.size(); ++i) {
    const Str& key = arguments[i];
    if (vault_->Delete(key)) {
      SendExecutionResult("true");
    } else {
      SendExecutionResult("false");
    }
  }
}

void VaultEngine::ExecuteUpdate(vector<Str>& arguments) {
  if (arguments.size() < 2) {
    SendError("No key or at least one value field filled.");
    return;
  }
  pair<VaultData, Err> vault_data = AssembleVaultData(arguments);
  if (vault_data.second.HasError()) {
    SendError(vault_data.second.What());
    return;
  }
  const Str& key = arguments[0];
  Err err = vault_->Update(key, vault_data.first);
  if (err.HasError()) {
    SendError(err.What());
  } else {
    SendExecutionResult("OK");
  }
}

void VaultEngine::ExecuteKeys(vector<Str>& arguments) {
  arguments.clear();
  vault_->GetKeys(output_stream_);
}

void VaultEngine::ExecuteRename(vector<Str>& arguments) {
  if (arguments.size() < 2) {
    SendError("keys to be renamed and to rename have not been specified.");
    return;
  }
  const Str& key_to_be_renamed = arguments[0];
  const Str& key_renamer = arguments[1];
  Err err = vault_->Rename(key_to_be_renamed, key_renamer);
  if (err.HasError()) {
    SendError(err.What());
  } else {
    SendExecutionResult("OK");
  }
}

void VaultEngine::ExecuteTTL(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("key have not been specified.");
    return;
  }
  const Str& key = arguments[0];
  pair<size_t, Err> out = vault_->GetTTL(key);
  if (out.second.HasError()) {
    SendExecutionResult(out.second.What());
  } else {
    SendExecutionResult(std::to_string(out.first));
  }
}

void VaultEngine::ExecuteFind(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("No value field filled.");
    return;
  }
  VaultData to_find;
  Err err = to_find.ReadPayload(arguments, 0);
  if (err.HasError()) {
    SendError(err.What());
    return;
  }
  vault_->Find(output_stream_, to_find);
}

void VaultEngine::ExecuteShowAll(vector<Str>& arguments) {
  arguments.clear();
  vault_->ShowAll(output_stream_);
}

void VaultEngine::ExecuteUpload(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("No file path have been specified.");
    return;
  }
  pair<size_t, Err> out = vault_->Upload(arguments[0]);
  if (out.second.HasError()) {
    SendError(out.second.What() + Str(" Uploaded: " + to_string(out.first)));
  } else {
    SendExecutionResult("OK " + to_string(out.first));
  }
}

void VaultEngine::ExecuteExport(vector<Str>& arguments) {
  if (arguments.size() < 1) {
    SendError("No file path have been specified.");
    return;
  }
  pair<size_t, Err> out = vault_->Export(arguments[0]);
  if (out.second.HasError()) {
    SendError(out.second.What());
  } else {
    SendExecutionResult("OK " + to_string(out.first));
  }
}

optional<Str> VaultEngine::Yield() {
  if (output_stream_) {
    return output_stream_.Get();
  }
  return {};
}

void VaultEngine::StopStreaming() { output_stream_.Close(); }

// /*
//     private methods
// */

pair<VaultData, Err> VaultEngine::AssembleVaultData(vector<Str>& arguments) {
  pair<VaultData, Err> returnable;
  returnable.second = returnable.first.ReadPayload(arguments, 1);
  if (returnable.second.HasError()) {
    return returnable;
  }
  pair<size_t, Err> life_time = ReadLifeTime(arguments);
  if (life_time.second.HasError()) {
    return {returnable.first, life_time.second};
  }
  returnable.first.SetDeathTimeMark(life_time.first);

  return returnable;
}

pair<size_t, Err> VaultEngine::ReadLifeTime(vector<Str>& arguments) {
  if (arguments.size() < 7) {
    return {0, Err()};
  }
  if (arguments[6] != "EX") {
    return {0, Err("\"EX\" as lifetime specifier expected, but got \"" +
                   arguments[6] + "\".")};
  }
  if (arguments.size() < 8) {
    return {0, Err("\"EX\" with no value have been specified.")};
  }
  if (!StrPlus::IsValid(arguments[7], StrPlus::Type::UInt)) {
    return {0, Err("unable to cast value \"" + arguments[7] +
                   "\" specified as \"EX\" value to type uint.")};
  }
  size_t lifetime = static_cast<size_t>(Timer().TimepointSec()) +
                    static_cast<size_t>(std::atoi(arguments[7].data()));
  return {lifetime, Err()};
}

void VaultEngine::SendError(const Str& message) {
  output_stream_.Send("> ERROR: " + message);
}

void VaultEngine::SendExecutionResult(const Str& message) {
  output_stream_.Send("> " + message);
}

}  // namespace hhullen
