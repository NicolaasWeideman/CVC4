/*********************                                                        */
/*! \file datatype_index.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King, Ken Matsui
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A class representing an index to a datatype living in NodeManager.
 **/

#include "cvc4_public.h"

#ifndef CVC4__DATATYPE_INDEX_H
#define CVC4__DATATYPE_INDEX_H

#include <iosfwd>

namespace CVC5 {

/* stores an index to Datatype residing in NodeManager */
class DatatypeIndexConstant
{
 public:
  DatatypeIndexConstant(unsigned index);

  unsigned getIndex() const { return d_index; }
  bool operator==(const DatatypeIndexConstant& uc) const
  {
    return d_index == uc.d_index;
  }
  bool operator!=(const DatatypeIndexConstant& uc) const
  {
    return !(*this == uc);
  }
  bool operator<(const DatatypeIndexConstant& uc) const
  {
    return d_index < uc.d_index;
  }
  bool operator<=(const DatatypeIndexConstant& uc) const
  {
    return d_index <= uc.d_index;
  }
  bool operator>(const DatatypeIndexConstant& uc) const
  {
    return !(*this <= uc);
  }
  bool operator>=(const DatatypeIndexConstant& uc) const
  {
    return !(*this < uc);
  }

 private:
  const unsigned d_index;
}; /* class DatatypeIndexConstant */

std::ostream& operator<<(std::ostream& out, const DatatypeIndexConstant& dic);

struct DatatypeIndexConstantHashFunction
{
  size_t operator()(const DatatypeIndexConstant& dic) const;
}; /* struct DatatypeIndexConstantHashFunction */

}  // namespace CVC5

#endif /* CVC4__DATATYPE_H */
