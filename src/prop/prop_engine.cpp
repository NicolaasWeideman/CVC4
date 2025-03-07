/*********************                                                        */
/*! \file prop_engine.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Haniel Barbosa, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the propositional engine of CVC4
 **
 ** Implementation of the propositional engine of CVC4.
 **/

#include "prop/prop_engine.h"

#include <iomanip>
#include <map>
#include <utility>

#include "base/check.h"
#include "base/output.h"
#include "decision/decision_engine.h"
#include "options/base_options.h"
#include "options/decision_options.h"
#include "options/main_options.h"
#include "options/options.h"
#include "options/proof_options.h"
#include "options/smt_options.h"
#include "proof/proof_manager.h"
#include "prop/cnf_stream.h"
#include "prop/minisat/minisat.h"
#include "prop/prop_proof_manager.h"
#include "prop/sat_solver.h"
#include "prop/sat_solver_factory.h"
#include "prop/theory_proxy.h"
#include "smt/smt_statistics_registry.h"
#include "theory/output_channel.h"
#include "theory/theory_engine.h"
#include "util/resource_manager.h"
#include "util/result.h"

namespace CVC5 {
namespace prop {

/** Keeps a boolean flag scoped */
class ScopedBool {

private:

  bool d_original;
  bool& d_reference;

public:

  ScopedBool(bool& reference) :
    d_reference(reference) {
    d_original = reference;
  }

