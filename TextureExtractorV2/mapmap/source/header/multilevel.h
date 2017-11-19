/**
 * Copyright (C) 2016, Daniel Thuerck
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#ifndef __MAPMAP_HEADER_MULTILEVEL_H_
#define __MAPMAP_HEADER_MULTILEVEL_H_

#include <memory>
#include <vector>

#include "tbb/mutex.h"

#include "defines.h"
#include "graph.h"
#include "costs.h"

#include "cost_instances/unary_table.h"
#include "cost_instances/pairwise_table.h"

NS_MAPMAP_BEGIN

template<typename COSTTYPE, uint_t SIMDWIDTH>
struct LevelSet
{
    /* might be modified by coloring */
    Graph<COSTTYPE> * level_graph;
    const LabelSet<COSTTYPE, SIMDWIDTH> * level_label_set;
    const UnaryCosts<COSTTYPE, SIMDWIDTH> * level_unaries;
    const PairwiseCosts<COSTTYPE, SIMDWIDTH> * level_pairwise;

    /* associate nodes from previous level with nodes in this level */
    std::vector<luint_t> prev_node_in_group;
};

/* ************************************************************************** */

/**
 * Given a level set (graph and costs) and a labelling, groups nodes
 * into supernodes. Does not require a contiguous ID range for supernodes.
 * That will be handled in the multilevel master class.
 *
 * Class must also be able to project the old solution to the
 * upper level (by choosing one label in each supernode e.g.), use
 * place of representative node in projected_solution.
 */
template<typename COSTTYPE, uint_t SIMDWIDTH>
class MultilevelCriterion
{
public:
    ~MultilevelCriterion() {}

    virtual void group_nodes(std::vector<luint_t>& node_in_group,
        const LevelSet<COSTTYPE, SIMDWIDTH> * current_level,
        const std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>& current_solution,
        std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>& projected_solution) = 0;
};

/* ************************************************************************** */

/**
 * A customizable multilevel solver. Starting from a given MRF, it coarsens (and
 * possibly refines) the graph and costs iteratively by grouping nodes
 * according to some criterion. The coarsened graph is another MRF to be
 * solved; its solution may i.e. be reprojected onto the original MRF.
 *
 * The criterion/algorithm for grouping nodes must be specified as an instance
 * of MultilevelCriterion.
 */

template<typename COSTTYPE, uint_t SIMDWIDTH, typename UNARY, typename PAIRWISE>
class Multilevel
{
public:
    Multilevel(Graph<COSTTYPE> * original_graph,
        const LabelSet<COSTTYPE, SIMDWIDTH> * original_label_set,
        const UNARY * original_unaries,
        const PAIRWISE * original_pairwise,
        MultilevelCriterion<COSTTYPE, SIMDWIDTH> *
            criterion);
    ~Multilevel();

    const UnaryTable<COSTTYPE, SIMDWIDTH> * get_level_unaries() const throw();
    const PairwiseTable<COSTTYPE, SIMDWIDTH> * get_level_pairwise() const
        throw();
    Graph<COSTTYPE> * get_level_graph() const throw();
    const LabelSet<COSTTYPE, SIMDWIDTH> * get_level_label_set() const throw();

    bool prev_level();
    bool next_level(const std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>&
        current_solution, std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>&
        projected_solution);
    void reproject_solution(const std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>&
        level_solution, std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>&
        original_solution);

protected:

    /* common functionalities */
    void compute_contiguous_ids(const
        std::vector<_iv_st<COSTTYPE, SIMDWIDTH>>& projected_solution);
    void compute_level_label_set() throw();
    void compute_level_unaries();
    void compute_level_pairwise();
    void compute_level_graph_from_node_groups();

protected:
    /* node grouping algorithm/criterion */
    MultilevelCriterion<COSTTYPE, SIMDWIDTH> *
        m_criterion;

    /* all levels - [0] is the original  */
    std::vector<LevelSet<COSTTYPE, SIMDWIDTH>> m_levels;

    /* current level */
    luint_t m_level;

    /* pointers for quick access to important levels */
    LevelSet<COSTTYPE, SIMDWIDTH> * m_previous;
    LevelSet<COSTTYPE, SIMDWIDTH> * m_current;

    /* data storage for the level structures */
    std::vector<std::unique_ptr<Graph<COSTTYPE>>> m_storage_graph;
    std::vector<std::unique_ptr<LabelSet<COSTTYPE, SIMDWIDTH>>>
        m_storage_label_set;
    std::vector<std::unique_ptr<UnaryTable<COSTTYPE, SIMDWIDTH>>>
        m_storage_unaries;
    std::vector<std::unique_ptr<PairwiseTable<COSTTYPE, SIMDWIDTH>>>
        m_storage_pairwise;

    /**
     * temporary data for the construction of the next level -
     * creeated from scratch for every level
     */

    /* node grouping data */
    luint_t m_num_supernodes;

    /* record which nodes are contained in which supernodes */
    std::vector<luint_t> m_supernode_sizes;
    std::vector<luint_t> m_supernode_offsets;
    std::vector<luint_t> m_supernode_list;

    /* record which original edges are incident with which supernode */
    std::vector<luint_t> m_superedge_sizes;
    std::vector<luint_t> m_superedge_offsets;
    std::vector<luint_t> m_superedge_list;

    /* save chosen labels for supernodes (-> dependencies lateron) */
    std::vector<_iv_st<COSTTYPE, SIMDWIDTH>> m_labels;

    /* mutex for writing access to the graph */
    tbb::mutex m_graph_write_mutex;

    /* debug */
    std::vector<_iv_st<COSTTYPE, SIMDWIDTH>> m_solution;
};

NS_MAPMAP_END

#include "../multilevel.impl.h"

#endif /* __MAPMAP_HEADER_MULTILEVEL_H_ */
