/**
 * Copyright (C) 2016, Daniel Thuerck
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#ifndef __MAPMAP_DYNAMIC_PROGRAMMING_H_
#define __MAPMAP_DYNAMIC_PROGRAMMING_H_

#include <memory>
#include <vector>

#include "tbb/blocked_range.h"
#include "tbb/concurrent_vector.h"
#include "tbb/tbb_allocator.h"

#include "defines.h"
#include "tree.h"
#include "vector_types.h"
#include "vector_math.h"
#include "costs.h"
#include "tree_optimizer.h"
#include "parallel_templates.h"

NS_MAPMAP_BEGIN

template<typename T>
using tbb_allocator_ptr = std::shared_ptr<tbb::tbb_allocator<T>>; 

template<typename COSTTYPE, uint_t SIMDWIDTH, typename UNARY, typename PAIRWISE>
struct DPBundle
{
    const LabelSet<COSTTYPE, SIMDWIDTH> * c_labels;

    const UNARY * c_unary;
    const PAIRWISE * c_binary;

    const std::vector<_s_t<COSTTYPE, SIMDWIDTH>*> * c_child_values;
    const std::vector<_iv_st<COSTTYPE, SIMDWIDTH>*> * c_child_labels;

    const std::vector<_iv_st<COSTTYPE, SIMDWIDTH>> * c_assignment; 
    bool respect_dependencies;
};

template<typename COSTTYPE, uint_t SIMDWIDTH, typename UNARY, typename PAIRWISE>
class DynamicProgrammingTableEntry
{
public:
    DynamicProgrammingTableEntry(
        const TreeNode<COSTTYPE>& node,
        _s_t<COSTTYPE, SIMDWIDTH> * tbl_opt_values,
        _iv_st<COSTTYPE, SIMDWIDTH> * tbl_opt_labels);
    ~DynamicProgrammingTableEntry();

    const luint_t& node_id();
    const luint_t& parent_id();

    void optimize_entry(
        const DPBundle<COSTTYPE, SIMDWIDTH, UNARY, PAIRWISE>& costs);
    const _s_t<COSTTYPE, SIMDWIDTH> * optimal_value();
    const _iv_st<COSTTYPE, SIMDWIDTH> * optimal_labels();

protected:
    TreeNode<COSTTYPE> m_node;
    _s_t<COSTTYPE, SIMDWIDTH> * m_opt_values;
    _iv_st<COSTTYPE, SIMDWIDTH> * m_opt_labels; 
};

template<typename COSTTYPE, uint_t SIMDWIDTH, typename UNARY, typename PAIRWISE>
class CombinatorialDynamicProgramming : 
    public TreeOptimizer<COSTTYPE, SIMDWIDTH, UNARY, PAIRWISE>
{
public:
    CombinatorialDynamicProgramming();
    ~CombinatorialDynamicProgramming();

    _s_t<COSTTYPE, SIMDWIDTH> optimize(
        std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>& solution) throw();

protected:
    void discover_leaves();
    void allocate_memory();

    void node_memory_allocate(const luint_t node_id);
    void node_memory_clean_children(const luint_t node_id);

    void bottom_up_opt();
    void top_down_opt(std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>& solution);

    _s_t<COSTTYPE, SIMDWIDTH> * value_ptr();
    _iv_st<COSTTYPE, SIMDWIDTH> * index_ptr();
    std::vector<luint_t> * current_level_queue_ptr();
    std::vector<luint_t> * next_level_queue_ptr();

    void next_level();

protected:
    luint_t m_level;
    luint_t m_level_size;

    tbb_allocator_ptr<_s_t<COSTTYPE, SIMDWIDTH>> m_value_allocator;

    std::vector<_iv_st<COSTTYPE, SIMDWIDTH>> m_opt_labels;
    std::vector<_s_t<COSTTYPE, SIMDWIDTH>> m_opt_values;

    std::vector<_s_t<COSTTYPE, SIMDWIDTH>*> m_opt_value_nodes;
    std::vector<_iv_st<COSTTYPE, SIMDWIDTH>> m_opt_value_sizes;
    std::vector<_iv_st<COSTTYPE, SIMDWIDTH>*> m_opt_label_nodes;

    std::vector<luint_t> m_queue_a;
    std::vector<luint_t> m_queue_b;

    std::vector<luint_t> m_leaf_ids;
    tbb::concurrent_vector<luint_t> m_root_ids;
};

NS_MAPMAP_END

#include "../dynamic_programming.impl.h"

#endif /* __MAPMAP_DYNAMIC_PROGRAMMING_H_ */
