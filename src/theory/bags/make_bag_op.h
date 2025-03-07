/*********************                                                        */
/*! \file make_bag_op.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mudathir Mohamed
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief a class for MK_BAG operator
 **/

#include "cvc4_public.h"

#ifndef CVC4__MAKE_BAG_OP_H
#define CVC4__MAKE_BAG_OP_H

#include <memory>

namespace CVC5 {

class TypeNode;

/**
 * The class is an operator for kind MK_BAG used to construct bags.
 * It specifies the type of the element especially when it is a constant.
 * e.g. the type of rational 1 is Int, however
 * (mkBag (mkBag_op Real) 1) is of type (Bag Real), not (Bag Int).
 * Note that the type passed to the constructor is the element's type, not the
 * bag type.
 */
class MakeBagOp
{
 public:
  explicit MakeBagOp(const TypeNode& elementType);
  MakeBagOp(const MakeBagOp& op);

  /** return the type of the current object */
  const TypeNode& getType() const;

  bool operator==(const MakeBagOp& op) const;

 private:
  /** a pointer to the type of the bag element */
  std::unique_ptr<TypeNode> d_type;
}; /* class MakeBagOp */

std::ostream& operator<<(std::ostream& out, const MakeBagOp& op);

/**
 * Hash function for the MakeBagOpHashFunction objects.
 */
struct MakeBagOpHashFunction
{
  size_t operator()(const MakeBagOp& op) const;
}; /* struct MakeBagOpHashFunction */

}  // namespace CVC5

#endif /* CVC4__MAKE_BAG_OP_H */
