/*********************                                                        */
/*! \file sygus_stats.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A shared statistics class for sygus.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__QUANTIFIERS__SYGUS_STATS_H
#define CVC4__THEORY__QUANTIFIERS__SYGUS_STATS_H

#include "util/statistics_registry.h"

namespace CVC5 {
namespace theory {
namespace quantifiers {

/**
 * All statistics managed for the synth engine.
 */
class SygusStatistics
{
 public:
  SygusStatistics();
  ~SygusStatistics();
  /** Number of counterexample lemmas */
  IntStat d_cegqi_lemmas_ce;
  /** Number of refinement lemmas */
  IntStat d_cegqi_lemmas_refine;
  /** Number of single invocation lemmas */
  IntStat d_cegqi_si_lemmas;
  /** Number of solutions printed (could be >1 for --sygus-stream) */
  IntStat d_solutions;
  /** Number of solutions filtered */
  IntStat d_filtered_solutions;
  /** Number of candidate rewrites printed (for --sygus-rr) */
  IntStat d_candidate_rewrites_print;
  /** Number of terms checked for rewrite-based symmetry in fast enumerators */
  IntStat d_enumTermsRewrite;
  /** Number of terms checked for example-based symmetry in fast enumerators */
  IntStat d_enumTermsExampleEval;
  /** Number of non-redundant terms generated by fast enumerators */
  IntStat d_enumTerms;
};

}  // namespace quantifiers
}  // namespace theory
} // namespace CVC5

#endif
