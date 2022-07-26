/**
 * @file
 * Defining 
 */

#ifndef SSTSS_CSI2
#define SSTSS_CSI2

#include "../algorithm/algorithm.hpp"

namespace sstss
{
    class CSI2 : public Algorithm
    {
    public:
        CSI2() {} /**< Empty constructor */
        ~CSI2() {} /**< Empty destructor */

        CSI2(Corpus const& corpus): Algorithm(corpus) {}

        void query(uint32_t k, DocId const& q, double const& a, ResultVec &results, 
		   uint32_t &prunes, uint32_t &prunes2, uint32_t &numdist) override;
	void preprocess(uint32_t m, double a);
    private:
	double lowerbound (Doc q, HybridCluster h);
	std::vector<std::vector<std::pair<DocId, Doc>>> clusters;
    };

} // namespace sstss

#endif
