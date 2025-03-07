/*********************                                                        */
/*! \file miplib_trick.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner, Morgan Deters, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The MIPLIB trick preprocessing pass
 **
 **/

#include "cvc4_private.h"

#ifndef CVC4__PREPROCESSING__PASSES__MIPLIB_TRICK_H
#define CVC4__PREPROCESSING__PASSES__MIPLIB_TRICK_H

#include "expr/node.h"
#include "preprocessing/preprocessing_pass.h"

namespace CVC5 {
namespace preprocessing {
namespace passes {

class MipLibTrick : public PreprocessingPass, public NodeManagerListener
{
 public:
  MipLibTrick(PreprocessingPassContext* preprocContext);
  ~MipLibTrick();

  // NodeManagerListener callbacks to collect d_boolVars.
  void nmNotifyNewVar(TNode n) override;
  void nmNotifyNewSkolem(TNode n,
                         const std::string& comment,
                         uint32_t flags) override;

 protected:
  PreprocessingPassResult applyInternal(
      AssertionPipeline* assertionsToPreprocess) override;

 private:
  struct Statistics
  {
    /** number of assertions removed by miplib pass */
    IntStat d_numMiplibAssertionsRemoved;
    Statistics();
    ~Statistics();
  };

  Statistics d_statistics;

  std::vector<Node> d_boolVars;
};

}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC5

#endif /* CVC4__PREPROCESSING__PASSES__MIPLIB_TRICK_H */
