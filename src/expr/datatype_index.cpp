/*********************                                                        */
/*! \file datatype_index.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A class representing an index to a datatype living in NodeManager.
 **/
#include "expr/datatype_index.h"

#include <sstream>
#include <string>
#include "util/hash.h"
#include "util/integer.h"

using namespace std;

namespace CVC5 {

DatatypeIndexConstant::DatatypeIndexConstant(uint32_t index) : d_index(index) {}
std::ostream& operator<<(std::ostream& out, const DatatypeIndexConstant& dic)
{
  return out << "index_" << dic.getIndex();
}

size_t DatatypeIndexConstantHashFunction::operator()(
    const DatatypeIndexConstant& dic) const
{
  return IntegerHashFunction()(dic.getIndex());
}

}  // namespace CVC5
