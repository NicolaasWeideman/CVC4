/*********************                                                        */
/*! \file type_enumerator.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Tim King, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief An enumerator for Booleans
 **
 ** An enumerator for Booleans.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__BOOLEANS__TYPE_ENUMERATOR_H
#define CVC4__THEORY__BOOLEANS__TYPE_ENUMERATOR_H

#include "theory/type_enumerator.h"
#include "expr/type_node.h"
#include "expr/kind.h"

namespace CVC5 {
namespace theory {
namespace booleans {

class BooleanEnumerator : public TypeEnumeratorBase<BooleanEnumerator> {
  enum { FALSE, TRUE, DONE } d_value;

 public:
  BooleanEnumerator(TypeNode type, TypeEnumeratorProperties* tep = nullptr)
      : TypeEnumeratorBase<BooleanEnumerator>(type), d_value(FALSE)
  {
    Assert(type.getKind() == kind::TYPE_CONSTANT
           && type.getConst<TypeConstant>() == BOOLEAN_TYPE);
  }

  Node operator*() override {
    switch(d_value) {
    case FALSE:
      return NodeManager::currentNM()->mkConst(false);
    case TRUE:
      return NodeManager::currentNM()->mkConst(true);
    default:
      throw NoMoreValuesException(getType());
    }
  }

  BooleanEnumerator& operator++() override
  {
    // sequence is FALSE, TRUE
    if(d_value == FALSE) {
      d_value = TRUE;
    } else {
      d_value = DONE;
    }
    return *this;
  }

  bool isFinished() override { return d_value == DONE; }
};/* class BooleanEnumerator */

}  // namespace booleans
}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__THEORY__BOOLEANS__TYPE_ENUMERATOR_H */
