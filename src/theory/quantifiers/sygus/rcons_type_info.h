/*********************                                                        */
/*! \file rcons_type_info.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Abdalrhman Mohamed
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief utility class for Sygus Reconstruct module
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__QUANTIFIERS__RCONS_TYPE_INFO_H
#define CVC4__THEORY__QUANTIFIERS__RCONS_TYPE_INFO_H

#include "theory/quantifiers/sygus/sygus_enumerator.h"

namespace CVC5 {
namespace theory {
namespace quantifiers {

/**
 * A utility class for Sygus Reconstruct datatype types (grammar non-terminals).
 * This class is mainly responsible for enumerating sygus datatype type terms
 * and building sets of equivalent builtin terms for the rcons algorithm.
 */
class RConsTypeInfo
{
 public:
  /**
   * Initialize a sygus enumerator and a candidate rewrite database for this
   * class' sygus datatype type.
   *
   * @param tds Database for sygus terms
   * @param s Statistics managed for the synth engine
   * @param stn The sygus datatype type encoding the syntax restrictions
   * @param builtinVars A list containing the builtin analog of sygus variable
   *                    list for the sygus datatype type
   */
  void initialize(TermDbSygus* tds,
                  SygusStatistics& s,
                  TypeNode stn,
                  const std::vector<Node>& builtinVars);

  /**
   * Returns the next enumerated term for the given sygus datatype type.
   *
   * @return The enumerated sygus term
   */
  Node nextEnum();

  /**
   * Add a pure term to the candidate rewrite database.
   *
   * @param n The term to add to the database
   * @return A previous term `eq_n` added to this class, such that `n` is
   * equivalent to `eq_n`. If no previous term equivalent to `n` exists then the
   * result is `n` itself
   */
  Node addTerm(Node n);

  /**
   * Set the obligation responsible for solving the given builtin term.
   *
   * @param builtin The builtin term
   * @param ob The corresponding obligation
   */
  void setBuiltinToOb(Node builtin, Node ob);

  /**
   * Return the obligation responsible for solving the given builtin term.
   *
   * @param builtin The builtin term
   * @return The corresponding obligation
   */
  Node builtinToOb(Node builtin);

 private:
  /** Sygus terms enumerator for this class' Sygus datatype type */
  std::unique_ptr<SygusEnumerator> d_enumerator;
  /** Candidate rewrite database for this class' sygus datatype type */
  std::unique_ptr<CandidateRewriteDatabase> d_crd;
  /** Sygus sampler needed for initializing the candidate rewrite database */
  SygusSampler d_sygusSampler;
  /** A map from a builtin term to its obligation.
   *
   * Each sygus datatype type has its own version of this map because it is
   * possible to have multiple obligations to reconstruct the same builtin term
   * from different sygus datatype types.
   */
  std::unordered_map<Node, Node, NodeHashFunction> d_ob;
};

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC5

#endif  // CVC4__THEORY__QUANTIFIERS__RCONS_TYPE_INFO_H
