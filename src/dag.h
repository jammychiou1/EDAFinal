#ifndef __DAG_H__
#define __DAG_H__

#include <cassert>
#include <functional>
#include <map>
#include <optional>
#include <vector>

template <typename NodeData>
class DAG {
 private:
  struct NodeInfo {
    int idx;
    std::vector<int> ins;
    std::vector<int> outs;
  };

  std::vector<std::pair<NodeInfo, NodeData>> m_nodes;
  std::optional<std::vector<int>> m_nodes_topo;

  void m_generate_topo() {
    assert(!m_nodes_topo);
    std::vector<int> topo = {};
    std::vector<int> in_deg = {};

    int now = 0;
    for (int i = 0; i < m_nodes.size(); i++) {
      auto &[info, data] = m_nodes[i];
      if (info.ins.size() == 0) {
        topo.push_back(i);
      }

      in_deg.push_back(info.ins.size());
    }

    while (now < topo.size()) {
      auto &[info, data] = m_nodes[topo[now]];
      now++;
      for (int v : info.outs) {
        in_deg[v]--;
        if (in_deg[v] == 0) {
          topo.push_back(v);
        }
      }
    }

    assert(topo.size() == m_nodes.size());

    m_nodes_topo = topo;
  }

 public:
  class Iterator {
   private:
    std::vector<int>::iterator itr;
    std::vector<std::pair<NodeInfo, NodeData>> *m_nodes;

   public:
    const std::pair<const NodeInfo, NodeData> &operator*() const {
      return m_nodes->at(*itr);
    }

    const std::pair<const NodeInfo, NodeData> *operator->() const {
      return &m_nodes->at(*itr);
    }

    Iterator &operator++() {
      ++itr;
      return *this;
    }
  };

  int add_node(NodeData data = {}) {
    int idx = m_nodes.size();
    m_nodes.emplace_back(NodeInfo{idx, {}, {}}, data);
    return idx;
  }

  int add_edge(int u, int v) {
    m_nodes[u].first.outs.push_back(v);
    m_nodes[v].first.ins.push_back(u);
  }

  Iterator begin() {
    assert(m_nodes_topo);
    return {m_nodes_topo->begin(), &m_nodes};
  }

  Iterator end() {
    assert(m_nodes_topo);
    return {m_nodes_topo->end(), &m_nodes};
  }

  void commit() { m_generate_topo(); }
};

#endif  // __DAG_H__
