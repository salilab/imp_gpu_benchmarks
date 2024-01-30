Place the private header files in this directory. They will be
available to your code with

     #include <IMP/custom/internal/myheader.h>

All headers should include `IMP/custom/custom_config.h` as their
first include and surround all code with `IMPCUSTOM_BEGIN_INTERNAL_NAMESPACE`
and `IMPCUSTOM_END_INTERNAL_NAMESPACE` to put it in the
IMP::custom::internal namespace and manage compiler warnings.
