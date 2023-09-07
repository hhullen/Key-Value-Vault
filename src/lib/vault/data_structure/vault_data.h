#ifndef SRC_LIB_VAULT_DATA_VAULT_DATA_H_
#define SRC_LIB_VAULT_DATA_VAULT_DATA_H_

#include <string>
#include <vector>

namespace hhullen {

using Str = std::string;

class VaultData {
 public:
  using Data = std::vector<Str>;
  VaultData() {
    data.resize(5);
    data.shrink_to_fit();
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] = '-';
    }
  }

 private:
  Data data;
};

}  // namespace hhullen

#endif  // SRC_LIB_VAULT_DATA_VAULT_DATA_H_
