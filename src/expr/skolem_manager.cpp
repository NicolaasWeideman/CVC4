/*********************                                                        */
/*! \file skolem_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of skolem manager class
 **/

#include "expr/skolem_manager.h"

#include "expr/attribute.h"
#include "expr/bound_var_manager.h"
#include "expr/node_algorithm.h"

using namespace CVC5::kind;

namespace CVC5 {

// Attributes are global maps from Nodes to data. Thus, note that these could
// be implemented as internal maps in SkolemManager.
struct WitnessFormAttributeId
{
};
typedef expr::Attribute<WitnessFormAttributeId, Node> WitnessFormAttribute;

struct SkolemFormAttributeId
{
};
typedef expr::Attribute<SkolemFormAttributeId, Node> SkolemFormAttribute;

struct OriginalFormAttributeId
{
};
typedef expr::Attribute<OriginalFormAttributeId, Node> OriginalFormAttribute;

Node SkolemManager::mkSkolem(Node v,
                             Node pred,
                             const std::string& prefix,
                             const std::string& comment,
                             int flags,
                             ProofGenerator* pg)
{
  // We do not currently insist that pred does not contain witness terms
  Assert(v.getKind() == BOUND_VARIABLE);
  // make the witness term
  NodeManager* nm = NodeManager::currentNM();
  Node bvl = nm->mkNode(BOUND_VAR_LIST, v);
  // Make the witness term, where notice that pred may contain skolems. We do
  // not recursively convert pred to witness form, since witness terms should
  // be treated as opaque. Moreover, the use of witness forms leads to
  // variable shadowing issues in e.g. skolemization.
  Node w = nm->mkNode(WITNESS, bvl, pred);
  // store the mapping to proof generator if it exists
  if (pg != nullptr)
  {
    // We cache based on the existential of the original predicate
    Node q = nm->mkNode(EXISTS, bvl, pred);
    // Notice this may overwrite an existing proof generator. This does not
    // matter since either should be able to prove q.
    d_gens[q] = pg;
  }
  Node k = mkSkolemInternal(w, prefix, comment, flags);
  // set witness form attribute for k
  WitnessFormAttribute wfa;
  k.setAttribute(wfa, w);
  Trace("sk-manager-skolem")
      << "skolem: " << k << " witness " << w << std::endl;
  return k;
}

Node SkolemManager::mkSkolemize(Node q,
                                std::vector<Node>& skolems,
                                const std::string& prefix,
                                const std::string& comment,
                                int flags,
                                ProofGenerator* pg)
{
  Trace("sk-manager-debug") << "mkSkolemize " << q << std::endl;
  Assert(q.getKind() == EXISTS);
  Node currQ = q;
  for (const Node& av : q[0])
  {
    Assert(currQ.getKind() == EXISTS && av == currQ[0][0]);
    // currQ is updated to the result of skolemizing its first variable in
    // the method below.
    Node sk = skolemize(currQ, currQ, prefix, comment, flags);
    Trace("sk-manager-debug")
        << "made skolem " << sk << " for " << av << std::endl;
    skolems.push_back(sk);
  }
  if (pg != nullptr)
  {
    // Same as above, this may overwrite an existing proof generator
    d_gens[q] = pg;
  }
  Trace("sk-manager-debug") << "...mkSkolemize returns " << currQ << std::endl;
  return currQ;
}

Node SkolemManager::skolemize(Node q,
                              Node& qskolem,
                              const std::string& prefix,
                              const std::string& comment,
                              int flags)
{
  Assert(q.getKind() == EXISTS);
  Node v;
  std::vector<Node> ovars;
  Trace("sk-manager-debug") << "mkSkolemize..." << std::endl;
  NodeManager* nm = NodeManager::currentNM();
  for (const Node& av : q[0])
  {
    if (v.isNull())
    {
      v = av;
      continue;
    }
    ovars.push_back(av);
  }
  Assert(!v.isNull());
  // make the predicate with one variable stripped off
  Node pred = q[1];
  Trace("sk-manager-debug") << "make exists predicate" << std::endl;
  if (!ovars.empty())
  {
    // keeps the same variables
    Node bvl = nm->mkNode(BOUND_VAR_LIST, ovars);
    // update the predicate
    pred = nm->mkNode(EXISTS, bvl, pred);
  }
  Trace("sk-manager-debug") << "call sub mkSkolem" << std::endl;
  // don't use a proof generator, since this may be an intermediate, partially
  // skolemized formula.
  Node k = mkSkolem(v, pred, prefix, comment, flags, nullptr);
  Assert(k.getType() == v.getType());
  TNode tv = v;
  TNode tk = k;
  Trace("sk-manager-debug")
      << "qskolem apply " << tv << " -> " << tk << " to " << pred << std::endl;
  // the quantified formula with one step of skolemization
  qskolem = pred.substitute(tv, tk);
  Trace("sk-manager-debug") << "qskolem done substitution" << std::endl;
  return k;
}

Node SkolemManager::mkPurifySkolem(Node t,
                                   const std::string& prefix,
                                   const std::string& comment,
                                   int flags)
{
  Node to = getOriginalForm(t);
  // We do not currently insist that to does not contain witness terms

  Node k = mkSkolemInternal(to, prefix, comment, flags);
  // set original form attribute for k
  OriginalFormAttribute ofa;
  k.setAttribute(ofa, to);

  Trace("sk-manager-skolem")
      << "skolem: " << k << " purify " << to << std::endl;
  return k;
}

Node SkolemManager::mkBooleanTermVariable(Node t)
{
  return mkPurifySkolem(t, "", "", NodeManager::SKOLEM_BOOL_TERM_VAR);
}

ProofGenerator* SkolemManager::getProofGenerator(Node t) const
{
  std::map<Node, ProofGenerator*>::const_iterator it = d_gens.find(t);
  if (it != d_gens.end())
  {
    return it->second;
  }
  return nullptr;
}

Node SkolemManager::getWitnessForm(Node k)
{
  Assert(k.getKind() == SKOLEM);
  // simply look up the witness form for k via an attribute
  WitnessFormAttribute wfa;
  return k.getAttribute(wfa);
}

Node SkolemManager::getOriginalForm(Node n)
{
  if (n.isNull())
  {
    return n;
  }
  Trace("sk-manager-debug")
      << "SkolemManager::getOriginalForm " << n << std::endl;
  OriginalFormAttribute ofa;
  NodeManager* nm = NodeManager::currentNM();
  std::unordered_map<TNode, Node, TNodeHashFunction> visited;
  std::unordered_map<TNode, Node, TNodeHashFunction>::iterator it;
  std::vector<TNode> visit;
  TNode cur;
  visit.push_back(n);
  do
  {
    cur = visit.back();
    visit.pop_back();
    it = visited.find(cur);

    if (it == visited.end())
    {
      if (cur.hasAttribute(ofa))
      {
        visited[cur] = cur.getAttribute(ofa);
      }
      else
      {
        visited[cur] = Node::null();
        visit.push_back(cur);
        if (cur.getMetaKind() == metakind::PARAMETERIZED)
        {
          visit.push_back(cur.getOperator());
        }
        for (const Node& cn : cur)
        {
          visit.push_back(cn);
        }
      }
    }
    else if (it->second.isNull())
    {
      Node ret = cur;
      bool childChanged = false;
      std::vector<Node> children;
      if (cur.getMetaKind() == metakind::PARAMETERIZED)
      {
        it = visited.find(cur.getOperator());
        Assert(it != visited.end());
        Assert(!it->second.isNull());
        childChanged = childChanged || cur.getOperator() != it->second;
        children.push_back(it->second);
      }
      for (const Node& cn : cur)
      {
        it = visited.find(cn);
        Assert(it != visited.end());
        Assert(!it->second.isNull());
        childChanged = childChanged || cn != it->second;
        children.push_back(it->second);
      }
      if (childChanged)
      {
        ret = nm->mkNode(cur.getKind(), children);
      }
      cur.setAttribute(ofa, ret);
      visited[cur] = ret;
    }
  } while (!visit.empty());
  Assert(visited.find(n) != visited.end());
  Assert(!visited.find(n)->second.isNull());
  Trace("sk-manager-debug") << "..return " << visited[n] << std::endl;
  return visited[n];
}

Node SkolemManager::mkSkolemInternal(Node w,
                                     const std::string& prefix,
                                     const std::string& comment,
                                     int flags)
{
  // note that witness, original forms are independent, but share skolems
  NodeManager* nm = NodeManager::currentNM();
  // w is not necessarily a witness term
  SkolemFormAttribute sfa;
  Node k;
  // could already have a skolem if we used w already
  if (w.hasAttribute(sfa))
  {
    return w.getAttribute(sfa);
  }
  // make the new skolem
  if (flags & NodeManager::SKOLEM_BOOL_TERM_VAR)
  {
    Assert (w.getType().isBoolean());
    k = nm->mkBooleanTermVariable();
  }
  else
  {
    k = nm->mkSkolem(prefix, w.getType(), comment, flags);
  }
  // set skolem form attribute for w
  w.setAttribute(sfa, k);
  Trace("sk-manager") << "SkolemManager::mkSkolem: " << k << " : " << w
                      << std::endl;
  return k;
}

}  // namespace CVC5
