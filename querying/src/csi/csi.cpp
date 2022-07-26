/**
 * @file
 * Implementation of CSI algorithm.
 */
#include <chrono> // for timing
#include "csi.hpp"
#include "../util/commons.hpp"
#include "../util/constants.hpp"
#include "../util/topkPriorityQueue.hpp"

namespace sstss
{

    void CSI::query(uint32_t k, DocId const& qid, double const& a, ResultVec &results,
		     uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist)
    {
        TopKPriorityQueue docscore_queue(k);
	Doc q = corpus_.docvec.at(qid);
	double U = DBL_MAX;

	std::vector < std::pair < double, size_t > > sorted_index; 
	for (size_t i=0; i < corpus_.hybrid_index.size(); i++) {
	    double d =  distance_doc_cluster(q, corpus_.hybrid_index.at(i), a, corpus_.max_space_distance, corpus_.max_semantic_distance);
	    sorted_index.push_back({d, i});
	}
	sort(sorted_index.begin(), sorted_index.end());

	for(size_t i = 0; i < sorted_index.size(); i++) {
	    for ( DocId obj : corpus_.hybrid_index.at(sorted_index.at(i).second).members ) {
		double score = distance(q, corpus_.docvec.at(obj), a, corpus_.max_space_distance, corpus_.max_semantic_distance);
		numdist++;
		docscore_queue.add_if_better(obj, score);
		if (docscore_queue.size() == k)
		    U = docscore_queue.peek();
	    }
	    if ((i+1) <  sorted_index.size()) // We end up accessing all clusters
		if ( sorted_index.at(i+1).first >= U )
		    break;
	}

	while (!docscore_queue.isEmpty()) {
	    results.push_back(docscore_queue.toppop());
	}

	prunes = 0;
	prunes2 = 0;
    }

} // namespace sstss
