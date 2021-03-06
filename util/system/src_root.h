#pragma once

#include "compiler.h"
#include "compat.h"

namespace NPrivate {
    struct TStaticBuf {
        constexpr TStaticBuf(const char* data, unsigned len) noexcept
            : Data(data)
            , Len(len)
        {
        }

        template <class T>
        constexpr T As() const noexcept {
            return T(Data, Len);
        }

        template <class T>
        constexpr operator T() const noexcept {
            return this->As<T>();
        }

        const char* Data;
        unsigned Len;
    };

#define STATIC_BUF(x) ::NPrivate::TStaticBuf(x, sizeof(x) - 1)

    constexpr TStaticBuf ArcRoot = STATIC_BUF(__XSTRING(ARCADIA_ROOT));
    constexpr TStaticBuf BuildRoot = STATIC_BUF(__XSTRING(ARCADIA_BUILD_ROOT));

    constexpr Y_FORCE_INLINE bool IsProperPrefix(const TStaticBuf prefix, const TStaticBuf string) noexcept {
        if (prefix.Len < string.Len) {
            for (unsigned i = prefix.Len; i-- > 0;) {
                if (prefix.Data[i] != string.Data[i]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    //$(SRC_ROOT)/prj/blah.cpp -> prj/blah.cpp
    constexpr Y_FORCE_INLINE TStaticBuf StripRoot(const TStaticBuf& f) noexcept {
        if (IsProperPrefix(ArcRoot, f)) {
            return TStaticBuf(f.Data + ArcRoot.Len + 1, f.Len - ArcRoot.Len - 1);
        }
        if (IsProperPrefix(BuildRoot, f)) {
            return TStaticBuf(f.Data + BuildRoot.Len + 1, f.Len - BuildRoot.Len - 1);
        }
        return f;
    }
}

#define __SOURCE_FILE_IMPL__ ::NPrivate::StripRoot(STATIC_BUF(__FILE__))
