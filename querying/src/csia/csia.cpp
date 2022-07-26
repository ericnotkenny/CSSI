/**
 * @file
 * Implementation of CSIA algorithm.
 */
#include "csia.hpp"
#include <chrono> // for timing
#include "../util/commons.hpp"
#include "../util/constants.hpp"
#include "../util/topkPriorityQueue2.hpp"

namespace sstss
{

    void CSIA::query(uint32_t k, DocId const& qid, double const& a, ResultVec &results,
		     uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist)
    {
        TopKPriorityQueue2 docscore_queue(k);
	Doc q = corpus_.docvec.at(qid);
	Doc q_ld = {q.first, corpus_.emb_ld.at(qid)};
	double U = 0;
	prunes = 0;
	prunes2 = 0;
	
	std::vector < std::pair < double, size_t > > sorted_index; 
	for (size_t i=0; i < corpus_.hybrid_index.size(); i++) {
	    double d =  distance_doc_cluster_ld(q.first, corpus_.emb_ld.at(qid), corpus_.hybrid_index.at(i), a, corpus_.max_space_distance, corpus_.max_semantic_distance_red);
	    sorted_index.push_back({d, i});
	}
	sort(sorted_index.begin(), sorted_index.end());

	for(size_t i = 0; i < sorted_index.size(); i++) {
	    for ( DocId obj : corpus_.hybrid_index.at(sorted_index.at(i).second).members ) {
		double score = distance(q, corpus_.docvec.at(obj), a, corpus_.max_space_distance, corpus_.max_semantic_distance);
		numdist++;
		docscore_queue.add_if_better(obj, score);
	    }

	    if (docscore_queue.size() == k) {
		DocScore cur = docscore_queue.at(0);
		Doc cur_ld = {corpus_.docvec.at(cur.first).first,  corpus_.emb_ld.at(cur.first)};
		U = distance(q_ld, cur_ld, a, corpus_.max_space_distance, corpus_.max_semantic_distance_red);
		for (size_t j=1; j < k; j++) {
		    cur = docscore_queue.at(j);
		    cur_ld = {corpus_.docvec.at(cur.first).first,  corpus_.emb_ld.at(cur.first)};
		    double s = distance(q_ld, cur_ld , a, corpus_.max_space_distance, corpus_.max_semantic_distance_red);
		    if (s > U ) {
			U = s;
		    }
		}
		if ((i+1) <  sorted_index.size()) // We end up accessing all clusters
		    if ( sorted_index.at(i+1).first > U )
			break;
	    }
	}

	while (!docscore_queue.isEmpty()) {
	    results.push_back(docscore_queue.toppop());
	}

	prunes = 0;
    }

} // namespace sstss
