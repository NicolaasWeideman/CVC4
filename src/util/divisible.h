/*********************                                                        */
/*! \file divisible.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_public.h"

#ifndef CVC4__DIVISIBLE_H
#define CVC4__DIVISIBLE_H

#include <iosfwd>
#include <ostream>
#include <stddef.h>

#include "util/integer.h"

namespace CVC5 {

/**
 * The structure representing the divisibility-by-k predicate.
 */
struct Divisible
{
  const Integer k;

  Divisible(const Integer& n);

  bool operator==(const Divisible& d) const {
    return k == d.k;
  }

  bool operator!=(const Divisible& d) const {
    return !(*this == d);
  }
}; /* struct Divisible */

/**
 * Hash function for the Divisible objects.
 */
struct DivisibleHashFunction
{
  size_t operator()(const Divisible& d) const {
    return d.k.hash();
  }
}; /* struct DivisibleHashFunction */

inline std::ostream& operator<<(std::ostream& os, const Divisible& d);
inline std::ostream& operator <<(std::ostream& os, const Divisible& d) {
  return os << "divisible-by-" << d.k;
}

}  // namespace CVC5

#endif /* CVC4__DIVISIBLE_H */
