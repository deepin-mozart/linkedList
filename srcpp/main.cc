/*******************************************************************************
 * Copyright (c) 2021, Initialsoft, Inc.
 * All rights reserved.
 *
 * main : [description]
 *
 * filename : main.cc
 * author   : hunt978(hunt978@initialsoft.com)
 * create   : 2021-03-15 03:15:35 UTC
 * modified : 2021-03-17 16:24:32 UTC
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Headers
//
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Typedefs & Constants
//
////////////////////////////////////////////////////////////////////////////////
// Inner Scope Objects
//
/**
 * storage pair of id
 */
struct Node final {
  std::string fid;
  std::string tid;
};

bool ascendFunc(const Node &pre, const Node &behind)
{
    if (pre.fid < behind.fid || (pre.fid == behind.fid && pre.tid < behind.tid))
        return true;
    return false;
}

bool decendFunc(const Node &pre, const Node &behind)
{
    if (pre.fid < behind.fid || (pre.fid == behind.fid && pre.tid < behind.tid))
        return false;
    return true;
}

/**
 * @brief      sort nodes
 *
 * @param      nodes   The nodes
 * @param[in]  accend  The accend
 */
void sort(std::vector<Node>& nodes, bool ascend)
{
    if (ascend) {
        std::sort(nodes.begin(), nodes.end(), ascendFunc);
    } else {
        std::sort(nodes.begin(), nodes.end(), decendFunc);
    }
}

/**
 * @brief      insert a new pair into the list
 *
 *             there are 4 strains for the container :
 *
 *               1) from id and to id **CANNOT** be the same
 *               2) it should **NOT** hold duplicated node
 *               3) it should **NOT** hold circled link
 *               4) fan-out is allowed while fan-in is **NOT**.
 *
 * @param      nodes  The nodes
 * @param[in]  fid    The fid
 * @param[in]  tid    The tid
 *
 * @return     success with true returned
 */
bool insert(
  std::vector<Node>& nodes, const std::string& fid, const std::string& tid)
{
    // Rule1 no equeal
    if (fid == tid)
        return false;

    int prev_node = 0;
    int loopCount = 1;
    for (auto node : nodes) {
        // Rule2 no repeat
        if (node.fid == fid && node.tid == tid)
            return false;

        // Rule3 no closed loop
        if (tid == node.fid)
            return false;

        // Rule4 Fan-out is allowed, fan-in is prohibited
        // tid should not appear twice.
        if (tid == node.tid)
            return false;

#if 1   // I'm not sure if this is needed because of the sort at 142 line before instert at 143 line,
        // If the title means this, please change 1 to 0.
        if (!prev_node && node.fid < fid)
            prev_node = loopCount;
#endif

        loopCount++;
    }

    Node n;
    n.fid = fid;
    n.tid = tid;

    // insert node in order.
    nodes.insert(nodes.begin() + prev_node, n);

    return true;
}

/**
 * @brief      dump list information
 *
 * @param[in]  nodes  The nodes
 */
void dump(const std::vector<Node>& nodes)
{
  std::cout << "---------------------------" << std::endl;
  for (const auto& node: nodes) {
    std::cout << "  " << node.fid << " -> " << node.tid << std::endl;
  }
}

/**
 * @brief      Does a test.
 */
void do_test()
{
  std::vector<Node> nodes;

  sort(nodes, true);
  insert(nodes, "1", "2");
  dump(nodes);

  sort(nodes, true);
  insert(nodes, "2", "4");
  dump(nodes);

  sort(nodes, true);
  insert(nodes, "2", "3");
  dump(nodes);

  sort(nodes, true);
  insert(nodes, "3", "4");
  dump(nodes);

  sort(nodes, true);
  insert(nodes, "4", "1");
  dump(nodes);

  sort(nodes, true);
  insert(nodes, "1", "2");
  dump(nodes);
}

/**
 * @brief      Does a test 2.
 */
void do_test2()
{
  std::vector<Node> nodes;
  insert(nodes, "1", "2");
  sort(nodes, false);
  dump(nodes);
  insert(nodes, "2", "3");
  sort(nodes, false);
  dump(nodes);
  insert(nodes, "2", "4");
  sort(nodes, false);
  dump(nodes);
  insert(nodes, "3", "4");
  sort(nodes, false);
  dump(nodes);
  insert(nodes, "4", "1");
  sort(nodes, false);
  dump(nodes);
  insert(nodes, "1", "2");
  sort(nodes, false);
  dump(nodes);
}

////////////////////////////////////////////////////////////////////////////////
// Classes
//

////////////////////////////////////////////////////////////////////////////////
// Functions
//
int main(int argc, char const* argv[])
{
  do_test();
  do_test2();
  return 0;
}

////////////////////////////////// EOF /////////////////////////////////////////
