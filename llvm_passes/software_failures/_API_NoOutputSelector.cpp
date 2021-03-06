// DO NOT MODIFY!
// File generated on 2020/10/12 01:13:12 CST

// This file was generated from <LLFI_SRC_ROOT>/tools/FIDL/TargetAllTemplate.cpp
// by the <LLFI_SRC_ROOT>/tools/FIDL/FIDL-Algorithm.py
// See https://github.com/DependableSystemsLab/LLFI/wiki/Using-FIDL-to-create-a-Custom-Software-Fault-Injector-and-a-Custom-Instruction-Selector
// for more information.

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/CFG.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/GraphTraits.h"

#include "Utils.h"
#include "FIInstSelector.h"
#include "FICustomSelectorManager.h"
#include "_SoftwareFaultRegSelectors.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace llvm;
namespace llfi {
//fidl_1
class _API_NoOutputInstSelector : public SoftwareFIInstSelector {
    public:
    virtual void getCompileTimeInfo(std::map<std::string, std::string>& info) {
//fidl_2
        info["failure_class"] = "API";
        info["failure_mode"] = "NoOutput";
        info["injector"] = "HangInjector";
        info["targets"] = "return";
    }
    private:
    virtual bool isInstFITarget(Instruction* inst) {
//fidl_3
        return isa<ReturnInst>(inst);
    }
    
    static bool isTargetLLFIIndex(Instruction* inst) {
//fidl_4
        const long n = 0;
        const long targeted_indices[] = {};
        if (n > 0) {
            long llfiindex = getLLFIIndexofInst(inst);
            for (int i = 0; i < n; i++) {  
                if (llfiindex == targeted_indices[i]) { 
                    return true;
                }
            }
            return false;
        } else {
            return true;
        }
    }
};

static RegisterFIInstSelector A("NoOutput(API)", new _API_NoOutputInstSelector());
static RegisterFIRegSelector B("NoOutput(API)", new RetValRegSelector());

}

