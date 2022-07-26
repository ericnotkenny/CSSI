/**
 * @file
 * Defining 
 */

#ifndef SSTSS_CSIA2
#define SSTSS_CSIA2

#include "../algorithm/algorithm.hpp"

namespace sstss
{
    class CSIA2 : public Algorithm
    {
    public:
        CSIA2() {} /**< Empty constructor */
        ~CSIA2() {} /**< Empty destructor */

        CSIA2(Corpus const& corpus): Algorithm(corpus) {}

        void query(uint32_t k, DocId const& q, double const& a, ResultVec &results, 
		   uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist) override;
	void preprocess(uint32_t m, double a);

    private:
	double lowerbound (Doc q, HybridCluster h);
 	std::vector<std::vector<std::pair<DocId, Doc>>> clusters;

    };

} // namespace sstss

#endif
