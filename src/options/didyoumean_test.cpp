/*********************                                                        */
/*! \file didyoumean_test.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Kshitij Bansal, Tim King, Andrew Reynolds
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

// This is not built as a part of CVC4 and is not built by Makefile.am.
// Compile: g++ didyoumean_test.cpp didyoumean.cpp
// For debug compile with -DDIDYOUMEAN_DEBUG or -DDIDYOUMEAN_DEBUG1 or both

#include "didyoumean.h"
#include <iostream>

using namespace std;
using namespace CVC5;

set<string> getDebugTags();
set<string> getOptionStrings();

int main() {
  string a, b;

  cin >> a;
  cout << "Matches with debug tags:" << endl;
  for (const string& s : DidYouMean(getDebugTags()).getMatch(a)) {
    cout << s << endl;
  }
  cout << "Matches with option strings:" << endl;
  for (const string& s : DidYouMean(getOptionStrings()).getMatch(a)) {
    cout << s << endl;
  }
}

set<string> getDebugTags() {
  set<string> a;
  a.insert("CDInsertHashMap");
  a.insert("CDTrailHashMap");
  a.insert("TrailHashMap");
  a.insert("approx");
  a.insert("approx::");
  a.insert("approx::branch");
  a.insert("approx::checkCutOnPad");
  a.insert("approx::constraint");
  a.insert("approx::gmi");
  a.insert("approx::gmiCut");
  a.insert("approx::guessIsConstructable");
  a.insert("approx::lemmas");
  a.insert("approx::mir");
  a.insert("approx::mirCut");
  a.insert("approx::nodelog");
  a.insert("approx::replayAssert");
  a.insert("approx::replayLogRec");
  a.insert("approx::soi");
  a.insert("approx::solveMIP");
  a.insert("approx::sumConstraints");
  a.insert("approx::vars");
  a.insert("arith");
  a.insert("arith::addSharedTerm");
  a.insert("arith::approx::cuts");
  a.insert("arith::arithvar");
  a.insert("arith::asVectors");
  a.insert("arith::bt");
  a.insert("arith::collectModelInfo");
  a.insert("arith::conflict");
  a.insert("arith::congruenceManager");
  a.insert("arith::congruences");
  a.insert("arith::consistency");
  a.insert("arith::consistency::comitonconflict");
  a.insert("arith::consistency::final");
  a.insert("arith::consistency::initial");
  a.insert("arith::constraint");
  a.insert("arith::dio");
  a.insert("arith::dio::ex");
  a.insert("arith::dio::max");
  a.insert("arith::div");
  a.insert("arith::dual");
  a.insert("arith::ems");
  a.insert("arith::eq");
  a.insert("arith::error::mem");
  a.insert("arith::explain");
  a.insert("arith::explainNonbasics");
  a.insert("arith::findModel");
  a.insert("arith::focus");
  a.insert("arith::hasIntegerModel");
  a.insert("arith::importSolution");
  a.insert("arith::ite");
  a.insert("arith::ite::red");
  a.insert("arith::learned");
  a.insert("arith::lemma");
  a.insert("arith::nf");
  a.insert("arith::oldprop");
  a.insert("arith::pivot");
  a.insert("arith::preprocess");
  a.insert("arith::preregister");
  a.insert("arith::presolve");
  a.insert("arith::print_assertions");
  a.insert("arith::print_model");
  a.insert("arith::prop");
  a.insert("arith::resolveOutPropagated");
  a.insert("arith::restart");
  a.insert("arith::rewriter");
  a.insert("arith::selectPrimalUpdate");
  a.insert("arith::simplex:row");
  a.insert("arith::solveInteger");
  a.insert("arith::static");
  a.insert("arith::subsumption");
  a.insert("arith::tracking");
  a.insert("arith::tracking::mid");
  a.insert("arith::tracking::post");
  a.insert("arith::tracking::pre");
  a.insert("arith::unate");
  a.insert("arith::unate::conf");
  a.insert("arith::update");
  a.insert("arith::update::select");
  a.insert("arith::value");
  a.insert("array-pf");
  a.insert("array-types");
  a.insert("arrays");
  a.insert("arrays-model-based");
  a.insert("arrays::propagate");
  a.insert("arrays::sharing");
  a.insert("attrgc");
  a.insert("basicsAtBounds");
  a.insert("bitvector");
  a.insert("bitvector-bb");
  a.insert("bitvector-bitblast");
  a.insert("bitvector-expandDefinition");
  a.insert("bitvector-model");
  a.insert("bitvector-preregister");
  a.insert("bitvector-rewrite");
  a.insert("bitvector::bitblaster");
  a.insert("bitvector::core");
  a.insert("bitvector::explain");
  a.insert("bitvector::propagate");
  a.insert("bitvector::sharing");
  a.insert("bool-flatten");
  a.insert("bool-ite");
  a.insert("boolean-terms");
  a.insert("bt");
  a.insert("builder");
  a.insert("bv-bitblast");
  a.insert("bv-core");
  a.insert("bv-core-model");
  a.insert("bv-inequality");
  a.insert("bv-inequality-explain");
  a.insert("bv-inequality-internal");
  a.insert("bv-rewrite");
  a.insert("bv-slicer");
  a.insert("bv-slicer-eq");
  a.insert("bv-slicer-uf");
  a.insert("bv-subtheory-inequality");
  a.insert("bv-to-bool");
  a.insert("bva");
  a.insert("bvminisat");
  a.insert("bvminisat::explain");
  a.insert("bvminisat::search");
  a.insert("cegqi");
  a.insert("cegqi-debug");
  a.insert("cegqi-prop-as-dec");
  a.insert("cd_set_collection");
  a.insert("cdlist");
  a.insert("cdlist:cmm");
  a.insert("cdqueue");
  a.insert("check-inst");
  a.insert("check-model::rep-checking");
  a.insert("circuit-prop");
  a.insert("cnf");
  a.insert("constructInfeasiblityFunction");
  a.insert("context");
  a.insert("current");
  a.insert("datatypes");
  a.insert("datatypes-cycle-check");
  a.insert("datatypes-cycles");
  a.insert("datatypes-cycles-find");
  a.insert("datatypes-debug");
  a.insert("datatypes-explain");
  a.insert("datatypes-gt");
  a.insert("datatypes-inst");
  a.insert("datatypes-labels");
  a.insert("datatypes-output");
  a.insert("datatypes-parametric");
  a.insert("datatypes-prereg");
  a.insert("datatypes-split");
  a.insert("decision");
  a.insert("decision::jh");
  a.insert("determineArithVar");
  a.insert("diamonds");
  a.insert("dio::push");
  a.insert("dt");
  a.insert("dt-enum");
  a.insert("dt-warn");
  a.insert("dt::propagate");
  a.insert("dualLike");
  a.insert("effortlevel");
  a.insert("ensureLiteral");
  a.insert("eq");
  a.insert("equality");
  a.insert("equality::backtrack");
  a.insert("equality::disequality");
  a.insert("equality::evaluation");
  a.insert("equality::graph");
  a.insert("equality::internal");
  a.insert("equality::trigger");
  a.insert("equalsConstant");
  a.insert("error");
  a.insert("estimateWithCFE");
  a.insert("expand");
  a.insert("export");
  a.insert("flipdec");
  a.insert("fmc-entry-trie");
  a.insert("fmc-interval-model-debug");
  a.insert("fmf-card-debug");
  a.insert("fmf-eval-debug");
  a.insert("fmf-eval-debug2");
  a.insert("fmf-exit");
  a.insert("fmf-index-order");
  a.insert("fmf-model-complete");
  a.insert("fmf-model-cons");
  a.insert("fmf-model-cons-debug");
  a.insert("fmf-model-eval");
  a.insert("fmf-model-prefs");
  a.insert("fmf-model-req");
  a.insert("focusDownToJust");
  a.insert("focusDownToLastHalf");
  a.insert("foo");
  a.insert("gaussianElimConstructTableRow");
  a.insert("gc");
  a.insert("gc:leaks");
  a.insert("getBestImpliedBound");
  a.insert("getCeiling");
  a.insert("getNewDomainValue");
  a.insert("getPropagatedLiterals");
  a.insert("getType");
  a.insert("glpk::loadVB");
  a.insert("guessCoefficientsConstructTableRow");
  a.insert("guessIsConstructable");
  a.insert("handleBorders");
  a.insert("includeBoundUpdate");
  a.insert("inst");
  a.insert("inst-engine");
  a.insert("inst-engine-ctrl");
  a.insert("inst-engine-debug");
  a.insert("inst-engine-phase-req");
  a.insert("inst-engine-stuck");
  a.insert("inst-fmf-ei");
  a.insert("inst-match-gen");
  a.insert("inst-trigger");
  a.insert("integers");
  a.insert("interactive");
  a.insert("intersectConstantIte");
  a.insert("isConst");
  a.insert("ite");
  a.insert("ite::atom");
  a.insert("ite::constantIteEqualsConstant");
  a.insert("ite::print-success");
  a.insert("ite::simpite");
  a.insert("lemma-ites");
  a.insert("lemmaInputChannel");
  a.insert("literal-matching");
  a.insert("logPivot");
  a.insert("matrix");
  a.insert("minisat");
  a.insert("minisat::lemmas");
  a.insert("minisat::pop");
  a.insert("minisat::remove-clause");
  a.insert("minisat::search");
  a.insert("miplib");
  a.insert("model");
  a.insert("model-getvalue");
  a.insert("nf::tmp");
  a.insert("nm");
  a.insert("normal-form");
  a.insert("options");
  a.insert("paranoid:check_tableau");
  a.insert("parser");
  a.insert("parser-extra");
  a.insert("parser-idt");
  a.insert("parser-param");
  a.insert("partial_model");
  a.insert("pb");
  a.insert("pipe");
  a.insert("portfolio::outputmode");
  a.insert("prec");
  a.insert("preemptGetopt");
  a.insert("proof:sat");
  a.insert("proof:sat:detailed");
  a.insert("prop");
  a.insert("prop-explain");
  a.insert("prop-value");
  a.insert("prop::lemmas");
  a.insert("propagateAsDecision");
  a.insert("properConflict");
  a.insert("qcf-ccbe");
  a.insert("qcf-check-inst");
  a.insert("qcf-eval");
  a.insert("qcf-match");
  a.insert("qcf-match-debug");
  a.insert("qcf-nground");
  a.insert("qint-check-debug2");
  a.insert("qint-debug");
  a.insert("qint-error");
  a.insert("qint-model-debug");
  a.insert("qint-model-debug2");
  a.insert("qint-var-order-debug2");
  a.insert("quant-arith");
  a.insert("quant-arith-debug");
  a.insert("quant-arith-simplex");
  a.insert("quant-datatypes");
  a.insert("quant-datatypes-debug");
  a.insert("quant-req-phase");
  a.insert("quant-uf-strategy");
  a.insert("quantifiers");
  a.insert("quantifiers-assert");
  a.insert("quantifiers-check");
  a.insert("quantifiers-dec");
  a.insert("quantifiers-engine");
  a.insert("quantifiers-flip");
  a.insert("quantifiers-other");
  a.insert("quantifiers-prereg");
  a.insert("quantifiers-presolve");
  a.insert("quantifiers-relevance");
  a.insert("quantifiers-sat");
  a.insert("quantifiers-substitute-debug");
  a.insert("quantifiers::collectModelInfo");
  a.insert("queueConditions");
  a.insert("rationalToCfe");
  a.insert("recentlyViolated");
  a.insert("register");
  a.insert("register::internal");
  a.insert("relevant-trigger");
  a.insert("removeFixed");
  a.insert("rl");
  a.insert("sat::minisat");
  a.insert("selectFocusImproving");
  a.insert("set_collection");
  a.insert("sets");
  a.insert("sets-assert");
  a.insert("sets-checkmodel-ignore");
  a.insert("sets-eq");
  a.insert("sets-learn");
  a.insert("sets-lemma");
  a.insert("sets-model");
  a.insert("sets-model-details");
  a.insert("sets-parent");
  a.insert("sets-pending");
  a.insert("sets-prop");
  a.insert("sets-prop-details");
  a.insert("sets-scrutinize");
  a.insert("sets-terminfo");
  a.insert("shared");
  a.insert("shared-terms-database");
  a.insert("shared-terms-database::assert");
  a.insert("sharing");
  a.insert("simple-trigger");
  a.insert("simplify");
  a.insert("smart-multi-trigger");
  a.insert("smt");
  a.insert("soi::findModel");
  a.insert("soi::selectPrimalUpdate");
  a.insert("solveRealRelaxation");
  a.insert("sort");
  a.insert("speculativeUpdate");
  a.insert("strings");
  a.insert("strings-explain");
  a.insert("strings-explain-debug");
  a.insert("strings-prereg");
  a.insert("strings-propagate");
  a.insert("substitution");
  a.insert("substitution::internal");
  a.insert("tableau");
  a.insert("te");
  a.insert("term-db-cong");
  a.insert("theory");
  a.insert("theory::assertions");
  a.insert("theory::atoms");
  a.insert("theory::bv::rewrite");
  a.insert("theory::conflict");
  a.insert("theory::explain");
  a.insert("theory::idl");
  a.insert("theory::idl::model");
  a.insert("theory::internal");
  a.insert("theory::propagate");
  a.insert("trans-closure");
  a.insert("treat-unknown-error");
  a.insert("tuprec");
  a.insert("typecheck-idt");
  a.insert("typecheck-q");
  a.insert("typecheck-r");
  a.insert("uf");
  a.insert("uf-ss");
  a.insert("uf-ss-check-region");
  a.insert("uf-ss-cliques");
  a.insert("uf-ss-debug");
  a.insert("uf-ss-disequal");
  a.insert("uf-ss-na");
  a.insert("uf-ss-region");
  a.insert("uf-ss-region-debug");
  a.insert("uf-ss-register");
  a.insert("uf-ss-sat");
  a.insert("uf::propagate");
  a.insert("uf::sharing");
  a.insert("ufgc");
  a.insert("ufsymm");
  a.insert("ufsymm:clauses");
  a.insert("ufsymm:eq");
  a.insert("ufsymm:match");
  a.insert("ufsymm:norm");
  a.insert("ufsymm:p");
  a.insert("update");
  a.insert("updateAndSignal");
  a.insert("weak");
  a.insert("whytheoryenginewhy");
  return a;
}

set<string> getOptionStrings() {
  const char* cmdlineOptions[] = {
      "lang",
      "output-lang",
      "language",
      "output-language",
      "verbose",
      "quiet",
      "stats",
      "no-stats",
      "statistics",
      "no-statistics",
      "stats-every-query",
      "no-stats-every-query",
      "statistics-every-query",
      "no-statistics-every-query",
      "parse-only",
      "no-parse-only",
      "preprocess-only",
      "no-preprocess-only",
      "trace",
      "debug",
      "print-success",
      "no-print-success",
      "smtlib-strict",
      "default-expr-depth",
      "default-dag-thresh",
      "print-expr-types",
      "eager-type-checking",
      "lazy-type-checking",
      "no-type-checking",
      "biased-ites",
      "no-biased-ites",
      "boolean-term-conversion-mode",
      "theoryof-mode",
      "use-theory",
      "bitblast-eager",
      "no-bitblast-eager",
      "bitblast-share-lemmas",
      "no-bitblast-share-lemmas",
      "bitblast-eager-fullcheck",
      "no-bitblast-eager-fullcheck",
      "bv-inequality-solver",
      "no-bv-inequality-solver",
      "bv-core-solver",
      "no-bv-core-solver",
      "bv-to-bool",
      "no-bv-to-bool",
      "bv-propagate",
      "no-bv-propagate",
      "bv-eq",
      "no-bv-eq",
      "dt-rewrite-error-sel",
      "no-dt-rewrite-error-sel",
      "dt-force-assignment",
      "unate-lemmas",
      "arith-prop",
      "heuristic-pivots",
      "standard-effort-variable-order-pivots",
      "error-selection-rule",
      "simplex-check-period",
      "pivot-threshold",
      "prop-row-length",
      "disable-dio-solver",
      "enable-arith-rewrite-equalities",
      "disable-arith-rewrite-equalities",
      "enable-miplib-trick",
      "disable-miplib-trick",
      "miplib-trick-subs",
      "cut-all-bounded",
      "no-cut-all-bounded",
      "maxCutsInContext",
      "revert-arith-models-on-unsat",
      "no-revert-arith-models-on-unsat",
      "fc-penalties",
      "no-fc-penalties",
      "use-fcsimplex",
      "no-use-fcsimplex",
      "use-soi",
      "no-use-soi",
      "restrict-pivots",
      "no-restrict-pivots",
      "collect-pivot-stats",
      "no-collect-pivot-stats",
      "use-approx",
      "no-use-approx",
      "approx-branch-depth",
      "dio-decomps",
      "no-dio-decomps",
      "new-prop",
      "no-new-prop",
      "arith-prop-clauses",
      "soi-qe",
      "no-soi-qe",
      "rewrite-divk",
      "no-rewrite-divk",
      "se-solve-int",
      "no-se-solve-int",
      "lemmas-on-replay-failure",
      "no-lemmas-on-replay-failure",
      "dio-turns",
      "rr-turns",
      "dio-repeat",
      "no-dio-repeat",
      "replay-early-close-depth",
      "replay-failure-penalty",
      "replay-num-err-penalty",
      "replay-reject-cut",
      "replay-lemma-reject-cut",
      "replay-soi-major-threshold",
      "replay-soi-major-threshold-pen",
      "replay-soi-minor-threshold",
      "replay-soi-minor-threshold-pen",
      "symmetry-breaker",
      "no-symmetry-breaker",
      "condense-function-values",
      "no-condense-function-values",
      "disable-uf-ss-regions",
      "uf-ss-eager-split",
      "no-uf-ss-eager-split",
      "uf-ss-totality",
      "no-uf-ss-totality",
      "uf-ss-totality-limited",
      "uf-ss-totality-sym-break",
      "no-uf-ss-totality-sym-break",
      "uf-ss-abort-card",
      "uf-ss-explained-cliques",
      "no-uf-ss-explained-cliques",
      "uf-ss-simple-cliques",
      "no-uf-ss-simple-cliques",
      "uf-ss-deq-prop",
      "no-uf-ss-deq-prop",
      "disable-uf-ss-min-model",
      "uf-ss-clique-splits",
      "no-uf-ss-clique-splits",
      "uf-ss-sym-break",
      "no-uf-ss-sym-break",
      "uf-ss-fair",
      "no-uf-ss-fair",
      "arrays-optimize-linear",
      "no-arrays-optimize-linear",
      "arrays-lazy-rintro1",
      "no-arrays-lazy-rintro1",
      "arrays-model-based",
      "no-arrays-model-based",
      "arrays-eager-index",
      "no-arrays-eager-index",
      "arrays-eager-lemmas",
      "no-arrays-eager-lemmas",
      "disable-miniscope-quant",
      "disable-miniscope-quant-fv",
      "disable-prenex-quant",
      "disable-var-elim-quant",
      "disable-ite-lift-quant",
      "cnf-quant",
      "no-cnf-quant",
      "clause-split",
      "no-clause-split",
      "pre-skolem-quant",
      "no-pre-skolem-quant",
      "ag-miniscope-quant",
      "no-ag-miniscope-quant",
      "macros-quant",
      "no-macros-quant",
      "fo-prop-quant",
      "no-fo-prop-quant",
      "disable-smart-triggers",
      "relevant-triggers",
      "no-relevant-triggers",
      "relational-triggers",
      "no-relational-triggers",
      "register-quant-body-terms",
      "no-register-quant-body-terms",
      "inst-when",
      "eager-inst-quant",
      "no-eager-inst-quant",
      "full-saturate-quant",
      "no-full-saturate-quant",
      "literal-matching",
      "enable-cbqi",
      "no-enable-cbqi",
      "cegqi-recurse",
      "no-cbqi-recurse",
      "user-pat",
      "flip-decision",
      "disable-quant-internal-reps",
      "finite-model-find",
      "no-finite-model-find",
      "mbqi",
      "mbqi-one-inst-per-round",
      "no-mbqi-one-inst-per-round",
      "mbqi-one-quant-per-round",
      "no-mbqi-one-quant-per-round",
      "fmf-inst-engine",
      "no-fmf-inst-engine",
      "disable-fmf-inst-gen",
      "fmf-inst-gen-one-quant-per-round",
      "no-fmf-inst-gen-one-quant-per-round",
      "fmf-fresh-dc",
      "no-fmf-fresh-dc",
      "disable-fmf-fmc-simple",
      "fmf-bound-int",
      "no-fmf-bound-int",
      "axiom-inst",
      "quant-cf",
      "no-quant-cf",
      "quant-cf-mode",
      "quant-cf-when",
      "rewrite-rules",
      "no-rewrite-rules",
      "rr-one-inst-per-round",
      "no-rr-one-inst-per-round",
      "strings-exp",
      "no-strings-exp",
      "strings-lb",
      "strings-fmf",
      "no-strings-fmf",
      "strings-eit",
      "no-strings-eit",
      "strings-alphabet-card",
      "show-sat-solvers",
      "random-freq",
      "random-seed",
      "restart-int-base",
      "restart-int-inc",
      "refine-conflicts",
      "no-refine-conflicts",
      "minisat-elimination",
      "no-minisat-elimination",
      "minisat-dump-dimacs",
      "no-minisat-dump-dimacs",
      "model-format",
      "dump",
      "dump-to",
      "force-logic",
      "simplification",
      "no-simplification",
      "static-learning",
      "no-static-learning",
      "produce-models",
      "no-produce-models",
      "check-models",
      "no-check-models",
      "dump-models",
      "no-dump-models",
      "proof",
      "no-proof",
      "check-proofs",
      "no-check-proofs",
      "dump-proofs",
      "no-dump-proofs",
      "produce-unsat-cores",
      "no-produce-unsat-cores",
      "produce-assignments",
      "no-produce-assignments",
      "interactive",
      "no-interactive",
      "ite-simp",
      "no-ite-simp",
      "on-repeat-ite-simp",
      "no-on-repeat-ite-simp",
      "simp-with-care",
      "no-simp-with-care",
      "simp-ite-compress",
      "no-simp-ite-compress",
      "unconstrained-simp",
      "no-unconstrained-simp",
      "repeat-simp",
      "no-repeat-simp",
      "simp-ite-hunt-zombies",
      "sort-inference",
      "no-sort-inference",
      "incremental",
      "no-incremental",
      "abstract-values",
      "no-abstract-values",
      "model-u-dt-enum",
      "no-model-u-dt-enum",
      "tlimit",
      "tlimit-per",
      "rlimit",
      "rlimit-per",
      "rewrite-apply-to-const",
      "no-rewrite-apply-to-const",
      "replay",
      "replay-log",
      "decision",
      "decision-threshold",
      "decision-use-weight",
      "no-decision-use-weight",
      "decision-random-weight",
      "decision-weight-internal",
      "version",
      "license",
      "help",
      "show-config",
      "show-debug-tags",
      "show-trace-tags",
      "early-exit",
      "no-early-exit",
      "threads",
      "threadN",
      "filter-lemma-length",
      "fallback-sequential",
      "no-fallback-sequential",
      "incremental-parallel",
      "no-incremental-parallel",
      "no-interactive-prompt",
      "immediate-exit",
      "segv-spin",
      "no-segv-spin",
      "segv-nospin",
      "wait-to-join",
      "no-wait-to-join",
      "strict-parsing",
      "no-strict-parsing",
      "mmap",
      "no-mmap",
      "no-checking",
      "no-filesystem-access",
      "no-include-file",
      "enable-idl-rewrite-equalities",
      "disable-idl-rewrite-equalities",
      "sets-propagate",
      "no-sets-propagate",
      "sets-eager-lemmas",
      "no-sets-eager-lemmas",
      NULL,
  }; /* cmdlineOptions */
  int i = 0;
  set<string> ret;
  while (cmdlineOptions[i] != NULL) {
    ret.insert(cmdlineOptions[i]);
    i++;
  }
  return ret;
}
