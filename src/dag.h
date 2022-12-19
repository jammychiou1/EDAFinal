#ifndef __DAG_H__
#define __DAG_H__

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

template <typename NodeData>
class DAG {
 public:
  class NodeInfo {
    friend DAG;

   private:
    int idx;
    std::vector<int> ins;
    std::vector<int> outs;
    NodeInfo(int idx, std::vector<int> ins = {}, std::vector<int> outs = {})
        : idx(idx), ins(ins), outs(outs) {}
  };

 private:
  std::vector<NodeInfo> m_node_infos;
  std::vector<NodeData> m_nodes;
  bool m_commited = false;
  std::vector<int> m_topo_ord_to_idx;
  std::vector<int> m_idx_to_topo_ord;

  void m_topo_sort() {
    std::vector<int> topo = {};
    std::vector<int> in_deg = {};

    int num_nodes = m_nodes.size();
    int ord = 0;
    for (int idx = 0; idx < num_nodes; idx++) {
      NodeInfo &info = m_node_infos[idx];
      if (info.ins.size() == 0) {
        topo.push_back(idx);
      }

      in_deg.push_back(info.ins.size());
    }

    while (ord < topo.size()) {
      NodeInfo &info = m_node_infos[topo[ord]];
      ord++;
      for (int idx : info.outs) {
        in_deg[idx]--;
        if (in_deg[idx] == 0) {
          topo.push_back(idx);
        }
      }
    }

    assert(topo.size() == num_nodes);

    m_topo_ord_to_idx = topo;

    m_idx_to_topo_ord.resize(num_nodes);

    for (int ord = 0; ord < num_nodes; ord++) {
      m_idx_to_topo_ord[m_topo_ord_to_idx[ord]] = ord;
    }
  }

 public:
  // class NodeHandle {
  //   friend DAG;

  //  private:
  //   int idx;
  //   DAG *outer;

  //  public:
  //   NodeHandle(int idx = 0, DAG *outer = nullptr) : idx(idx), outer(outer) {}

  //   bool operator==(const NodeHandle &other) const { return idx == other.idx; }

  //   bool operator!=(const NodeHandle &other) const { return idx != other.idx; }

  //   NodeData &operator*() const { return outer->m_nodes[idx]; }

  //   NodeData *operator->() const { return &outer->m_nodes[idx]; }

  //   NodeHandle &operator++() {
  //     int topo = outer->m_idx_to_topo_ord[idx];
  //     idx = outer->m_topo_ord_to_idx[topo + 1];
  //     return *this;
  //   }
  // };

  int add_node(NodeData data = {}) {
    int idx = m_nodes.size();
    m_nodes.push_back(data);
    m_node_infos.push_back(NodeInfo{idx, {}, {}});
    // std::cerr << "new node " << idx << '\n';
    return idx;
  }

  void add_edge(int u, int v) {
    m_node_infos[u].outs.push_back(v);
    m_node_infos[v].ins.push_back(u);
    // std::cerr << "new edge " << u << "->" << v << '\n';
  }

  // NodeHandle begin() {
  //   assert(m_commited);
  //   return {m_topo_ord_to_idx[0], this};
  // }

  // NodeHandle end() {
  //   assert(m_commited);
  //   return {m_topo_ord_to_idx[m_nodes.size() - 1], this};
  // }

  NodeData &get_node (int idx) {
    return m_nodes[idx];
  }

  std::vector<int> topo_ord_to_idx() {
    assert(m_commited);
    return m_topo_ord_to_idx;
  }

  std::vector<int> in_neighbours(int idx) {
    assert(m_commited);
    return m_node_infos[idx].ins;
  }

  std::vector<int> out_neighbours(int idx) {
    assert(m_commited);
    return m_node_infos[idx].outs;
  }

  void commit() {
    assert(!m_commited);
    m_commited = true;
    m_topo_sort();
  }
};

#endif  // __DAG_H__
