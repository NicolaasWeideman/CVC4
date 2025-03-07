/*********************                                                        */
/*! \file theory_quantifiers.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the theory of quantifiers
 **
 ** Implementation of the theory of quantifiers.
 **/

#include "theory/quantifiers/theory_quantifiers.h"

#include "expr/proof_node_manager.h"
#include "options/quantifiers_options.h"
#include "theory/quantifiers/quantifiers_modules.h"
#include "theory/quantifiers/quantifiers_rewriter.h"
#include "theory/valuation.h"

using namespace CVC5::kind;
using namespace CVC5::context;

namespace CVC5 {
namespace theory {
namespace quantifiers {

TheoryQuantifiers::TheoryQuantifiers(Context* c,
                                     context::UserContext* u,
                                     OutputChannel& out,
                                     Valuation valuation,
                                     const LogicInfo& logicInfo,
                                     ProofNodeManager* pnm)
    : Theory(THEORY_QUANTIFIERS, c, u, out, valuation, logicInfo, pnm),
      d_qstate(c, u, valuation, logicInfo),
      d_qreg(),
      d_treg(d_qstate, d_qreg),
      d_qim(*this, d_qstate, d_qreg, d_treg, pnm),
      d_qengine(nullptr)
{
  out.handleUserAttribute( "fun-def", this );
  out.handleUserAttribute("qid", this);
  out.handleUserAttribute( "quant-inst-max-level", this );
  out.handleUserAttribute( "quant-elim", this );
  out.handleUserAttribute( "quant-elim-partial", this );

  ProofChecker* pc = pnm != nullptr ? pnm->getChecker() : nullptr;
  if (pc != nullptr)
  {
    // add the proof rules
    d_qChecker.registerTo(pc);
  }

  // Finish initializing the term registry by hooking it up to the inference
  // manager. This is required due to a cyclic dependency between the term
  // database and the instantiate module. Term database needs inference manager
  // since it sends out lemmas when term indexing is inconsistent, instantiate
  // needs term database for entailment checks.
  d_treg.finishInit(&d_qim);

  // construct the quantifiers engine
  d_qengine.reset(new QuantifiersEngine(d_qstate, d_qreg, d_treg, d_qim, pnm));

  //!!!!!!!!!!!!!! temporary (project #15)
  d_treg.getModel()->finishInit(d_qengine.get());

  // indicate we are using the quantifiers theory state object
  d_theoryState = &d_qstate;
  // use the inference manager as the official inference manager
  d_inferManager = &d_qim;
  // Set the pointer to the quantifiers engine, which this theory owns. This
  // pointer will be retreived by TheoryEngine and set to all theories
  // post-construction.
  d_quantEngine = d_qengine.get();
}

TheoryQuantifiers::~TheoryQuantifiers() {
}

TheoryRewriter* TheoryQuantifiers::getTheoryRewriter() { return &d_rewriter; }
void TheoryQuantifiers::finishInit()
{
  // quantifiers are not evaluated in getModelValue
  d_valuation.setUnevaluatedKind(EXISTS);
  d_valuation.setUnevaluatedKind(FORALL);
  // witness is used in several instantiation strategies
  d_valuation.setUnevaluatedKind(WITNESS);
}

bool TheoryQuantifiers::needsEqualityEngine(EeSetupInfo& esi)
{
  // use the master equality engine
  esi.d_useMaster = true;
  return true;
}

void TheoryQuantifiers::preRegisterTerm(TNode n)
{
  if (n.getKind() != FORALL)
  {
    return;
  }
  Debug("quantifiers-prereg")
      << "TheoryQuantifiers::preRegisterTerm() " << n << std::endl;
  // Preregister the quantified formula.
  // This initializes the modules used for handling n in this user context.
  getQuantifiersEngine()->preRegisterQuantifier(n);
  Debug("quantifiers-prereg")
      << "TheoryQuantifiers::preRegisterTerm() done " << n << std::endl;
}


void TheoryQuantifiers::presolve() {
  Debug("quantifiers-presolve") << "TheoryQuantifiers::presolve()" << std::endl;
  if( getQuantifiersEngine() ){
    getQuantifiersEngine()->presolve();
  }
}

void TheoryQuantifiers::ppNotifyAssertions(
    const std::vector<Node>& assertions) {
  Trace("quantifiers-presolve")
      << "TheoryQuantifiers::ppNotifyAssertions" << std::endl;
  if (getQuantifiersEngine()) {
    getQuantifiersEngine()->ppNotifyAssertions(assertions);
  }
}

bool TheoryQuantifiers::collectModelValues(TheoryModel* m,
                                           const std::set<Node>& termSet)
{
  for(assertions_iterator i = facts_begin(); i != facts_end(); ++i) {
    if ((*i).d_assertion.getKind() == NOT)
    {
      Debug("quantifiers::collectModelInfo")
          << "got quant FALSE: " << (*i).d_assertion[0] << std::endl;
      if (!m->assertPredicate((*i).d_assertion[0], false))
      {
        return false;
      }
    }
    else
    {
      Debug("quantifiers::collectModelInfo")
          << "got quant TRUE : " << *i << std::endl;
      if (!m->assertPredicate(*i, true))
      {
        return false;
      }
    }
  }
  return true;
}

void TheoryQuantifiers::postCheck(Effort level)
{
  // call the quantifiers engine to check
  getQuantifiersEngine()->check(level);
}

bool TheoryQuantifiers::preNotifyFact(
    TNode atom, bool polarity, TNode fact, bool isPrereg, bool isInternal)
{
  Kind k = atom.getKind();
  if (k == FORALL)
  {
    getQuantifiersEngine()->assertQuantifier(atom, polarity);
  }
  else
  {
    Unhandled() << "Unexpected fact " << fact;
  }
  // don't use equality engine, always return true
  return true;
}

void TheoryQuantifiers::setUserAttribute(const std::string& attr, Node n, std::vector<Node> node_values, std::string str_value){
  QuantAttributes::setUserAttribute( attr, n, node_values, str_value );
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC5
