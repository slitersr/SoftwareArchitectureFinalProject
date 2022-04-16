#pragma once

#ifdef LicensingDLL
#define dll __declspec(dllexport)
#else
#define dll __declspec(dllimport)
#endif