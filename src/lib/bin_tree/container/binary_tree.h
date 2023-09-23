#ifndef SRC_LIB_BINARY_TREE_DEFINITION_H_
#define SRC_LIB_BINARY_TREE_DEFINITION_H_

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "i_container_iterator.h"

namespace hhullen {

#define BIN_TREE_DEF BinTree<Value, Key, KeyRetractor, Comparator>
#define TEMPLATE_DEF \
  template <class Value, class Key, class KeyRetractor, class Comparator>

template <class T1>
class BinTreeKeyRetractor {
 public:
  const T1& operator()(const T1& value) const { return value; }
};

template <class Value, class Key = Value,
          class KeyRetractor = BinTreeKeyRetractor<Value>,
          class Comparator = std::less<Key>>
class BinTree {
 public:
  struct Node;
  using NodePtr = Node*;
  using NodePtrPair = std::pair<NodePtr, NodePtr>;
  struct Node {
    ~Node() {
      for (int i = 0; i < 2; ++i) {
        if (relatives[i]) {
          delete relatives[i];
        }
        relatives[i] = nullptr;
      }
      relatives[2] = nullptr;
    }
    Value value;
    enum Relatives : size_t { Left = 0, Right, Parent };
    NodePtr relatives[3];
  };
  class Iterator : public IContainerIterator<Iterator, Value> {
   public:
    Iterator();
    Iterator(NodePtr node, NodePtr end);
    Value operator*() const override;
    Iterator& operator++() override;
    Iterator operator++(int) override;
    Iterator& operator--() override;
    Iterator operator--(int) override;
    bool operator==(const Iterator& other) const override;
    bool operator!=(const Iterator& other) const override;

   private:
    NodePtr node_ptr_, end_;
  };
  BinTree();
  ~BinTree();

  Iterator Begin();
  Iterator End();
  void Clear();
  bool Contains(const Key& key);
  bool Empty();
  Iterator Find(const Key& key);
  Iterator Emplace(const Value& value);
  Iterator Delete(const Key& key);
  size_t Size();
  size_t Height();

 private:
  NodePtr root_, end_;
  size_t size_;

  NodePtr& SetNewNodeOnNull(NodePtr& prev, const Value& value);
  void SetNewNodeOnEnd(NodePtr& selector, const Value& value);
  NodePtrPair Seek(const Key& key);
  void ReplaceNode(NodePtr& to_replace, NodePtr& src);
  void ReplaceNodeRelative(NodePtr& to_replace, NodePtr& replacer,
                           size_t relative);
  void DeleteWithNoChilds(NodePtr& node);
  void DeleteWithBothChilds(NodePtr& node);
  size_t CalculateHeight(const NodePtr&);
  NodePtr& GetSelfFromNodeParent(NodePtr& node);
  void RunBalancingFromNode(NodePtr node);
  int GetChilrenHeightDifference(const NodePtr& node);
  void RotateLeft(NodePtr& node);
  void RotateRight(NodePtr node);

  static bool IsEQ(const Key& key1, const Key& key2);
  static NodePtr& GetRelative(const NodePtr& node, size_t direction);
  static void GoToEnd(NodePtr& selector, size_t direction);
};

}  // namespace hhullen

#include "binary_tree.cc"
#include "iterator.cc"

#endif  // SRC_LIB_BINARY_TREE_DEFINITION_H_
