/*********************                                                        */
/*! \file assertion.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Theory assertions.
 **
 ** Theory assertions.
 **/

#include "theory/assertion.h"

namespace CVC5 {
namespace theory {

std::ostream& operator<<(std::ostream& out, const Assertion& a) {
  return out << a.d_assertion;
}

}  // namespace theory
}  // namespace CVC5
