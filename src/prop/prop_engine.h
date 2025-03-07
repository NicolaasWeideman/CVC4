/*********************                                                        */
/*! \file prop_engine.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The PropEngine (propositional engine); main interface point
 ** between CVC4's SMT infrastructure and the SAT solver
 **
 ** The PropEngine (propositional engine); main interface point
 ** between CVC4's SMT infrastructure and the SAT solver.
 **/

#include "cvc4_private.h"

#ifndef CVC4__PROP_ENGINE_H
#define CVC4__PROP_ENGINE_H

#include "context/cdlist.h"
#include "expr/node.h"
#include "theory/output_channel.h"
#include "theory/trust_node.h"
#include "util/result.h"

namespace CVC5 {

class ResourceManager;
class DecisionEngine;
class OutputManager;
class ProofNodeManager;
class TheoryEngine;

namespace prop {

class CnfStream;
class CDCLTSatSolverInterface;
class ProofCnfStream;
class PropPfManager;
class TheoryProxy;

/**
 * PropEngine is the abstraction of a Sat Solver, providing methods for
 * solving the SAT problem and conversion to CNF (via the CnfStream).
 */
class PropEngine
{
 public:
  /**
   * Create a PropEngine with a particular decision and theory engine.
   */
  PropEngine(TheoryEngine*,
             context::Context* satContext,
             context::UserContext* userContext,
             ResourceManager* rm,
             OutputManager& outMgr,
             ProofNodeManager* pnm);

  /**
   * Destructor.
   */
  ~PropEngine();

  /**
   * Finish initialize. Call this after construction just before we are
   * ready to use this class. Should be called after TheoryEngine::finishInit.
   * This method converts and asserts true and false into the CNF stream.
   */
  void finishInit();

  /**
   * This is called by SmtEngine, at shutdown time, just before
   * destruction.  It is important because there are destruction
   * ordering issues between some parts of the system (notably between
   * PropEngine and Theory).  For now, there's nothing to do here in
   * the PropEngine.
   */
  void shutdown() {}

  /**
   * Preprocess the given node. Return the REWRITE trust node corresponding to
   * rewriting node. New lemmas and skolems are added to ppLemmas and
   * ppSkolems respectively.
   *
   * @param node The assertion to preprocess,
   * @param ppLemmas The lemmas to add to the set of assertions,
   * @param ppSkolems The skolems that newLemmas correspond to,
   * @return The (REWRITE) trust node corresponding to rewritten node via
   * preprocessing.
   */
  theory::TrustNode preprocess(TNode node,
                               std::vector<theory::TrustNode>& ppLemmas,
                               std::vector<Node>& ppSkolems);
  /**
   * Remove term ITEs (and more generally, term formulas) from the given node.
   * Return the REWRITE trust node corresponding to rewriting node. New lemmas
   * and skolems are added to ppLemmas and ppSkolems respectively. This can
   * be seen a subset of the above preprocess method, which also does theory
   * preprocessing and rewriting.
   *
   * @param node The assertion to preprocess,
   * @param ppLemmas The lemmas to add to the set of assertions,
   * @param ppSkolems The skolems that newLemmas correspond to,
   * @return The (REWRITE) trust node corresponding to rewritten node via
   * preprocessing.
   */
  theory::TrustNode removeItes(TNode node,
                               std::vector<theory::TrustNode>& ppLemmas,
                               std::vector<Node>& ppSkolems);
  /**
   * Notify preprocessed assertions. This method is called just before the
   * assertions are asserted to this prop engine. This method notifies the
   * theory engine of the given assertions. Notice this vector includes
   * both the input formulas and the skolem definitions.
   */
  void notifyPreprocessedAssertions(const std::vector<Node>& assertions);

