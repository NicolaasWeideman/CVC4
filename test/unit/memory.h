/*********************                                                        */
/*! \file memory.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King, Morgan Deters, Aina Niemetz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility class to help testing out-of-memory conditions.
 **
 ** Utility class to help testing out-of-memory conditions.
 **
 ** Use it like this (for example):
 **
 **   CVC5::test::WithLimitedMemory wlm(amount);
 **   TS_ASSERT_THROWS( foo(), bad_alloc );
 **
 ** The WithLimitedMemory destructor will re-establish the previous limit.
 **
 ** This class does not exist in CVC4_MEMORY_LIMITING_DISABLED is defined.
 ** This can be disabled for a variety of reasons.
 ** If this is disabled, there will be a function:
 **   void WarnWithLimitedMemoryDisabledReason()
 ** uses TS_WARN to alert users that these tests are disabled.
 **/

#include "test.h"

#ifndef __CVC4__TEST__UNIT__MEMORY_H
#define __CVC4__TEST__UNIT__MEMORY_H

#include <string>
#include <sys/resource.h>
#include <sys/time.h>

#include "base/check.h"
#include "base/configuration_private.h"

// Conditionally define CVC4_MEMORY_LIMITING_DISABLED.
#ifdef __APPLE__
#  define CVC4_MEMORY_LIMITING_DISABLED 1
#  define CVC4_MEMORY_LIMITING_DISABLED_REASON "setrlimit() is broken on Mac."
#else /* __APPLE__ */

// Tests cannot expect bad_alloc to be thrown due to limit memory using
// setrlimit when ASAN is enable. ASAN instead aborts on mmap failures.
#  if IS_ASAN_BUILD
#    define CVC4_MEMORY_LIMITING_DISABLED 1
#    define CVC4_MEMORY_LIMITING_DISABLED_REASON "ASAN's mmap failures abort."
#  endif
#endif

namespace CVC5 {
namespace test {

#ifndef CVC4_MEMORY_LIMITING_DISABLED
class WithLimitedMemory {
 public:
  WithLimitedMemory() { remember(); }

  WithLimitedMemory(rlim_t amount) {
    remember();
    set(amount);
  }

  ~WithLimitedMemory() { set(d_prevAmount); }

  void set(rlim_t amount) {
    struct rlimit rlim;
    rlim.rlim_cur = amount;
    rlim.rlim_max = RLIM_INFINITY;
    ASSERT_EQ(setrlimit(RLIMIT_AS, &rlim), 0);
  }

 private:
  void remember() {
    struct rlimit rlim;
    ASSERT_EQ(getrlimit(RLIMIT_AS, &rlim), 0);
    d_prevAmount = rlim.rlim_cur;
  }

  rlim_t d_prevAmount;
}; /* class WithLimitedMemory */
#endif

}  // namespace test
}  // namespace CVC5

// Remove CVC4_MEMORY_LIMITING_DISABLED_REASON if it is defined.
#ifdef CVC4_MEMORY_LIMITING_DISABLED_REASON
#undef CVC4_MEMORY_LIMITING_DISABLED_REASON
#endif /* CVC4_MEMORY_LIMITING_DISABLED_REASON */

#endif /* __CVC4__TEST__MEMORY_H */
