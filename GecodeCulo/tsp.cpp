/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2007
 *
 *  Bugfixes provided by:
 *     Geoffrey Chu
 *
 *  Last modified:
 *     $Date: 2013-07-08 14:22:40 +0200 (Mon, 08 Jul 2013) $ by $Author: schulte $
 *     $Revision: 13820 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "stdafx.h"

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <algorithm>

using namespace Gecode;

/// Support for %TSP instances
namespace {

  /// This instance is taken from SICStus Prolog
  int PA_n = 1;
  int * PA_d;
  

  /// Problem instance
  class Problem {
  private:
    const int  _n; ///< Size
    const int* _d; ///< Distances
  public:
    /// Initialize problem instance
    Problem(const int n, const int* d);
    /// Return size of instance
    int size(void) const;
    /// Return distance between node \a i and \a j
    int d(int i, int j) const;
    /// Return distances
    const int* d(void) const;
    /// Return estimate for maximal cost of a path
    int max(void) const;
  };

  inline
  Problem::Problem(const int n=0, const int* d=0)
    : _n(n), _d(d) {}
  inline int
  Problem::size(void) const {
    return _n;
  }
  inline int
  Problem::d(int i, int j) const {
    return _d[i*_n+j];
  }
  inline const int*
  Problem::d(void) const {
    return _d;
  }
  inline int
  Problem::max(void) const {
    int m=0;
    for (int i=_n*_n; i--; )
      m = std::max(m,_d[i]);
    return m*_n;
  }

  Problem* PA;
}

/**
 * \brief %Example: Travelling salesman problem (%TSP)
 *
 * Simple travelling salesman problem instances. Just meant
 * as a test for circuit.
 *
 * \ingroup Example
 *
 */
class TSP : public IntMinimizeScript {
protected:
  /// Problem instance to be solved
  Problem     p;
  /// Successor edges
  IntVarArray succ;
  /// Total cost of travel
  IntVar      total;
public:
  /// Actual model
  TSP(const SizeOptions& opt)
    : p(*PA),
      succ(*this, p.size(), 0, p.size()-1),
      total(*this, 0, p.max()) {
    int n = p.size();

    // Cost matrix
    IntArgs c(n*n, p.d());

    for (int i=n; i--; )
      for (int j=n; j--; )
        if (p.d(i,j) == 0)
          rel(*this, succ[i], IRT_NQ, j);

    // Cost of each edge
    IntVarArgs costs(*this, n, Int::Limits::min, Int::Limits::max);

    // Enforce that the succesors yield a tour with appropriate costs
    circuit(*this, c, succ, costs, total, opt.icl());

    // Just assume that the circle starts forwards
    {
      IntVar p0(*this, 0, n-1);
      element(*this, succ, p0, 0);
      rel(*this, p0, IRT_LE, succ[0]);
    }

    // First enumerate cost values, prefer those that maximize cost reduction
    branch(*this, costs, INT_VAR_REGRET_MIN_MIN(), INT_VAL_SPLIT_MIN());

    // Then fix the remaining successors
    branch(*this, succ,  INT_VAR_MIN_MIN(), INT_VAL_MIN());
  }
  /// Return solution cost
  virtual IntVar cost(void) const {
    return total;
  }
  /// Constructor for cloning \a s
  TSP(bool share, TSP& s) : IntMinimizeScript(share,s), p(s.p) {
    succ.update(*this, share, s.succ);
    total.update(*this, share, s.total);
  }
  /// Copy during cloning
  virtual Space*
  copy(bool share) {
    return new TSP(share,*this);
  }
  /// Print solution
  virtual void
  print(std::ostream& os) const {
    bool assigned = true;
    for (int i=0; i<succ.size(); i++) {
      if (!succ[i].assigned()) {
        assigned = false;
        break;
      }
    }
    if (assigned) {
      os << "\tTour: ";
      int i=0;
      do {
        os << i << " -> ";
        i=succ[i].val();
      } while (i != 0);
      os << 0 << std::endl;
      os << "\tCost: " << total << std::endl;
    } else {
      os << "\tTour: " << std::endl;
      for (int i=0; i<succ.size(); i++) {
        os << "\t" << i << " -> " << succ[i] << std::endl;
      }
      os << "\tCost: " << total << std::endl;
    }

  }
};

// STATISTICS: example-any
