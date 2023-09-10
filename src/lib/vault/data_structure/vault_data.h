#ifndef SRC_LIB_VAULT_DATA_VAULT_DATA_H_
#define SRC_LIB_VAULT_DATA_VAULT_DATA_H_

#include <string>
#include <vector>

namespace hhullen {

using Str = std::string;

class VaultData {
 public:
  using Row = std::vector<Str>;
  VaultData() {
    row.resize(5, "-");
    row.shrink_to_fit();
  }

 private:
  Row row;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_DATA_VAULT_DATA_H_
