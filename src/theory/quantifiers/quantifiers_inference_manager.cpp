/*********************                                                        */
/*! \file quantifiers_inference_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility for quantifiers inference manager
 **/

#include "theory/quantifiers/quantifiers_inference_manager.h"

#include "theory/quantifiers/instantiate.h"
#include "theory/quantifiers/skolemize.h"

namespace CVC5 {
namespace theory {
namespace quantifiers {

QuantifiersInferenceManager::QuantifiersInferenceManager(
    Theory& t,
    QuantifiersState& state,
    QuantifiersRegistry& qr,
    TermRegistry& tr,
    ProofNodeManager* pnm)
    : InferenceManagerBuffered(t, state, pnm, "theory::quantifiers"),
      d_instantiate(new Instantiate(state, *this, qr, tr, pnm)),
      d_skolemize(new Skolemize(state, pnm))
{
}

QuantifiersInferenceManager::~QuantifiersInferenceManager() {}

Instantiate* QuantifiersInferenceManager::getInstantiate()
{
  return d_instantiate.get();
}

Skolemize* QuantifiersInferenceManager::getSkolemize()
{
  return d_skolemize.get();
}

void QuantifiersInferenceManager::doPending()
{
  doPendingLemmas();
  doPendingPhaseRequirements();
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC5
