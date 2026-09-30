#pragma once

#ifndef _SNOWY_MACROS_H_
#define _SNOWY_MACROS_H_

#include <iostream>

#ifdef _DEBUG
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x)
#endif // _DEBUG

#define COUT(...) std::cout << __VA_ARGS__ << std::endl
#define CERR(...) std::cerr << __VA_ARGS__ << std::endl
#define THROW_RUNTIME_ERR throw std::runtime_error

#define VALIDATE_OR_RET(x, ...) if (!(x)) { CERR(__VA_ARGS__); return false;}

#define ASSERT(cond, mesg) _STL_VERIFY(cond, mesg)
#define DEBUG_ASSERT(cond, mesg) IF_DEBUG( ASSERT(cond, mesg) )
#define SNOWY_ERROR(mesg) _STL_REPORT_ERROR(mesg)

#endif // !_SNOWY_MACROS_H_
