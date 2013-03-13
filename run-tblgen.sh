#!/usr/bin/zsh

LLVM_DIR="`$llvm_config --src-root`"

./wattage-tblgen $@ -I"$LLVM_DIR/include" -I"$LLVM_DIR/lib/Target/X86" < "$LLVM_DIR/lib/Target/X86/X86.td"
