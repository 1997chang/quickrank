/*
 * dcg.h
 *
 *  Created on: Oct 3, 2014
 *      Author: claudio
 */

#ifndef QUICKRANK_METRIC_IR_DCG_H_
#define QUICKRANK_METRIC_IR_DCG_H_

#include "types.h"
#include "metric.h"

namespace quickrank {
namespace metric {
namespace ir {

/**
 * This class implements the Discounted cumulative Gain DCG\@K measure.
 *
 * DCG is measured as: \f$ DCG_k = \sum_{i=1}^k \frac{2^{l_i}-1}{\log_2 (i+1)}\f$,
 * where \f$l_i\f$ is the relevance label of the i-th document.
 */
class Dcg : public Metric {
 public:
  explicit Dcg(int k = NO_CUTOFF)
      : Metric(k) {
  }
  virtual ~Dcg() {
  }
  ;

  virtual MetricScore evaluate_result_list(
      const quickrank::data::QueryResults* rl, const Score* scores) const;

  virtual std::unique_ptr<Jacobian> jacobian(std::shared_ptr<data::RankedResults> ranked) const;

 protected:
  /// Computes the DCG\@K of a given list of labels.
  /// \param rl The given results list. Only labels are actually used.
  /// \return DCG\@K for computed on the given labels.
  MetricScore compute_dcg(const quickrank::data::QueryResults* rl) const;

 private:
  friend std::ostream& operator<<(std::ostream& os, const Dcg& ndcg) {
    return ndcg.put(os);
  }
  virtual std::ostream& put(std::ostream& os) const;

};

}  // namespace ir
}  // namespace metric
}  // namespace quickrank

#endif // QUICKRANK_DCG_H_
