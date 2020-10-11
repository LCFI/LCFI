// DO NOT MODIFY!
// File generated on 

// This file was generated from <LLFI_SRC_ROOT>/tools/FIDL/TargetSingleTemplate.cpp
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
    public:
//fidl_2
        if (funcNames.size() == 0) {
//fidl_3
        }
    }
    
    virtual void getCompileTimeInfo(std::map<std::string, std::string>& info) {
//fidl_4
        for(std::set<std::string>::iterator SI = funcNames.begin(); SI != funcNames.end(); SI++){
            info["targets"] += *SI + "()/";
        }
        //remove the '/' at the end
        info["targets"] = info["targets"].substr(0, info["targets"].length() - 1);
//fidl_5
    }

    private:
    static std::set<std::string> funcNames;

    virtual bool isInstFITarget(Instruction* inst) {
        if (!isa<CallInst>(inst)) {
            return false;
        }
        
        CallInst* CI = dyn_cast<CallInst>(inst);
        Function* called_func = CI->getCalledFunction();
        if (called_func == NULL) {
            return false;
        }
        
        std::string func_name = std::string(called_func->getName());
//fidl_6
    }
    
    static bool isTargetLLFIIndex(Instruction* inst) {
//fidl_7
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
    
    static std::set<std::string>::iterator key_partially_matches(std::string func_name) {
        std::set<std::string>::iterator SI;
        for (SI = funcNames.begin(); SI != funcNames.end(); SI++) {
            if (func_name.find(*SI) != std::string::npos) {
                break;
            }
        }
        return SI;
    }
};

