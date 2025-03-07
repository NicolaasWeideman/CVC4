/*********************                                                        */
/*! \file strings_fmf.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Andres Noetzli, Tianyi Liang
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A finite model finding decision strategy for strings.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__STRINGS_FMF_H
#define CVC4__THEORY__STRINGS__STRINGS_FMF_H

#include "context/cdhashset.h"
#include "context/cdo.h"
#include "context/context.h"
#include "expr/node.h"
#include "theory/decision_strategy.h"
#include "theory/strings/term_registry.h"
#include "theory/valuation.h"

namespace CVC5 {
namespace theory {
namespace strings {

/** Strings finite model finding
 *
 * This class manages the creation of a decision strategy that bounds the
 * sum of lengths of terms of type string.
 */
class StringsFmf
{
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;

 public:
  StringsFmf(context::Context* c,
             context::UserContext* u,
             Valuation valuation,
             TermRegistry& tr);
  ~StringsFmf();
  /** presolve
   *
   * This initializes a (new copy) of the decision strategy d_sslds.
   */
  void presolve();
  /**
   * Get the decision strategy, valid after a call to presolve in the duration
   * of a check-sat call.
   */
  DecisionStrategy* getDecisionStrategy() const;

 private:
  /** String sum of lengths decision strategy
   *
   * This decision strategy enforces that len(x_1) + ... + len(x_k) <= n
   * for a minimal natural number n, where x_1, ..., x_n is the list of
   * input variables of the problem of type String.
   *
   * This decision strategy is enabled by option::stringsFmf().
   */
  class StringSumLengthDecisionStrategy : public DecisionStrategyFmf
  {
   public:
    StringSumLengthDecisionStrategy(context::Context* c,
                                    context::UserContext* u,
                                    Valuation valuation);
    /** make literal */
    Node mkLiteral(unsigned i) override;
    /** identify */
    std::string identify() const override;
    /** is initialized */
    bool isInitialized();
    /** initialize */
    void initialize(const std::vector<Node>& vars);

    /*
     * Do not hide the zero-argument version of initialize() inherited from the
     * base class
     */
    using DecisionStrategyFmf::initialize;

   private:
    /**
     * User-context-dependent node corresponding to the sum of the lengths of
     * input variables of type string
     */
    context::CDO<Node> d_inputVarLsum;
  };
  /** an instance of the above class */
  std::unique_ptr<StringSumLengthDecisionStrategy> d_sslds;
  /** The SAT search context for the theory of strings. */
  context::Context* d_satContext;
  /** The user level assertion context for the theory of strings. */
  context::UserContext* d_userContext;
  /** The valuation object of theory of strings */
  Valuation d_valuation;
  /** The term registry of theory of strings */
  TermRegistry& d_termReg;
};

}  // namespace strings
}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__THEORY__STRINGS__STRINGS_FMF_H */
