#ifndef SRC_LIB_I_CONTAINER_ITERATOR_H_
#define SRC_LIB_I_CONTAINER_ITERATOR_H_

namespace hhullen {

template <class Iterator, class Value>
class IContainerIterator {
 public:
  virtual Value operator*() const = 0;
  virtual Iterator& operator++() = 0;
  virtual Iterator operator++(int) = 0;
  virtual Iterator& operator--() = 0;
  virtual Iterator operator--(int) = 0;
  virtual bool operator==(const Iterator& other) const = 0;
  virtual bool operator!=(const Iterator& other) const = 0;
};

}  // namespace hhullen

#endif  // SRC_LIB_I_CONTAINER_ITERATOR_H_
