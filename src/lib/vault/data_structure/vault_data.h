#ifndef SRC_LIB_VAULT_DATA_VAULT_DATA_H_
#define SRC_LIB_VAULT_DATA_VAULT_DATA_H_

#include <vector>

namespace hhullen {

using Str = std::string;
using Data = std::vector<Str>;

class VaultData {
 public:
  VaultData();

 private:
  Data data;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_DATA_VAULT_DATA_H_
