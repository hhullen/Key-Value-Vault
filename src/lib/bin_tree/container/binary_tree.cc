#ifndef SRC_LIB_BINARY_TREE_DEFINITION_H_
#include "binary_tree.h"
#endif  // SRC_LIB_BINARY_TREE_DEFINITION_H_

namespace hhullen {

TEMPLATE_DEF
BIN_TREE_DEF::BinTree() : root_{new Node()}, end_{root_}, size_{0} {}

TEMPLATE_DEF
BIN_TREE_DEF::~BinTree() {
  if (root_) {
    delete root_;
    root_ = nullptr;
  }
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Begin() {
  if (root_ != end_) {
    NodePtr selector = root_;
    GoToEnd(selector, Node::Left);
    return Iterator(selector, end_);
  } else {
    return Iterator(end_, end_);
  }
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::End() {
  return Iterator(end_, end_);
}

TEMPLATE_DEF
void BIN_TREE_DEF::Clear() {
  root_->~Node();
  end_ = root_;
  size_ = 0;
}

TEMPLATE_DEF
bool BIN_TREE_DEF::Contains(const Key& key) {
  if (Find(key) != End()) {
    return true;
  }
  return false;
}

TEMPLATE_DEF
bool BIN_TREE_DEF::Empty() { return size_ == 0; }

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Find(const Key& key) {
  NodePtr found = Seek(key).second;
  if (!found) {
    found = end_;
  }
  return Iterator(found, end_);
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Emplace(const Value& value) {
  const Key key = KeyRetractor()(value);
  NodePtrPair found = Seek(key);
  if (found.second == end_) {
    SetNewNodeOnEnd(found.second, value);
    RunBalancingFromNode(found.second);
    ++size_;
  } else if (found.second) {
    found.second->value = value;
  } else {
    RunBalancingFromNode(SetNewNodeOnNull(found.first, value));
    ++size_;
  }
  return Iterator(found.second, end_);
}

TEMPLATE_DEF
typename BIN_TREE_DEF::Iterator BIN_TREE_DEF::Delete(const Key& key) {
  NodePtr found = Seek(key).second;
  if (!found || found == end_) {
    return Iterator(end_, end_);
  }
  Iterator next = ++Iterator(found, end_);
  NodePtr parent_to_balance = found->relatives[Node::Parent];
  if (!found->relatives[Node::Left] && !found->relatives[Node::Right]) {
    NodePtr& node = GetSelfFromNodeParent(found);
    delete node;
    node = nullptr;
  } else if (found->relatives[Node::Right] == end_ &&
             !found->relatives[Node::Left]) {
    ReplaceNode(found, end_);
  } else if (!found->relatives[Node::Right] ||
             found->relatives[Node::Right] == end_) {
    ReplaceNode(found, found->relatives[Node::Left]);
  } else {
    DeleteWithBothChilds(found);
  }
  RunBalancingFromNode(parent_to_balance);
  --size_;
  return next;
}

TEMPLATE_DEF
size_t BIN_TREE_DEF::Size() { return size_; }

TEMPLATE_DEF
size_t BIN_TREE_DEF::Height() { return CalculateHeight(root_); }

// /*
//     private methods
// */

TEMPLATE_DEF
void BIN_TREE_DEF::SetNewNodeOnEnd(NodePtr& node, const Value& value) {
  node->value = value;
  node->relatives[Node::Right] = new Node();
  end_ = node->relatives[Node::Right];
  end_->relatives[Node::Parent] = node;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::NodePtr& BIN_TREE_DEF::SetNewNodeOnNull(
    NodePtr& prev, const Value& value) {
  bool relative_selector =
      Comparator()(KeyRetractor()(prev->value), KeyRetractor()(value));
  NodePtr& node_curr = prev->relatives[relative_selector];
  node_curr = new Node();
  node_curr->value = value;
  node_curr->relatives[Node::Parent] = prev;
  return node_curr;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::NodePtrPair BIN_TREE_DEF::Seek(const Key& key) {
  NodePtr selector = root_, previous = root_;
  for (; selector && selector != end_;) {
    previous = selector;
    const Key selected_key = KeyRetractor()(selector->value);
    if (IsEQ(key, selected_key)) {
      break;
    }
    selector = selector->relatives[Comparator()(selected_key, key)];
  }
  return {previous, selector};
}

TEMPLATE_DEF
void BIN_TREE_DEF::ReplaceNode(NodePtr& to_replace, NodePtr& src) {
  ReplaceNodeRelative(to_replace, src, Node::Left);
  ReplaceNodeRelative(to_replace, src, Node::Right);
  src->relatives[Node::Parent] = to_replace->relatives[Node::Parent];

  NodePtr& src_parent = src->relatives[Node::Parent];
  if (src_parent) {
    if (src_parent->relatives[Node::Left] == to_replace) {
      src_parent->relatives[Node::Left] = src;
    } else if (src_parent->relatives[Node::Right] == to_replace) {
      src_parent->relatives[Node::Right] = src;
    }
  }

  if (!src->relatives[Node::Parent]) {
    root_ = src;
  }
  to_replace->relatives[Node::Left] = nullptr;
  to_replace->relatives[Node::Right] = nullptr;
  delete to_replace;
  to_replace = nullptr;
}

TEMPLATE_DEF
void BIN_TREE_DEF::ReplaceNodeRelative(NodePtr& to_replace, NodePtr& replacer,
                                       size_t relative) {
  if (to_replace->relatives[relative] != replacer) {
    replacer->relatives[relative] = to_replace->relatives[relative];
    if (replacer->relatives[relative]) {
      replacer->relatives[relative]->relatives[Node::Parent] = replacer;
    }
  }
}

TEMPLATE_DEF
void BIN_TREE_DEF::DeleteWithNoChilds(NodePtr& node) {
  NodePtr parent = node->relatives[Node::Parent];
  if (parent->relatives[Node::Left] == node) {
    parent->relatives[Node::Left] = nullptr;
  } else {
    parent->relatives[Node::Right] = nullptr;
  }
}

TEMPLATE_DEF
void BIN_TREE_DEF::DeleteWithBothChilds(NodePtr& node) {
  NodePtr exchange_node = node->relatives[Node::Right];
  if (exchange_node->relatives[Node::Left]) {
    GoToEnd(exchange_node, Node::Left);
    if (exchange_node->relatives[Node::Right]) {
      NodePtr& parent_self_ptr = GetSelfFromNodeParent(exchange_node);
      parent_self_ptr = exchange_node->relatives[Node::Right];
      parent_self_ptr->relatives[Node::Parent] =
          exchange_node->relatives[Node::Parent];
      ReplaceNode(node, exchange_node);
    } else {
      GetSelfFromNodeParent(exchange_node) = nullptr;
      ReplaceNode(node, exchange_node);
    }
  } else {
    ReplaceNode(node, exchange_node);
  }
  if (!exchange_node->relatives[Node::Parent]) {
    root_ = exchange_node;
  }
}

TEMPLATE_DEF
size_t BIN_TREE_DEF::CalculateHeight(const NodePtr& start_node) {
  if (!start_node || start_node == end_) {
    return 0;
  }
  size_t height = 0;
  std::vector<NodePtr> traversal_targets[2];
  bool selector = 0;
  traversal_targets[selector].emplace_back(start_node);
  for (; !traversal_targets[selector].empty(); ++height) {
    for (size_t i = 0; i < traversal_targets[selector].size(); ++i) {
      NodePtr& node = traversal_targets[selector][i];
      if (node->relatives[Node::Left] && node->relatives[Node::Left] != end_) {
        traversal_targets[!selector].emplace_back(node->relatives[Node::Left]);
      }
      if (node->relatives[Node::Right] &&
          node->relatives[Node::Right] != end_) {
        traversal_targets[!selector].emplace_back(node->relatives[Node::Right]);
      }
    }
    traversal_targets[selector].clear();
    selector = !selector;
  }
  return height;
}

TEMPLATE_DEF
typename BIN_TREE_DEF::NodePtr& BIN_TREE_DEF::GetSelfFromNodeParent(
    NodePtr& node) {
  if (node->relatives[Node::Parent]->relatives[Node::Left] == node) {
    return node->relatives[Node::Parent]->relatives[Node::Left];
  } else {
    return node->relatives[Node::Parent]->relatives[Node::Right];
  }
}

TEMPLATE_DEF
void BIN_TREE_DEF::RunBalancingFromNode(NodePtr node) {
  for (; node; node = node->relatives[Node::Parent]) {
    int height_diff = GetChilrenHeightDifference(node);
    if (height_diff > 1) {
      if (GetChilrenHeightDifference(node->relatives[Node::Right]) < 0) {
        RotateRight(node->relatives[Node::Right]);
      }
      RotateLeft(node);
      return;
    } else if (height_diff < -1) {
      if (GetChilrenHeightDifference(node->relatives[Node::Left]) > 0) {
        RotateLeft(node->relatives[Node::Left]);
      }
      RotateRight(node);
      return;
    }
  }
}

TEMPLATE_DEF
int BIN_TREE_DEF::GetChilrenHeightDifference(const NodePtr& node) {
  if (node) {
    return static_cast<int>(CalculateHeight(node->relatives[Node::Right]) -
                            CalculateHeight(node->relatives[Node::Left]));
  }
  return 0;
}

TEMPLATE_DEF
void BIN_TREE_DEF::RotateLeft(NodePtr& node) {
  NodePtr to_turn = node->relatives[Node::Right];
  node->relatives[Node::Right] = to_turn->relatives[Node::Left];
  if (node->relatives[Node::Right]) {
    node->relatives[Node::Right]->relatives[Node::Parent] = node;
  }

  NodePtr& to_turn_left = to_turn->relatives[Node::Left];
  to_turn_left = node;
  if (to_turn_left) {
    to_turn->relatives[Node::Parent] = to_turn_left->relatives[Node::Parent];
    if (to_turn->relatives[Node::Parent]) {
      if (to_turn->relatives[Node::Parent]->relatives[Node::Left] == node) {
        to_turn->relatives[Node::Parent]->relatives[Node::Left] = to_turn;
      } else {
        to_turn->relatives[Node::Parent]->relatives[Node::Right] = to_turn;
      }
    } else {
      root_ = to_turn;
    }
    to_turn_left->relatives[Node::Parent] = to_turn;
  }
}

TEMPLATE_DEF
void BIN_TREE_DEF::RotateRight(NodePtr node) {
  NodePtr to_turn = node->relatives[Node::Left];
  node->relatives[Node::Left] = to_turn->relatives[Node::Right];
  if (node->relatives[Node::Left]) {
    node->relatives[Node::Left]->relatives[Node::Parent] = node;
  }

  NodePtr& to_turn_right = to_turn->relatives[Node::Right];
  to_turn_right = node;
  if (to_turn_right) {
    to_turn->relatives[Node::Parent] = to_turn_right->relatives[Node::Parent];
    if (to_turn->relatives[Node::Parent]) {
      if (to_turn->relatives[Node::Parent]->relatives[Node::Left] == node) {
        to_turn->relatives[Node::Parent]->relatives[Node::Left] = to_turn;
      } else {
        to_turn->relatives[Node::Parent]->relatives[Node::Right] = to_turn;
      }
    } else {
      root_ = to_turn;
    }
    to_turn_right->relatives[Node::Parent] = to_turn;
  }
}

// /*
//     private static methods
// */

TEMPLATE_DEF
bool BIN_TREE_DEF::IsEQ(const Key& key1, const Key& key2) {
  return !(Comparator()(key1, key2) || Comparator()(key2, key1));
}

TEMPLATE_DEF
typename BIN_TREE_DEF::NodePtr& BIN_TREE_DEF::GetRelative(const NodePtr& node,
                                                          size_t direction) {
  return node->relatives[direction];
}

TEMPLATE_DEF
void BIN_TREE_DEF::GoToEnd(NodePtr& selector, size_t direction) {
  for (; selector->relatives[direction];
       selector = selector->relatives[direction]) {
  }
}

}  // namespace hhullen