  /**
   * Converts the given formula to CNF and assert the CNF to the SAT solver.
   * The formula is asserted permanently for the current context. Note the
   * formula should correspond to an input formula and not a lemma introduced
   * by term formula removal (which instead should use the interface below).
   * @param node the formula to assert
   */
  void assertFormula(TNode node);
  /**
   * Same as above, but node corresponds to the skolem definition of the given
   * skolem.
   * @param node the formula to assert
   * @param skolem the skolem that this lemma defines.
   *
   * For example, if k is introduced by ITE removal of (ite C x y), then node
   * is the formula (ite C (= k x) (= k y)).  It is important to distinguish
   * these kinds of lemmas from input assertions, as the justification decision
   * heuristic treates them specially.
   */
  void assertSkolemDefinition(TNode node, TNode skolem);

  /**
   * Converts the given formula to CNF and assert the CNF to the SAT solver.
   * The formula can be removed by the SAT solver after backtracking lower
   * than the (SAT and SMT) level at which it was asserted.
   *
   * @param trn the trust node storing the formula to assert
   * @param p the properties of the lemma
   */
  void assertLemma(theory::TrustNode tlemma, theory::LemmaProperty p);

  /**
   * If ever n is decided upon, it must be in the given phase.  This
   * occurs *globally*, i.e., even if the literal is untranslated by
   * user pop and retranslated, it keeps this phase.  The associated
   * variable will _always_ be phase-locked.
   *
   * @param n the node in question; must have an associated SAT literal
   * @param phase the phase to use
   */
  void requirePhase(TNode n, bool phase);

  /**
   * Return whether the given literal is a SAT decision.  Either phase
   * is permitted; that is, if "lit" is a SAT decision, this function
   * returns true for both lit and the negation of lit.
   */
  bool isDecision(Node lit) const;

  /**
   * Checks the current context for satisfiability.
   *
   */
  Result checkSat();

  /**
   * Get the value of a boolean variable.
   *
   * @return mkConst<true>, mkConst<false>, or Node::null() if
   * unassigned.
   */
  Node getValue(TNode node) const;

  /**
   * Return true if node has an associated SAT literal.
   */
  bool isSatLiteral(TNode node) const;

  /**
   * Check if the node has a value and return it if yes.
   */
  bool hasValue(TNode node, bool& value) const;

  /**
   * Returns the Boolean variables known to the SAT solver.
   */
  void getBooleanVariables(std::vector<TNode>& outputVariables) const;

  /**
   * Ensure that the given node will have a designated SAT literal
   * that is definitionally equal to it. Note that theory preprocessing is
   * applied to n. The node returned by this method can be subsequently queried
   * via getSatValue().
   */
  Node ensureLiteral(TNode n);
  /**
   * This returns the theory-preprocessed form of term n. This rewrites and
   * preprocesses n, which notice may involve adding clauses to the SAT solver
   * if preprocessing n involves introducing new skolems.
   */
  Node getPreprocessedTerm(TNode n);
  /**
   * Same as above, but also compute the skolems in n and in the lemmas
   * corresponding to their definition.
   *
   * Note this will include skolems that occur in the definition lemma
   * for all skolems in sks. This is run until a fixed point is reached.
   * For example, if k1 has definition (ite A (= k1 k2) (= k1 x)) where k2 is
   * another skolem introduced by term formula removal, then calling this
   * method on (P k1) will include both k1 and k2 in sks, and their definitions
   * in skAsserts.
   *
   * Notice that this method is not frequently used. It is used for algorithms
   * that explicitly care about knowing which skolems occur in the preprocessed
   * form of a term, recursively.
   */
  Node getPreprocessedTerm(TNode n,
                           std::vector<Node>& skAsserts,
                           std::vector<Node>& sks);

  /**
   * Push the context level.
   */
  void push();

  /**
   * Pop the context level.
   */
  void pop();

  /*
   * Reset the decisions in the DPLL(T) SAT solver at the current assertion
   * level.
   */
  void resetTrail();

  /**
   * Get the assertion level of the SAT solver.
   */
  unsigned getAssertionLevel() const;

  /**
   * Return true if we are currently searching (either in this or
   * another thread).
   */
  bool isRunning() const;

  /**
   * Interrupt a running solver (cause a timeout).
   *
   * Can potentially throw a ModalException.
   */
  void interrupt();

