/*********************                                                        */
/*! \file listener.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility classes for listeners and collections of listeners.
 **
 ** Utilities for the development of a Listener interface class. This class
 ** provides a single notification that must be overwritten.
 **/

#include "cvc4_public.h"

#ifndef CVC4__LISTENER_H
#define CVC4__LISTENER_H

namespace CVC5 {

/**
 * Listener interface class.
 *
 * The interface provides a notify() function.
 */
class Listener
{
 public:
  Listener();
  virtual ~Listener();

  /** Note that notify may throw arbitrary exceptions. */
  virtual void notify() = 0;
};

}  // namespace CVC5

#endif /* CVC4__LISTENER_H */
