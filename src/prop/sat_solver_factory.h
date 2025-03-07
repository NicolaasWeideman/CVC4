/*********************                                                        */
/*! \file sat_solver_factory.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner, Liana Hadarean, Aina Niemetz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief SAT Solver.
 **
 ** SAT Solver creation facility
 **/

#include "cvc4_private.h"

#ifndef CVC4__PROP__SAT_SOLVER_FACTORY_H
#define CVC4__PROP__SAT_SOLVER_FACTORY_H

#include <string>
#include <vector>

#include "context/context.h"
#include "prop/minisat/minisat.h"
#include "prop/sat_solver.h"
#include "util/statistics_registry.h"

namespace CVC5 {
namespace prop {

class SatSolverFactory
{
 public:
  static BVSatSolverInterface* createMinisat(context::Context* mainSatContext,
                                             StatisticsRegistry* registry,
                                             const std::string& name = "");

  static MinisatSatSolver* createCDCLTMinisat(StatisticsRegistry* registry);

  static SatSolver* createCryptoMinisat(StatisticsRegistry* registry,
                                        const std::string& name = "");

  static SatSolver* createCadical(StatisticsRegistry* registry,
                                  const std::string& name = "");

  static SatSolver* createKissat(StatisticsRegistry* registry,
                                 const std::string& name = "");
}; /* class SatSolverFactory */

}  // namespace prop
}  // namespace CVC5

#endif  // CVC4__PROP__SAT_SOLVER_FACTORY_H
