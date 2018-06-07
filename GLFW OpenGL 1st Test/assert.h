#pragma once

#define ASSERTIONS_ENABLED = 1

#ifdef ASSERTIONS_ENABLED
// NtTerminateProcess Windows 8.1 0x002b

#define ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
		std::cerr << #expr << " " << \
		__FILE__ << " " << __LINE__ << std::endl; \
		std::exit(-1); \
	}
#else
#define ASSERT(expr)
#endif