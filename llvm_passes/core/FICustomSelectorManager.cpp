#include "llvm/Support/raw_ostream.h"

#include "FIInstSelector.h"
#include "FIRegSelector.h"
#include "FICustomSelectorManager.h"

namespace llfi {

// fault injection instruction selector manager
FICustomInstSelectorManager
    *FICustomInstSelectorManager::getCustomInstSelectorManager() {
  static FICustomInstSelectorManager instsel_manager;
  return &instsel_manager;  
}

void FICustomInstSelectorManager::addCustomInstSelector(
    const std::string &name, FIInstSelector *instselector) {
  if (optionname_instselector.find(name) == optionname_instselector.end()) {
    optionname_instselector[name] = instselector;
  } else {
    errs() << "ERROR: Duplicate custom fault injection instruction selector: "
        << name << "\n";
    exit(1);
  }
}

void FICustomInstSelectorManager::getAllSoftwareSelectors(
  std::set<std::string>& all_software_failure_names){
  for(std::map<const std::string, FIInstSelector* >::iterator it = 
    optionname_instselector.begin(); it != optionname_instselector.end(); 
    ++it) {
      if(it->second->getInstSelectorClass() == std::string("SoftwareFault")){
        all_software_failure_names.insert(it->first);
      }
  }
  return;
}

void FICustomInstSelectorManager::getAllHardwareSelectors(
  std::set<std::string>& all_hardware_failure_names){
  for(std::map<const std::string, FIInstSelector* >::iterator it = 
    optionname_instselector.begin(); it != optionname_instselector.end(); 
    ++it) {
      if(it->second->getInstSelectorClass() == std::string("HardwareFault")){
        all_hardware_failure_names.insert(it->first);
      }
  }
  return;
}

FIInstSelector *FICustomInstSelectorManager::getCustomInstSelector(
    const std::string &name) {
  if (optionname_instselector.find(name) != optionname_instselector.end()) {
    return optionname_instselector[name];
  } else {
    errs() << "ERROR: Unknown custom fault injection instruction selector: "
        << name << "\n";
    exit(1);
  }
}


// fault injection register selector manager
FICustomRegSelectorManager
    *FICustomRegSelectorManager::getCustomRegSelectorManager() {
  static FICustomRegSelectorManager regsel_manager;
  return &regsel_manager;
}

void FICustomRegSelectorManager::addCustomRegSelector(
    const std::string &name, FIRegSelector *regselector) {
  if (optionname_regselector.find(name) == optionname_regselector.end()) {
    optionname_regselector[name] = regselector;
  } else {
    errs() << "ERROR: Duplicate custom fault injection register selector: "
        << name << "\n";
    exit(1);
  }
}

FIRegSelector *FICustomRegSelectorManager::getCustomRegSelector(
    const std::string &name) {
  if (optionname_regselector.find(name) != optionname_regselector.end()) {
    return optionname_regselector[name];
  } else {
    errs() << "ERROR: Unknown custom fault injection register selector: " <<
        name << "\n";
    exit(1);
  }
}

void FICustomRegSelectorManager::getAllSoftwareSelectors(
  std::set<std::string>& all_software_failure_names){
  for(std::map<const std::string, FIRegSelector* >::iterator it = 
    optionname_regselector.begin(); it != optionname_regselector.end(); 
    ++it) {
      if(it->second->getRegSelectorClass() == std::string("SoftwareFault")){
        all_software_failure_names.insert(it->first);
      }
  }
  return;
}

void FICustomRegSelectorManager::getAllHardwareSelectors(
  std::set<std::string>& all_hardware_failure_names){
  for(std::map<const std::string, FIRegSelector* >::iterator it = 
    optionname_regselector.begin(); it != optionname_regselector.end(); 
    ++it) {
      if(it->second->getRegSelectorClass() == std::string("HardwareFault")){
        all_hardware_failure_names.insert(it->first);
      }
  }
  return;
}

}
