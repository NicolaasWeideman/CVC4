/*********************                                                        */
/*! \file rewrite.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Caleb Donovick
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The rewrite preprocessing pass
 **
 ** Calls the rewriter on every assertion
 **/

#include "preprocessing/passes/rewrite.h"

#include "preprocessing/assertion_pipeline.h"
#include "theory/rewriter.h"

namespace CVC5 {
namespace preprocessing {
namespace passes {

using namespace CVC5::theory;

Rewrite::Rewrite(PreprocessingPassContext* preprocContext)
    : PreprocessingPass(preprocContext, "rewrite"){};


PreprocessingPassResult Rewrite::applyInternal(
  AssertionPipeline* assertionsToPreprocess)
{
  for (unsigned i = 0; i < assertionsToPreprocess->size(); ++i) {
    assertionsToPreprocess->replace(i, Rewriter::rewrite((*assertionsToPreprocess)[i]));
  }

  return PreprocessingPassResult::NO_CONFLICT;
}


}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC5
