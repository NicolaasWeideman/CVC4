/*********************                                                        */
/*! \file theory_fp.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Martin Brain, Aina Niemetz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__FP__THEORY_FP_H
#define CVC4__THEORY__FP__THEORY_FP_H

#include <string>
#include <utility>

#include "context/cdhashset.h"
#include "context/cdo.h"
#include "theory/fp/theory_fp_rewriter.h"
#include "theory/theory.h"
#include "theory/theory_inference_manager.h"
#include "theory/theory_state.h"
#include "theory/uf/equality_engine.h"

namespace CVC5 {
namespace theory {
namespace fp {

class FpConverter;

class TheoryFp : public Theory
{
 public:
  /** Constructs a new instance of TheoryFp w.r.t. the provided contexts. */
  TheoryFp(context::Context* c,
           context::UserContext* u,
           OutputChannel& out,
           Valuation valuation,
           const LogicInfo& logicInfo,
           ProofNodeManager* pnm = nullptr);

  //--------------------------------- initialization
  /** Get the official theory rewriter of this theory. */
  TheoryRewriter* getTheoryRewriter() override;
  /**
   * Returns true if we need an equality engine. If so, we initialize the
   * information regarding how it should be setup. For details, see the
   * documentation in Theory::needsEqualityEngine.
   */
  bool needsEqualityEngine(EeSetupInfo& esi) override;
  /** Finish initialization. */
  void finishInit() override;
  //--------------------------------- end initialization

  TrustNode expandDefinition(Node node) override;

  void preRegisterTerm(TNode node) override;

  TrustNode ppRewrite(TNode node, std::vector<SkolemLemma>& lems) override;

  //--------------------------------- standard check
  /** Do we need a check call at last call effort? */
  bool needsCheckLastEffort() override;
  /** Post-check, called after the fact queue of the theory is processed. */
  void postCheck(Effort level) override;
  /** Pre-notify fact, return true if processed. */
  bool preNotifyFact(TNode atom,
                     bool pol,
                     TNode fact,
                     bool isPrereg,
                     bool isInternal) override;
  //--------------------------------- end standard check

  Node getModelValue(TNode var) override;
  bool collectModelInfo(TheoryModel* m,
                        const std::set<Node>& relevantTerms) override;
  /**
   * Collect model values in m based on the relevant terms given by
   * relevantTerms.
   */
  bool collectModelValues(TheoryModel* m,
                          const std::set<Node>& relevantTerms) override;

  std::string identify() const override { return "THEORY_FP"; }

  TrustNode explain(TNode n) override;

 protected:
  using PairTypeNodeHashFunction = PairHashFunction<TypeNode,
                                                    TypeNode,
                                                    TypeNodeHashFunction,
                                                    TypeNodeHashFunction>;
  /** Uninterpreted functions for undefined cases of non-total operators. */
  using ComparisonUFMap =
      context::CDHashMap<TypeNode, Node, TypeNodeHashFunction>;
  /** Uninterpreted functions for lazy handling of conversions. */
  using ConversionUFMap = context::
      CDHashMap<std::pair<TypeNode, TypeNode>, Node, PairTypeNodeHashFunction>;
  using ConversionAbstractionMap = ComparisonUFMap;
  using AbstractionMap = context::CDHashMap<Node, Node, NodeHashFunction>;

  /** Equality engine. */
  class NotifyClass : public eq::EqualityEngineNotify {
   protected:
    TheoryFp& d_theorySolver;

   public:
    NotifyClass(TheoryFp& solver) : d_theorySolver(solver) {}
    bool eqNotifyTriggerPredicate(TNode predicate, bool value) override;
    bool eqNotifyTriggerTermEquality(TheoryId tag,
                                     TNode t1,
                                     TNode t2,
                                     bool value) override;
    void eqNotifyConstantTermMerge(TNode t1, TNode t2) override;
    void eqNotifyNewClass(TNode t) override {}
    void eqNotifyMerge(TNode t1, TNode t2) override {}
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) override {}
  };
  friend NotifyClass;

  NotifyClass d_notification;

  /** General utility. */
  void registerTerm(TNode node);
  bool isRegistered(TNode node);

  context::CDHashSet<Node, NodeHashFunction> d_registeredTerms;

  /** The word-blaster. Translates FP -> BV. */
  std::unique_ptr<FpConverter> d_conv;

  bool d_expansionRequested;

  void convertAndEquateTerm(TNode node);

  /** Interaction with the rest of the solver **/
  void handleLemma(Node node, InferenceId id = InferenceId::UNKNOWN);
  /**
   * Called when literal node is inferred by the equality engine. This
   * propagates node on the output channel.
   */
  bool propagateLit(TNode node);
  /**
   * Called when two constants t1 and t2 merge in the equality engine. This
   * sends a conflict on the output channel.
   */
  void conflictEqConstantMerge(TNode t1, TNode t2);

  bool refineAbstraction(TheoryModel* m, TNode abstract, TNode concrete);

  Node minUF(Node);
  Node maxUF(Node);

  Node toUBVUF(Node);
  Node toSBVUF(Node);

  Node toRealUF(Node);

  Node abstractRealToFloat(Node);
  Node abstractFloatToReal(Node);

 private:

  ComparisonUFMap d_minMap;
  ComparisonUFMap d_maxMap;
  ConversionUFMap d_toUBVMap;
  ConversionUFMap d_toSBVMap;
  ComparisonUFMap d_toRealMap;
  ConversionAbstractionMap d_realToFloatMap;
  ConversionAbstractionMap d_floatToRealMap;
  AbstractionMap d_abstractionMap;  // abstract -> original

  /** The theory rewriter for this theory. */
  TheoryFpRewriter d_rewriter;
  /** A (default) theory state object */
  TheoryState d_state;
  /** A (default) inference manager */
  TheoryInferenceManager d_im;
}; /* class TheoryFp */

}  // namespace fp
}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__THEORY__FP__THEORY_FP_H */
