/*********************                                                        */
/*! \file theory_preprocess.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Mathias Preiner, Andres Noetzli
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The TheoryPreprocess preprocessing pass
 **
 ** Calls Theory::preprocess(...) on every assertion of the formula.
 **/

#include "preprocessing/passes/theory_preprocess.h"

#include "options/smt_options.h"
#include "preprocessing/assertion_pipeline.h"
#include "preprocessing/preprocessing_pass_context.h"
#include "proof/proof_manager.h"
#include "prop/prop_engine.h"
#include "theory/rewriter.h"
#include "theory/theory_engine.h"

namespace CVC5 {
namespace preprocessing {
namespace passes {

using namespace CVC5::theory;

TheoryPreprocess::TheoryPreprocess(PreprocessingPassContext* preprocContext)
    : PreprocessingPass(preprocContext, "theory-preprocess"){};

PreprocessingPassResult TheoryPreprocess::applyInternal(
    AssertionPipeline* assertions)
{
  d_preprocContext->spendResource(ResourceManager::Resource::PreprocessStep);

  IteSkolemMap& imap = assertions->getIteSkolemMap();
  prop::PropEngine* propEngine = d_preprocContext->getPropEngine();
  // Remove all of the ITE occurrences and normalize
  for (unsigned i = 0, size = assertions->size(); i < size; ++i)
  {
    Node assertion = (*assertions)[i];
    std::vector<theory::TrustNode> newAsserts;
    std::vector<Node> newSkolems;
    TrustNode trn = propEngine->preprocess(assertion, newAsserts, newSkolems);
    if (!trn.isNull())
    {
      // process
      assertions->replaceTrusted(i, trn);
    }
    Assert(newSkolems.size() == newAsserts.size());
    for (unsigned j = 0, nnasserts = newAsserts.size(); j < nnasserts; j++)
    {
      imap[assertions->size()] = newSkolems[j];
      assertions->pushBackTrusted(newAsserts[j]);
      // new assertions have a dependence on the node (old pf architecture)
      if (options::unsatCores())
      {
        ProofManager::currentPM()->addDependence(newAsserts[j].getProven(),
                                                 assertion);
      }
    }
  }

  return PreprocessingPassResult::NO_CONFLICT;
}


}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC5
