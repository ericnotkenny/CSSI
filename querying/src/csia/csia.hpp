/**
 * @file
 * Defining 
 */

#ifndef SSTSS_CSIA
#define SSTSS_CSIA

#include "../algorithm/algorithm.hpp"

namespace sstss
{
    class CSIA : public Algorithm
    {
    public:
        CSIA() {} /**< Empty constructor */
        ~CSIA() {} /**< Empty destructor */

        CSIA(Corpus const& corpus): Algorithm(corpus) {}

        void query(uint32_t k, DocId const& q, double const& a, ResultVec &results, 
		   uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist) override;
    private:
	double lowerbound (Doc q, HybridCluster h);
 
    };

} // namespace sstss

#endif
