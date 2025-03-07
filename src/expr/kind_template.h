/*********************                                                        */
/*! \file kind_template.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli, Morgan Deters, Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Template for the Node kind header
 **
 ** Template for the Node kind header.
 **/

#include "cvc4_public.h"

#ifndef CVC4__KIND_H
#define CVC4__KIND_H

#include <iosfwd>

#include "base/exception.h"
#include "theory/theory_id.h"

namespace CVC5 {
namespace kind {

enum Kind_t
{
  UNDEFINED_KIND = -1,    /**< undefined */
  NULL_EXPR,              /**< Null kind */
  ${kind_decls} LAST_KIND /**< marks the upper-bound of this enumeration */

}; /* enum Kind_t */

}  // namespace kind

// import Kind into the "CVC4" namespace but keep the individual kind
// constants under kind::
typedef ::CVC5::kind::Kind_t Kind;

namespace kind {

/**
 * Converts an kind to a string. Note: This function is also used in
 * `safe_print()`. Changing this functions name or signature will result in
 * `safe_print()` printing "<unsupported>" instead of the proper strings for
 * the enum values.
 *
 * @param k The kind
 * @return The name of the kind
 */
const char* toString(CVC5::Kind k);

/**
 * Writes a kind name to a stream.
 *
 * @param out The stream to write to
 * @param k The kind to write to the stream
 * @return The stream
 */
std::ostream& operator<<(std::ostream&, CVC5::Kind);

/** Returns true if the given kind is associative. This is used by ExprManager to
 * decide whether it's safe to modify big expressions by changing the grouping of
 * the arguments. */
/* TODO: This could be generated. */
bool isAssociative(::CVC5::Kind k);
std::string kindToString(::CVC5::Kind k);

struct KindHashFunction {
  inline size_t operator()(::CVC5::Kind k) const { return k; }
};/* struct KindHashFunction */

}  // namespace kind

/**
 * The enumeration for the built-in atomic types.
 */
enum TypeConstant
{
  ${type_constant_list} LAST_TYPE
}; /* enum TypeConstant */

/**
 * We hash the constants with their values.
 */
struct TypeConstantHashFunction {
  inline size_t operator()(TypeConstant tc) const {
    return tc;
  }
};/* struct TypeConstantHashFunction */

std::ostream& operator<<(std::ostream& out, TypeConstant typeConstant);

namespace theory {

::CVC5::theory::TheoryId kindToTheoryId(::CVC5::Kind k);
::CVC5::theory::TheoryId typeConstantToTheoryId(
    ::CVC5::TypeConstant typeConstant);

}  // namespace theory
}  // namespace CVC5

#endif /* CVC4__KIND_H */
