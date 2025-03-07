/*********************                                                        */
/*! \file check_models.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility for checking models
 **/

#include "cvc4_private.h"

#ifndef CVC4__SMT__CHECK_MODELS_H
#define CVC4__SMT__CHECK_MODELS_H

#include "context/cdlist.h"
#include "expr/node.h"

namespace CVC5 {
namespace smt {

class Model;
class SmtSolver;

/**
 * This utility is responsible for checking the current model.
 */
class CheckModels
{
 public:
  CheckModels(SmtSolver& s);
  ~CheckModels();
  /**
   * Check model m against the current set of input assertions al.
   *
   * This throws an exception if we fail to verify that m is a proper model
   * given assertion list al based on the model checking policy.
   */
  void checkModel(Model* m, context::CDList<Node>* al, bool hardFailure);

 private:
  /** Reference to the SMT solver */
  SmtSolver& d_smt;
};

}  // namespace smt
}  // namespace CVC5

#endif
