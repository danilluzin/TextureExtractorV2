//
//  TextureExtractor_mapMap.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 19/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include <stdio.h>
#include "TextureExtractor.hpp"
#include "mapmap/full.h"
#include <set>

bool TextureExtractor::mapMapGetLabeling(){
    using uint_t = unsigned int;
    using cost_t = float;
    constexpr uint_t simd_w = mapmap::sys_max_simd_width<cost_t>();
    using unary_t = mapmap::UnaryTable<cost_t, simd_w>;
    using pairwise_t = mapmap::PairwisePotts<cost_t, simd_w>;
    
    
    uint numNodes = (uint)mesh.adjacencyGraph.nodes.size();
    mapmap::Graph<cost_t> mgraph(mesh.adjacencyGraph.nodes.size());
    
    print("Loading graph into the mapMap solver.(this may take a while)\n");
    for (auto & n : mesh.adjacencyGraph.nodes) {
        if (dataCosts[n.first].empty())
            continue;
        
        std::set<uint> adj_faces = n.second.neighbours;
        for (auto & af : adj_faces) {
            if (dataCosts[af].empty())
                continue;
            
            //TODO:consistency amplifier here
            if (n.first < af) {
                mgraph.add_edge(n.first - 1, af - 1, 1.0f);
            }
        }
    }
    mgraph.update_components();
    
    mapmap::LabelSet<cost_t, simd_w> label_set(numNodes, false);
    for (auto & dc : dataCosts) {
        std::map<uint,PatchQuality> & data_costs_for_node = dc.second;
        
        std::vector<mapmap::_iv_st<cost_t, simd_w> > labels;
        if (data_costs_for_node.empty()) {
            labels.push_back(0);
        } else {
            labels.resize(data_costs_for_node.size());
            uint j = 0;
            for(auto & cost : data_costs_for_node) {
                labels[j] = cost.first;
                j++;
            }
        }
        label_set.set_label_set_for_node(dc.first - 1, labels);
    }
    
    mapmap::UnaryTable<cost_t, simd_w> unaries(&mgraph, &label_set);
    mapmap::PairwisePotts<cost_t, simd_w> pairwise(1.0f);
    for (auto & dc : dataCosts) {
        std::map<uint,PatchQuality> & data_costs_for_node = dc.second;
        
        std::vector<mapmap::_s_t<cost_t, simd_w> > costs;
        if (data_costs_for_node.empty()) {
            costs.push_back(1.0f);
        } else {
            costs.resize(data_costs_for_node.size());
            uint j=0;
            for(auto & cost : data_costs_for_node) {
                costs[j] = cost.second.quality;
                j++;
            }
        }
        unaries.set_costs_for_node(dc.first - 1, costs);
    }
    
    mapmap::StopWhenReturnsDiminish<cost_t, simd_w> terminate(5, 0.01);
    std::vector<mapmap::_iv_st<cost_t, simd_w> > solution;
    
    auto display = [](const mapmap::luint_t time_ms,
                      const mapmap::_iv_st<cost_t, simd_w> objective) {
        std::cout << "\r\t" << time_ms / 1000 << "\t" << objective << std::flush;
    };
    
    auto nodisplay = [](const mapmap::luint_t time_ms,
                      const mapmap::_iv_st<cost_t, simd_w> objective) {};
    
    print("Data loaded into the mapMap solver.\n");
    
    mapmap::mapMAP<cost_t, simd_w, unary_t, pairwise_t> solver;
    solver.set_graph(&mgraph);
    solver.set_label_set(&label_set);
    solver.set_unaries(&unaries);
    solver.set_pairwise(&pairwise);
    
    if(verbose)
        solver.set_logging_callback(display);
    else
        solver.set_logging_callback(nodisplay);
    solver.set_termination_criterion(&terminate);
    
    print("Optimizing:\n\tTime[s]\tEnergy\n");
    solver.optimize(solution);
    
   
    std::size_t num_labels = views.size() + 1;
    std::size_t undefined = 0;
   
    for (std::size_t i = 0; i < numNodes; ++i) {
        int label = label_set.label_from_offset(i, solution[i]);
        if (label < 0 || num_labels <= static_cast<std::size_t>(label)) {
            throw std::runtime_error("Incorrect labeling");
        }
        if (label == 0) undefined += 1;
        mesh.triangles[i + 1].viewId = (uint)static_cast<std::size_t>(label);
    }
    print("Optimizing done\n");
    return true;
}



