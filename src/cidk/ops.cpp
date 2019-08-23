#include "cidk/cx.hpp"
#include "cidk/op.hpp"
#include "cidk/ops.hpp"

#include "cidk/ops/call.hpp"
#include "cidk/ops/check.hpp"
#include "cidk/ops/clock.hpp"
#include "cidk/ops/cp.hpp"
#include "cidk/ops/defconst.hpp"
#include "cidk/ops/defun.hpp"
#include "cidk/ops/dispatch.hpp"
#include "cidk/ops/do.hpp"
#include "cidk/ops/drop.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/ops/for.hpp"
#include "cidk/ops/include.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/ops/is.hpp"
#include "cidk/ops/isa.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/ops/link.hpp"
#include "cidk/ops/mark.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/recall.hpp"
#include "cidk/ops/set.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/ops/step.hpp"
#include "cidk/ops/swap.hpp"
#include "cidk/ops/sweep.hpp"
#include "cidk/ops/typeof.hpp"

namespace cidk {
  void mark_refs(Ops &in) {
    for (Op &o: in) { o.mark_refs(); }
  }

  void init_op_types(Cx &cx) {
    vector<const OpType *> ts {
      &ops::Call, &ops::Check, &ops::Clock, &ops::Cp,
        &ops::Defconst, &ops::Defun, &ops::Dispatch, &ops::Do, &ops::Drop, &ops::Dump,
        &ops::For,
        &ops::Include,
        &ops::If, &ops::Is, &ops::Isa,
        &ops::Let, &ops::Link, 
        &ops::Mark,
        &ops::Push,
        &ops::Recall,
        &ops::Set, &ops::Stash, &ops::Step, &ops::Swap, &ops::Sweep,
        &ops::Typeof
    };

    for (auto &t: ts) { cx.op_types.emplace(t->id, t); }
  }
}
