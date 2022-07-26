/**
 * @file
 * Defining 
 */

#ifndef SSTSS_CSI
#define SSTSS_CSI

#include "../algorithm/algorithm.hpp"

namespace sstss
{
    class CSI : public Algorithm
    {
    public:
        CSI() {} /**< Empty constructor */
        ~CSI() {} /**< Empty destructor */

        CSI(Corpus const& corpus): Algorithm(corpus) {}

        void query(uint32_t k, DocId const& q, double const& a, ResultVec &results, 
		   uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist) override;
    private:
	double lowerbound (Doc q, HybridCluster h);
 
    };

} // namespace sstss

#endif
