rm -rf $HOME/LCFI-build &&
chmod +x setup && chmod +x tools/FIDL/FIDL-Algorithm.py
./setup -LLFI_BUILD_ROOT $HOME/LCFI-build -LLVM_SRC_ROOT $HOME/llvm-3.4 -LLVM_DST_ROOT $HOME/llvm-build --no_gui &&
cd $HOME/LCFI-build/bin && 
chmod +x instrument && chmod +x profile && chmod +x injectfault &&
chmod +x $HOME/LCFI-build/tools/GenerateMakefile
