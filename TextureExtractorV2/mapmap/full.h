/**
 * Copyright (C) 2016, Daniel Thuerck
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#ifndef __MAPMAP_FULL_H_
#define __MAPMAP_FULL_H_

/* basic classes */
#include "source/header/costs.h"
#include "source/header/defines.h"
#include "source/header/dynamic_programming.h"
#include "source/header/graph.h"
#include "source/header/mapmap.h"
#include "source/header/multilevel.h"
#include "source/header/parallel_templates.h"
#include "source/header/termination_criterion.h"
#include "source/header/tree.h"
#include "source/header/tree_optimizer.h"
#include "source/header/tree_sampler.h"
#include "source/header/vector_math.h"
#include "source/header/vector_types.h"

/* cost functions */
#include "source/header/cost_instances/pairwise_antipotts.h"
#include "source/header/cost_instances/pairwise_potts.h"
#include "source/header/cost_instances/pairwise_table.h"
#include "source/header/cost_instances/pairwise_truncated_linear.h"
#include "source/header/cost_instances/unary_table.h"

/* termination criteria */
#include "source/header/termination_instances/stop_after_iterations.h"
#include "source/header/termination_instances/stop_after_time.h"
#include "source/header/termination_instances/stop_when_flat.h"
#include "source/header/termination_instances/stop_when_returns_diminish.h"

/* node grouping criteria */
#include "source/header/multilevel_instances/group_same_label.h"

#endif /* __MAPMAP_FULL_H_ */
