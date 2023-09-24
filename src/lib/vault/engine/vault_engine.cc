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

  pair<VaultData, Str> vault_data = AssembleVaultData(arguments);
  if (vault_data.second != "") {
    SendError(vault_data.second);
    return;
  }

  const Str& key = arguments[0];
  Str err = vault_->Set(key, vault_data.first);
  if (err != "") {
    SendError(err);
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
    pair<VaultData, Str> found = vault_->Get(key);
    if (found.second != "") {
      SendExecutionResult(found.second);
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

  pair<VaultData, Str> vault_data = AssembleVaultData(arguments);
  if (vault_data.second != "") {
    SendError(vault_data.second);
    return;
  }

  const Str& key = arguments[0];
  Str err = vault_->Update(key, vault_data.first);
  if (err != "") {
    SendError(err);
  } else {
    SendExecutionResult("OK");
  }
}

void VaultEngine::ExecuteKeys(vector<Str>& arguments) {
  vault_->GetKeys(output_stream_);
}

void VaultEngine::ExecuteRename(vector<Str>& arguments) {
  if (arguments.size() < 2) {
    SendError("keys to be renamed and to rename have not been specified.");
    return;
  }
  const Str& key_to_be_renamed = arguments[0];
  const Str& key_renamer = arguments[1];
  Str err = vault_->Rename(key_to_be_renamed, key_renamer);
  if (err != "") {
    SendError(err);
  } else {
    SendExecutionResult("OK");
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

pair<VaultData, Str> VaultEngine::AssembleVaultData(vector<Str>& arguments) {
  pair<VaultData, Str> payload = ReadPayload(arguments);
  if (payload.second != "") {
    return payload;
  }

  pair<size_t, Str> life_time = ReadLifeTime(arguments);
  if (life_time.second != "") {
    return pair<VaultData, Str>(VaultData(), life_time.second);
  }
  payload.first.SetDeathTimeMark(life_time.first);

  return payload;
}

pair<VaultData, Str> VaultEngine::ReadPayload(vector<Str>& arguments) {
  VaultData payload;
  for (size_t i = 1; i < arguments.size() && i - 1 < VaultData::kMaxFields;
       ++i) {
    Str err = payload.SetField(i - 1, arguments[i]);
    if (err != "") {
      return pair<VaultData, Str>(payload, err);
    }
  }
  return pair<VaultData, Str>(payload, Str());
}

pair<size_t, Str> VaultEngine::ReadLifeTime(vector<Str>& arguments) {
  if (arguments.size() < 7) {
    return pair<size_t, Str>({0, ""});
  }
  if (arguments[6] != "EX") {
    return pair<size_t, Str>(
        {0, "\"EX\" as lifetime specifier expected, but got \"" + arguments[6] +
                "\"."});
  }
  if (arguments.size() < 8) {
    return pair<size_t, Str>({0, "\"EX\" with no value have been specified."});
  }
  if (!StrPlus::IsValid(arguments[7], StrPlus::Type::UInt)) {
    return pair<size_t, Str>(
        {0, "unable to cast value \"" + arguments[7] +
                "\" specified as \"EX\" value to type uint."});
  }
  return pair<size_t, Str>(std::atoi(arguments[7].data()), "");
}

void VaultEngine::SendError(const Str& message) {
  output_stream_.Send("> ERROR: " + message);
}

void VaultEngine::SendExecutionResult(const Str& message) {
  output_stream_.Send("> " + message);
}

}  // namespace hhullen
