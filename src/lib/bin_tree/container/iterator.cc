#ifndef SRC_LIB_BINARY_TREE_DEFINITION_H_
#include "binary_tree.h"
#endif  // SRC_LIB_BINARY_TREE_DEFINITION_H_

namespace hhullen {

TEMPLATE_DEF
BIN_TREE_DEF::Iterator::Iterator() : node_ptr_{nullptr}, end_{nullptr} {}

TEMPLATE_DEF
BIN_TREE_DEF::Iterator::Iterator(NodePtr node, NodePtr end)
    : node_ptr_{node}, end_{end} {}

TEMPLATE_DEF
Value BIN_TREE_DEF::Iterator::operator*() const { return node_ptr_->value; }

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator& BIN_TREE_DEF::Iterator::operator++() {
  if (node_ptr_ == end_) {
    return *this;
  }
  NodePtr selector_right = node_ptr_->relatives[Node::Right];
  if (selector_right) {
    GoToEnd(selector_right, Node::Left);
    node_ptr_ = selector_right;
  } else {
    NodePtr right_of_parent =
        GetRelative(GetRelative(node_ptr_, Node::Parent), Node::Right);
    if (right_of_parent == node_ptr_) {
      NodePtr parent_of_current = GetRelative(node_ptr_, Node::Parent);
      for (; parent_of_current &&
             Comparator()(KeyRetractor()(parent_of_current->value),
                          KeyRetractor()(node_ptr_->value));
           parent_of_current = GetRelative(node_ptr_, Node::Parent)) {
        node_ptr_ = parent_of_current;
      }
    }
    node_ptr_ = GetRelative(node_ptr_, Node::Parent);
  }
  return *this;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Iterator::operator++(int) {
  typename BIN_TREE_DEF::Iterator iter(*this);
  ++(*this);
  return iter;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator& BIN_TREE_DEF::Iterator::operator--() {
  NodePtr selector_left = node_ptr_->relatives[Node::Left];
  if (selector_left) {
    GoToEnd(selector_left, Node::Right);
    node_ptr_ = selector_left;
  } else {
    NodePtr current = node_ptr_;
    NodePtr left_of_parent =
        GetRelative(GetRelative(current, Node::Parent), Node::Left);
    if (left_of_parent == current) {
      NodePtr parent_of_current = GetRelative(current, Node::Parent);
      for (; parent_of_current &&
             Comparator()(KeyRetractor()(current->value),
                          KeyRetractor()(parent_of_current->value));
           parent_of_current = GetRelative(current, Node::Parent)) {
        current = parent_of_current;
      }
    }
    current = GetRelative(current, Node::Parent);
    if (!current) {
      return *this;
    }
    node_ptr_ = current;
  }
  return *this;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Iterator::operator--(int) {
  typename BIN_TREE_DEF::Iterator iter(*this);
  --(*this);
  return iter;
}

TEMPLATE_DEF
bool BIN_TREE_DEF::Iterator::operator==(
    const typename BIN_TREE_DEF::Iterator& other) const {
  return node_ptr_ == other.node_ptr_;
}

TEMPLATE_DEF
bool BIN_TREE_DEF::Iterator::operator!=(
    const typename BIN_TREE_DEF::Iterator& other) const {
  return node_ptr_ != other.node_ptr_;
}

}  // namespace hhullen