  ~ScopedBool() {
    d_reference = d_original;
  }
};

PropEngine::PropEngine(TheoryEngine* te,
                       context::Context* satContext,
                       context::UserContext* userContext,
                       ResourceManager* rm,
                       OutputManager& outMgr,
                       ProofNodeManager* pnm)
    : d_inCheckSat(false),
      d_theoryEngine(te),
      d_context(satContext),
      d_theoryProxy(nullptr),
      d_satSolver(nullptr),
      d_pnm(pnm),
      d_cnfStream(nullptr),
      d_pfCnfStream(nullptr),
      d_ppm(nullptr),
      d_interrupted(false),
      d_resourceManager(rm),
      d_outMgr(outMgr)
{
  Debug("prop") << "Constructing the PropEngine" << std::endl;

  d_decisionEngine.reset(new DecisionEngine(satContext, userContext, rm));
  d_decisionEngine->init();  // enable appropriate strategies

  d_satSolver = SatSolverFactory::createCDCLTMinisat(smtStatisticsRegistry());

  // CNF stream and theory proxy required pointers to each other, make the
  // theory proxy first
  d_theoryProxy = new TheoryProxy(this,
                                  d_theoryEngine,
                                  d_decisionEngine.get(),
                                  satContext,
                                  userContext,
                                  pnm);
  d_cnfStream = new CnfStream(d_satSolver,
                              d_theoryProxy,
                              userContext,
                              &d_outMgr,
                              rm,
                              FormulaLitPolicy::TRACK);

  // connect theory proxy
  d_theoryProxy->finishInit(d_cnfStream);
  // connect SAT solver
  d_satSolver->initialize(d_context, d_theoryProxy, userContext, pnm);

  d_decisionEngine->setSatSolver(d_satSolver);
  d_decisionEngine->setCnfStream(d_cnfStream);
  if (pnm)
  {
    d_pfCnfStream.reset(new ProofCnfStream(
        userContext,
        *d_cnfStream,
        static_cast<MinisatSatSolver*>(d_satSolver)->getProofManager(),
        pnm));
    d_ppm.reset(
        new PropPfManager(userContext, pnm, d_satSolver, d_pfCnfStream.get()));
  }
  else if (options::unsatCores())
  {
    ProofManager::currentPM()->initCnfProof(d_cnfStream, userContext);
  }
}

void PropEngine::finishInit()
{
  NodeManager* nm = NodeManager::currentNM();
  d_cnfStream->convertAndAssert(nm->mkConst(true), false, false);
  // this is necessary because if True is later asserted to the prop engine the
  // CNF stream will ignore it since the SAT solver already had it registered,
  // thus not having True as an assumption for the SAT proof. To solve this
  // issue we track it directly here
  if (isProofEnabled())
  {
    static_cast<MinisatSatSolver*>(d_satSolver)
        ->getProofManager()
        ->registerSatAssumptions({nm->mkConst(true)});
  }
  d_cnfStream->convertAndAssert(nm->mkConst(false).notNode(), false, false);
}

PropEngine::~PropEngine() {
  Debug("prop") << "Destructing the PropEngine" << std::endl;
  d_decisionEngine->shutdown();
  d_decisionEngine.reset(nullptr);
  delete d_cnfStream;
  delete d_satSolver;
  delete d_theoryProxy;
}

theory::TrustNode PropEngine::preprocess(
    TNode node,
    std::vector<theory::TrustNode>& newLemmas,
    std::vector<Node>& newSkolems)
{
  return d_theoryProxy->preprocess(node, newLemmas, newSkolems);
}

theory::TrustNode PropEngine::removeItes(
    TNode node,
    std::vector<theory::TrustNode>& newLemmas,
    std::vector<Node>& newSkolems)
{
  return d_theoryProxy->removeItes(node, newLemmas, newSkolems);
}

void PropEngine::notifyPreprocessedAssertions(
    const std::vector<Node>& assertions)
{
  // notify the theory proxy of preprocessed assertions
  d_theoryProxy->notifyPreprocessedAssertions(assertions);
}

void PropEngine::assertFormula(TNode node) {
  Assert(!d_inCheckSat) << "Sat solver in solve()!";
  Debug("prop") << "assertFormula(" << node << ")" << std::endl;
  // NOTE: we do not notify the theory proxy here, since we've already
  // notified the theory proxy during notifyPreprocessedAssertions
  assertInternal(node, false, false, true);
}

void PropEngine::assertSkolemDefinition(TNode node, TNode skolem)
{
  Assert(!d_inCheckSat) << "Sat solver in solve()!";
  Debug("prop") << "assertFormula(" << node << ")" << std::endl;
  d_theoryProxy->notifyAssertion(node, skolem);
  assertInternal(node, false, false, true);
}

void PropEngine::assertLemma(theory::TrustNode tlemma, theory::LemmaProperty p)
{
  bool removable = isLemmaPropertyRemovable(p);

  // call preprocessor
  std::vector<theory::TrustNode> ppLemmas;
  std::vector<Node> ppSkolems;
  theory::TrustNode tplemma =
      d_theoryProxy->preprocessLemma(tlemma, ppLemmas, ppSkolems);

  Assert(ppSkolems.size() == ppLemmas.size());

  // do final checks on the lemmas we are about to send
  if (isProofEnabled() && options::proofEagerChecking())
  {
    Assert(tplemma.getGenerator() != nullptr);
    // ensure closed, make the proof node eagerly here to debug
    tplemma.debugCheckClosed("te-proof-debug", "TheoryEngine::lemma");
    for (size_t i = 0, lsize = ppLemmas.size(); i < lsize; ++i)
    {
      Assert(ppLemmas[i].getGenerator() != nullptr);
      ppLemmas[i].debugCheckClosed("te-proof-debug", "TheoryEngine::lemma_new");
    }
  }

  if (Trace.isOn("te-lemma"))
  {
    Trace("te-lemma") << "Lemma, output: " << tplemma.getProven() << std::endl;
    for (size_t i = 0, lsize = ppLemmas.size(); i < lsize; ++i)
    {
      Trace("te-lemma") << "Lemma, new lemma: " << ppLemmas[i].getProven()
                        << " (skolem is " << ppSkolems[i] << ")" << std::endl;
    }
  }

  // now, assert the lemmas
  assertLemmasInternal(tplemma, ppLemmas, ppSkolems, removable);
}

void PropEngine::assertTrustedLemmaInternal(theory::TrustNode trn,
                                            bool removable)
{
  Node node = trn.getNode();
  Debug("prop::lemmas") << "assertLemma(" << node << ")" << std::endl;
  bool negated = trn.getKind() == theory::TrustNodeKind::CONFLICT;
  Assert(!isProofEnabled() || trn.getGenerator() != nullptr);
  assertInternal(trn.getNode(), negated, removable, false, trn.getGenerator());
}

void PropEngine::assertInternal(
    TNode node, bool negated, bool removable, bool input, ProofGenerator* pg)
{
  // Assert as (possibly) removable
  if (isProofEnabled())
  {
    d_pfCnfStream->convertAndAssert(node, negated, removable, pg);
    // if input, register the assertion
    if (input)
    {
      d_ppm->registerAssertion(node);
    }
  }
  else
  {
    d_cnfStream->convertAndAssert(node, removable, negated, input);
  }
}
void PropEngine::assertLemmasInternal(
    theory::TrustNode trn,
    const std::vector<theory::TrustNode>& ppLemmas,
    const std::vector<Node>& ppSkolems,
    bool removable)
{
  if (!trn.isNull())
  {
    assertTrustedLemmaInternal(trn, removable);
  }
  for (const theory::TrustNode& tnl : ppLemmas)
  {
    assertTrustedLemmaInternal(tnl, removable);
  }
  // assert to decision engine
  if (!removable)
  {
    // also add to the decision engine, where notice we don't need proofs
    if (!trn.isNull())
    {
      // notify the theory proxy of the lemma
      d_theoryProxy->notifyAssertion(trn.getProven());
    }
    Assert(ppSkolems.size() == ppLemmas.size());
    for (size_t i = 0, lsize = ppLemmas.size(); i < lsize; ++i)
    {
      Node lem = ppLemmas[i].getProven();
      d_theoryProxy->notifyAssertion(ppLemmas[i].getProven(), ppSkolems[i]);
    }
  }
}

void PropEngine::requirePhase(TNode n, bool phase) {
  Debug("prop") << "requirePhase(" << n << ", " << phase << ")" << std::endl;

  Assert(n.getType().isBoolean());
  SatLiteral lit = d_cnfStream->getLiteral(n);
  d_satSolver->requirePhase(phase ? lit : ~lit);
}

bool PropEngine::isDecision(Node lit) const {
  Assert(isSatLiteral(lit));
  return d_satSolver->isDecision(d_cnfStream->getLiteral(lit).getSatVariable());
}

void PropEngine::printSatisfyingAssignment(){
  const CnfStream::NodeToLiteralMap& transCache =
    d_cnfStream->getTranslationCache();
  Debug("prop-value") << "Literal | Value | Expr" << std::endl
                      << "----------------------------------------"
                      << "-----------------" << std::endl;
  for(CnfStream::NodeToLiteralMap::const_iterator i = transCache.begin(),
      end = transCache.end();
      i != end;
      ++i) {
    std::pair<Node, SatLiteral> curr = *i;
    SatLiteral l = curr.second;
    if(!l.isNegated()) {
      Node n = curr.first;
      SatValue value = d_satSolver->modelValue(l);
      Debug("prop-value") << "'" << l << "' " << value << " " << n << std::endl;
    }
  }
}

Result PropEngine::checkSat() {
  Assert(!d_inCheckSat) << "Sat solver in solve()!";
  Debug("prop") << "PropEngine::checkSat()" << std::endl;

  // Mark that we are in the checkSat
  ScopedBool scopedBool(d_inCheckSat);
  d_inCheckSat = true;

  // TODO This currently ignores conflicts (a dangerous practice).
  d_theoryEngine->presolve();

  if(options::preprocessOnly()) {
    return Result(Result::SAT_UNKNOWN, Result::REQUIRES_FULL_CHECK);
  }

  // Reset the interrupted flag
  d_interrupted = false;

  // Check the problem
  SatValue result = d_satSolver->solve();

  if( result == SAT_VALUE_UNKNOWN ) {

    Result::UnknownExplanation why = Result::INTERRUPTED;
    if (d_resourceManager->outOfTime())
      why = Result::TIMEOUT;
    if (d_resourceManager->outOfResources())
      why = Result::RESOURCEOUT;

    return Result(Result::SAT_UNKNOWN, why);
  }

  if( result == SAT_VALUE_TRUE && Debug.isOn("prop") ) {
    printSatisfyingAssignment();
  }

  Debug("prop") << "PropEngine::checkSat() => " << result << std::endl;
  if(result == SAT_VALUE_TRUE && d_theoryEngine->isIncomplete()) {
    return Result(Result::SAT_UNKNOWN, Result::INCOMPLETE);
  }
  return Result(result == SAT_VALUE_TRUE ? Result::SAT : Result::UNSAT);
}

Node PropEngine::getValue(TNode node) const
{
  Assert(node.getType().isBoolean());
  Assert(d_cnfStream->hasLiteral(node));

  SatLiteral lit = d_cnfStream->getLiteral(node);

  SatValue v = d_satSolver->value(lit);
  if (v == SAT_VALUE_TRUE)
  {
    return NodeManager::currentNM()->mkConst(true);
  }
  else if (v == SAT_VALUE_FALSE)
  {
    return NodeManager::currentNM()->mkConst(false);
  }
  else
  {
    Assert(v == SAT_VALUE_UNKNOWN);
    return Node::null();
  }
}

bool PropEngine::isSatLiteral(TNode node) const
{
  return d_cnfStream->hasLiteral(node);
}

bool PropEngine::hasValue(TNode node, bool& value) const
{
  Assert(node.getType().isBoolean());
  Assert(d_cnfStream->hasLiteral(node)) << node;

  SatLiteral lit = d_cnfStream->getLiteral(node);

  SatValue v = d_satSolver->value(lit);
  if (v == SAT_VALUE_TRUE)
  {
    value = true;
    return true;
  }
  else if (v == SAT_VALUE_FALSE)
  {
    value = false;
    return true;
  }
  else
  {
    Assert(v == SAT_VALUE_UNKNOWN);
    return false;
  }
}

void PropEngine::getBooleanVariables(std::vector<TNode>& outputVariables) const
{
  d_cnfStream->getBooleanVariables(outputVariables);
}

Node PropEngine::ensureLiteral(TNode n)
{
  // must preprocess
  Node preprocessed = getPreprocessedTerm(n);
  Trace("ensureLiteral") << "ensureLiteral preprocessed: " << preprocessed
                         << std::endl;
  if (isProofEnabled())
  {
    d_pfCnfStream->ensureLiteral(preprocessed);
  }
  else
  {
    d_cnfStream->ensureLiteral(preprocessed);
  }
  return preprocessed;
}

Node PropEngine::getPreprocessedTerm(TNode n)
{
  // must preprocess
  std::vector<theory::TrustNode> newLemmas;
  std::vector<Node> newSkolems;
  theory::TrustNode tpn = d_theoryProxy->preprocess(n, newLemmas, newSkolems);
  // send lemmas corresponding to the skolems introduced by preprocessing n
  theory::TrustNode trnNull;
  assertLemmasInternal(trnNull, newLemmas, newSkolems, false);
  return tpn.isNull() ? Node(n) : tpn.getNode();
}

Node PropEngine::getPreprocessedTerm(TNode n,
                                     std::vector<Node>& skAsserts,
                                     std::vector<Node>& sks)
{
  // get the preprocessed form of the term
  Node pn = getPreprocessedTerm(n);
  // initialize the set of skolems and assertions to process
  std::vector<Node> toProcessAsserts;
  std::vector<Node> toProcess;
  d_theoryProxy->getSkolems(pn, toProcessAsserts, toProcess);
  size_t index = 0;
  // until fixed point is reached
  while (index < toProcess.size())
  {
    Node ka = toProcessAsserts[index];
    Node k = toProcess[index];
    index++;
    if (std::find(sks.begin(), sks.end(), k) != sks.end())
    {
      // already added the skolem to the list
      continue;
    }
    // must preprocess lemmas as well
    Node kap = getPreprocessedTerm(ka);
    skAsserts.push_back(kap);
    sks.push_back(k);
    // get the skolems in the preprocessed form of the lemma ka
    d_theoryProxy->getSkolems(kap, toProcessAsserts, toProcess);
  }
  // return the preprocessed term
  return pn;
}

void PropEngine::push()
{
  Assert(!d_inCheckSat) << "Sat solver in solve()!";
  d_satSolver->push();
  Debug("prop") << "push()" << std::endl;
}

void PropEngine::pop()
{
  Assert(!d_inCheckSat) << "Sat solver in solve()!";
  d_satSolver->pop();
  Debug("prop") << "pop()" << std::endl;
}

void PropEngine::resetTrail()
{
  d_satSolver->resetTrail();
  Debug("prop") << "resetTrail()" << std::endl;
}

unsigned PropEngine::getAssertionLevel() const
{
  return d_satSolver->getAssertionLevel();
}

bool PropEngine::isRunning() const { return d_inCheckSat; }
void PropEngine::interrupt()
{
  if (!d_inCheckSat)
  {
    return;
  }

  d_interrupted = true;
  d_satSolver->interrupt();
  Debug("prop") << "interrupt()" << std::endl;
}

void PropEngine::spendResource(ResourceManager::Resource r)
{
  d_resourceManager->spendResource(r);
}

bool PropEngine::properExplanation(TNode node, TNode expl) const
{
  if (!d_cnfStream->hasLiteral(node))
  {
    Trace("properExplanation")
        << "properExplanation(): Failing because node "
        << "being explained doesn't have a SAT literal ?!" << std::endl
        << "properExplanation(): The node is: " << node << std::endl;
    return false;
  }

  SatLiteral nodeLit = d_cnfStream->getLiteral(node);

  for (TNode::kinded_iterator i = expl.begin(kind::AND),
                              i_end = expl.end(kind::AND);
       i != i_end;
       ++i)
  {
    if (!d_cnfStream->hasLiteral(*i))
    {
      Trace("properExplanation")
          << "properExplanation(): Failing because one of explanation "
          << "nodes doesn't have a SAT literal" << std::endl
          << "properExplanation(): The explanation node is: " << *i
          << std::endl;
      return false;
    }

    SatLiteral iLit = d_cnfStream->getLiteral(*i);

    if (iLit == nodeLit)
    {
      Trace("properExplanation")
          << "properExplanation(): Failing because the node" << std::endl
          << "properExplanation(): " << node << std::endl
          << "properExplanation(): cannot be made to explain itself!"
          << std::endl;
      return false;
    }

    if (!d_satSolver->properExplanation(nodeLit, iLit))
    {
      Trace("properExplanation")
          << "properExplanation(): SAT solver told us that node" << std::endl
          << "properExplanation(): " << *i << std::endl
          << "properExplanation(): is not part of a proper explanation node for"
          << std::endl
          << "properExplanation(): " << node << std::endl
          << "properExplanation(): Perhaps it one of the two isn't assigned or "
             "the explanation"
          << std::endl
          << "properExplanation(): node wasn't propagated before the node "
             "being explained"
          << std::endl;
      return false;
    }
  }

  return true;
}

void PropEngine::checkProof(context::CDList<Node>* assertions)
{
  if (!d_pnm)
  {
    return;
  }
  return d_ppm->checkProof(assertions);
}

ProofCnfStream* PropEngine::getProofCnfStream() { return d_pfCnfStream.get(); }

std::shared_ptr<ProofNode> PropEngine::getProof()
{
  if (!d_pnm)
  {
    return nullptr;
  }
  return d_ppm->getProof();
}

bool PropEngine::isProofEnabled() const { return d_pfCnfStream != nullptr; }

}  // namespace prop
}  // namespace CVC5
