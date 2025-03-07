/*********************                                                        */
/*! \file inst_strategy.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Instantiation engine strategy base class
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__QUANTIFIERS__INST_STRATEGY_H
#define CVC4__THEORY__QUANTIFIERS__INST_STRATEGY_H

#include <vector>
#include "expr/node.h"
#include "options/quantifiers_options.h"
#include "theory/theory.h"

namespace CVC5 {
namespace theory {
namespace quantifiers {

namespace inst {
class TriggerDatabase;
}

class QuantifiersState;
class QuantifiersInferenceManager;
class QuantifiersRegistry;
class TermRegistry;

/** A status response to process */
enum class InstStrategyStatus
{
  // the strategy is not finished
  STATUS_UNFINISHED,
  // the status of the strategy is unknown
  STATUS_UNKNOWN,
};

/**
 * A base class for instantiation strategies within E-matching.
 */
class InstStrategy
{
 public:
  InstStrategy(inst::TriggerDatabase& td,
               QuantifiersState& qs,
               QuantifiersInferenceManager& qim,
               QuantifiersRegistry& qr,
               TermRegistry& tr);
  virtual ~InstStrategy();
  /** presolve */
  virtual void presolve();
  /** reset instantiation */
  virtual void processResetInstantiationRound(Theory::Effort effort) = 0;
  /** process method, returns a status */
  virtual InstStrategyStatus process(Node f, Theory::Effort effort, int e) = 0;
  /** identify */
  virtual std::string identify() const;

 protected:
  /**
   * Get the current user pat mode, which may be interleaved based on counters
   * maintained by the quantifiers state.
   */
  options::UserPatMode getInstUserPatMode() const;
  /** reference to the trigger database */
  inst::TriggerDatabase& d_td;
  /** The quantifiers state object */
  QuantifiersState& d_qstate;
  /** The quantifiers inference manager object */
  QuantifiersInferenceManager& d_qim;
  /** Reference to the quantifiers registry */
  QuantifiersRegistry& d_qreg;
  /** Reference to the term registry */
  TermRegistry& d_treg;
}; /* class InstStrategy */

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__THEORY__QUANTIFIERS__INSTANTIATION_ENGINE_H */
