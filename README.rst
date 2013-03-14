Wattage
=======

Runtime estimation of power characteristics of a program.  Implements
[1].

Building
--------

Wattage uses Intel's Pin [2] framework and llvm[3].  To build wattage:

 * symlink the directory containing Pin to ./pin
 * set the environment variable llvm_config to the path where you have
   your llvm-config executable.
 * run `make`.

[1] Steinke, S., Knauer, M., Wehmeyer, L., & Marwedel, P. (2001). An accurate and fine grain instruction-level energy model supporting software optimizations. Proc of PATMOS. Citeseer.

[2] http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool

[3] http://llvm.org
