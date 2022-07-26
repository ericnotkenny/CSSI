/**
 * @file
 * Implementation of CSI variant with preorpocessed member structure
 */
#include <chrono> // for timing
#include "csi2.hpp"
#include "../util/commons.hpp"
#include "../util/constants.hpp"
#include "../util/topkPriorityQueue.hpp"

namespace sstss
{
 
    void CSI2::preprocess(uint32_t m, double a)
    {
	(void) m; // Just to silence "unused parameter"
	(void) a; // Just to silence "unused parameter"
    
	// Inefficient, don't waste time profiling :)
	for (auto h :  corpus_.hybrid_index ) { 
	    std::vector<std::pair<DocId, Doc>> docs_in_cluster;
		for (DocId m :  h.members ) {
		    docs_in_cluster.push_back({m, corpus_.docvec.at(m)});
		}
		clusters.push_back(docs_in_cluster);
	  }

    }


    void CSI2::query(uint32_t k, DocId const& qid, double const& a, ResultVec &results,
		     uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist)
    {
        TopKPriorityQueue docscore_queue(k);
	Doc q = corpus_.docvec.at(qid);
	double U = DBL_MAX;
	prunes = 0;
	prunes2 = 0;

	std::vector < std::pair < double, size_t > > sorted_index; 
	for (size_t i=0; i < corpus_.hybrid_index.size(); i++) {
	    double d =  distance_doc_cluster(q, corpus_.hybrid_index.at(i), a, corpus_.max_space_distance, corpus_.max_semantic_distance);
	    sorted_index.push_back({d, i});
	}
	sort(sorted_index.begin(), sorted_index.end());

	for(size_t i = 0; i < sorted_index.size(); i++) {
	    for ( auto const &obj : clusters.at(sorted_index.at(i).second )) {
		double score = distance(q, obj.second, a, corpus_.max_space_distance, corpus_.max_semantic_distance);
		numdist++;
		docscore_queue.add_if_better(obj.first, score);
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
    }

} // namespace sstss

/*
	    auto start = std::chrono::high_resolution_clock::now();
	    auto end = std::chrono::high_resolution_clock::now();
	    std::cout << "Sort: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()/1000 << std::endl;
*/
