#pragma once

#include <mitsuba/mitsuba.h>

#if defined(ENOKI_X86_64)
#  if defined(__GNUG__) && !defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wbool-operation"
#    pragma GCC diagnostic ignored "-Wbool-compare"
#  endif

#  include <asmjit/asmjit.h>

#  if defined(__GNUG__) && !defined(__clang__)
#    pragma GCC diagnostic pop
#  endif
#endif

#include <mutex>

NAMESPACE_BEGIN(mitsuba)

struct MTS_EXPORT_CORE Jit {
    std::mutex mutex;
#if defined(ENOKI_X86_64)
    asmjit::JitRuntime runtime;
#endif

    /**
     * \brief Statically initialize the JIT runtime
     *
     * This function also does a runtime-check to ensure that the host
     * processor supports all instruction sets which were selected at compile
     * time. If not, the application is terminated via \c abort().
     */
    static void static_initialization();

    /// Release all memory used by JIT-compiled routines
    static void static_shutdown();

    static Jit *get_instance();

private:
    Jit();
    Jit(const Jit &) = delete;
};

NAMESPACE_END(mitsuba)
