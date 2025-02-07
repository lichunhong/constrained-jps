#ifndef WARTHOG_BIDIRECTIONAL_GRAPH_EXPANSION_POLICY_H
#define WARTHOG_BIDIRECTIONAL_GRAPH_EXPANSION_POLICY_H

// search/bidirectional_graph_expansion_policy.h
//
// An expansion policy for bidirectional search algorithms.
// When expanding in the forward direction this policy
// generates all outgoing neighbours as successors.
// When expanding in the backward direction this policy
// generates all incoming neighbours as successors.
//
// @author: dharabor
// @created: 2019-08-31
//

#include "expansion_policy.h"
#include "xy_graph.h"

#include <vector>

namespace warthog{

class problem_instance;
class search_node;

class bidirectional_graph_expansion_policy : public  expansion_policy
{
    public:
        // @param g: the input contracted graph
        // @param backward: when true successors are generated by following 
        // incoming arcs rather than outgoing arcs (default is outgoing)
        bidirectional_graph_expansion_policy(warthog::graph::xy_graph* g, 
                bool backward=false);

        virtual 
        ~bidirectional_graph_expansion_policy() { }

		virtual void 
		expand(warthog::search_node*, warthog::problem_instance*);

        virtual void
        get_xy(warthog::sn_id_t node_id, int32_t& x, int32_t& y)
        {
            g_->get_xy((uint32_t)node_id, x, y);
        }

        inline size_t
        get_num_nodes() { return g_->get_num_nodes(); }

        virtual warthog::search_node* 
        generate_start_node(warthog::problem_instance* pi);

        virtual warthog::search_node*
        generate_target_node(warthog::problem_instance* pi);

        virtual size_t
        mem();

    private:
        bool backward_;
        warthog::graph::xy_graph* g_;

        // we use function pointers to optimise away a 
        // branching instruction when fetching successors
        typedef warthog::graph::edge_iter
                (warthog::bidirectional_graph_expansion_policy::*chep_get_iter_fn) 
                (warthog::graph::node* n);
        
        // pointers to the neighbours in the direction of the search
        chep_get_iter_fn fn_begin_iter_;
        chep_get_iter_fn fn_end_iter_;

        // pointers to neighbours in the reverse direction to the search
        chep_get_iter_fn fn_rev_begin_iter_;
        chep_get_iter_fn fn_rev_end_iter_;

        inline warthog::graph::edge_iter
        get_fwd_begin_iter(warthog::graph::node* n) 
        { return n->outgoing_begin(); }

        inline warthog::graph::edge_iter
        get_fwd_end_iter(warthog::graph::node* n) 
        { return n->outgoing_end(); }

        inline warthog::graph::edge_iter
        get_bwd_begin_iter(warthog::graph::node* n) 
        { return n->incoming_begin(); }

        inline warthog::graph::edge_iter
        get_bwd_end_iter(warthog::graph::node* n) 
        { return n->incoming_end(); }
};

}
#endif

