/*********************                                                        */
/*! \file smt_engine_subsolver.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utilities for initializing subsolvers (copies of SmtEngine) during
 ** solving.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__SMT_ENGINE_SUBSOLVER_H
#define CVC4__THEORY__SMT_ENGINE_SUBSOLVER_H

#include <memory>
#include <vector>

#include "expr/node.h"
#include "smt/smt_engine.h"

namespace CVC5 {
namespace theory {

/**
 * This function initializes the smt engine smte to check the satisfiability
 * of the argument "query". It takes the logic and options of the current
 * SMT engine in scope.
 *
 * Notice this method intentionally does not fully initialize smte. This means
 * that the options of smte can still be modified after it is returned by
 * this method.
 *
 * Notice that some aspects of subsolvers are not incoporated by this call.
 * For example, the type of separation logic heaps is not set on smte, even
 * if the current SMT engine has declared a separation logic heap.
 *
 * @param smte The smt engine pointer to initialize
 * @param needsTimeout Whether we would like to set a timeout
 * @param timeout The timeout (in milliseconds)
 */
void initializeSubsolver(std::unique_ptr<SmtEngine>& smte,
                         bool needsTimeout = false,
                         unsigned long timeout = 0);

/**
 * This returns the result of checking the satisfiability of formula query.
 *
 * If necessary, smte is initialized to the SMT engine that checked its
 * satisfiability.
 */
Result checkWithSubsolver(std::unique_ptr<SmtEngine>& smte,
                          Node query,
                          bool needsTimeout = false,
                          unsigned long timeout = 0);

/**
 * This returns the result of checking the satisfiability of formula query.
 *
 * In contrast to above, this is used if the user of this method is not
 * concerned with the state of the SMT engine after the check.
 *
 * @param query The query to check
 * @param needsTimeout Whether we would like to set a timeout
 * @param timeout The timeout (in milliseconds)
 */
Result checkWithSubsolver(Node query,
                          bool needsTimeout = false,
                          unsigned long timeout = 0);

/**
 * This returns the result of checking the satisfiability of formula query.
 * Additionally, if the query is satisfiable, it adds the model values for vars
 * into modelVars.
 *
 * @param query The query to check
 * @param vars The variables we are interesting in getting a model for.
 * @param modelVals A vector storing the model values of variables in vars.
 * @param needsTimeout Whether we would like to set a timeout
 * @param timeout The timeout (in milliseconds)
 */
Result checkWithSubsolver(Node query,
                          const std::vector<Node>& vars,
                          std::vector<Node>& modelVals,
                          bool needsTimeout = false,
                          unsigned long timeout = 0);

}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__THEORY__SMT_ENGINE_SUBSOLVER_H */
