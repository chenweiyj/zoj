/*
 * Copyright 2007 Xu, Chuan <xuchuan@gmail.com>
 *
 * This file is part of ZOJ.
 *
 * ZOJ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZOJ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZOJ. if not, see <http://www.gnu.org/licenses/>.
 */

#include "script_runner.h"

#include <string>

#include <signal.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common_io.h"
#include "logging.h"
#include "protocol.h"
#include "tracer.h"
#include "util.h"

namespace {

class ScriptTracer : public Tracer {
  public:
    ScriptTracer(pid_t pid, ScriptRunner* runner) : Tracer(pid), runner_(runner) {
    }

  protected:
    virtual void OnExit() {
        runner_->UpdateStatus();
    }

    virtual bool HandleSyscall(struct user_regs_struct& regs) {
        return Tracer::HandleSyscall(regs);
    }

  private:
    ScriptRunner* runner_;
};

}

Tracer* ScriptRunner::CreateTracer(pid_t pid, Runner* runner) {
    ScriptRunner* r = dynamic_cast<ScriptRunner*>(runner);
    return new ScriptTracer(pid, r);
}

void ScriptRunner::InternalRun() {
    RunProgram(commands);
}
