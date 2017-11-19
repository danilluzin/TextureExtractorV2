/**
 * Copyright (C) 2017, Daniel Thuerck
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#ifndef __MAPMAP_HEADER_TREE_SAMPLER_H_
#define __MAPMAP_HEADER_TREE_SAMPLER_H_

#include "defines.h"
#include "tree.h"

#include <random>

NS_MAPMAP_BEGIN

template<typename COSTTYPE, bool ACYCLIC>
class TreeSampler
{
public:
    TreeSampler(Graph<COSTTYPE> * graph);
    ~TreeSampler();

    virtual void
        select_random_roots(const luint_t k, std::vector<luint_t>& roots) = 0;
    virtual std::unique_ptr<Tree<COSTTYPE>> sample(std::vector<luint_t>& roots,
        bool record_dependencies, bool relax = true) = 0;

protected:
    void build_component_lists();
    void create_adj_acc();

protected:
    Graph<COSTTYPE> * m_graph;
    std::random_device m_rnd_dev;

    /* data needed in all derived classes */
    std::vector<std::vector<luint_t>> m_component_lists;

    std::vector<luint_t> m_adj_offsets;
    std::vector<luint_t> m_adj;
};

NS_MAPMAP_END

/* include function implementations */
#include "../tree_sampler.impl.h"

#endif /* __MAPMAP_HEADER_TREE_SAMPLER_H_ */