  /**
   * Informs the ResourceManager that a resource has been spent.  If out of
   * resources, can throw an UnsafeInterruptException exception.
   */
  void spendResource(ResourceManager::Resource r);

  /**
   * For debugging.  Return true if "expl" is a well-formed
   * explanation for "node," meaning:
   *
   * 1. expl is either a SAT literal or an AND of SAT literals
   *    currently assigned true;
   * 2. node is assigned true;
   * 3. node does not appear in expl; and
   * 4. node was assigned after all of the literals in expl
   */
  bool properExplanation(TNode node, TNode expl) const;

  /** Retrieve this modules proof CNF stream. */
  ProofCnfStream* getProofCnfStream();

  /** Checks that the proof is closed w.r.t. asserted formulas to this engine as
   * well as to the given assertions. */
  void checkProof(context::CDList<Node>* assertions);

  /**
   * Return the prop engine proof. This should be called only when proofs are
   * enabled. Returns a proof of false whose free assumptions are the
   * preprocessed assertions.
   */
  std::shared_ptr<ProofNode> getProof();

  /** Is proof enabled? */
  bool isProofEnabled() const;
 private:
  /** Dump out the satisfying assignment (after SAT result) */
  void printSatisfyingAssignment();

  /**
   * Converts the given formula to CNF and asserts the CNF to the SAT solver.
   * The formula can be removed by the SAT solver after backtracking lower
   * than the (SAT and SMT) level at which it was asserted.
   *
   * @param trn the trust node storing the formula to assert
   * @param removable whether this lemma can be quietly removed based
   * on an activity heuristic
   */
  void assertTrustedLemmaInternal(theory::TrustNode trn, bool removable);
  /**
   * Assert node as a formula to the CNF stream
   * @param node The formula to assert
   * @param negated Whether to assert the negation of node
   * @param removable Whether the formula is removable
   * @param input Whether the formula came from the input
   * @param pg Pointer to a proof generator that can provide a proof of node
   * (or its negation if negated is true).
   */
  void assertInternal(TNode node,
                      bool negated,
                      bool removable,
                      bool input,
                      ProofGenerator* pg = nullptr);
  /**
   * Assert lemmas internal, where trn is a trust node corresponding to a
   * formula to assert to the CNF stream, ppLemmas and ppSkolems are the
   * skolem definitions and skolems obtained from preprocessing it, and
   * removable is whether the lemma is removable.
   */
  void assertLemmasInternal(theory::TrustNode trn,
                            const std::vector<theory::TrustNode>& ppLemmas,
                            const std::vector<Node>& ppSkolems,
                            bool removable);

  /**
   * Indicates that the SAT solver is currently solving something and we should
   * not mess with it's internal state.
   */
  bool d_inCheckSat;

  /** The theory engine we will be using */
  TheoryEngine* d_theoryEngine;

  /** The decision engine we will be using */
  std::unique_ptr<DecisionEngine> d_decisionEngine;

  /** The context */
  context::Context* d_context;

  /** SAT solver's proxy back to theories; kept around for dtor cleanup */
  TheoryProxy* d_theoryProxy;

  /** The SAT solver proxy */
  CDCLTSatSolverInterface* d_satSolver;

  /** List of all of the assertions that need to be made */
  std::vector<Node> d_assertionList;

  /** A pointer to the proof node maneger to be used by this engine. */
  ProofNodeManager* d_pnm;

  /** The CNF converter in use */
  CnfStream* d_cnfStream;
  /** Proof-producing CNF converter */
  std::unique_ptr<ProofCnfStream> d_pfCnfStream;

  /** The proof manager for prop engine */
  std::unique_ptr<PropPfManager> d_ppm;

  /** Whether we were just interrupted (or not) */
  bool d_interrupted;
  /** Pointer to resource manager for associated SmtEngine */
  ResourceManager* d_resourceManager;

  /** Reference to the output manager of the smt engine */
  OutputManager& d_outMgr;
};

}  // namespace prop
}  // namespace CVC5

#endif /* CVC4__PROP_ENGINE_H */
