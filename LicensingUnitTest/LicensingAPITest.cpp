#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"

#include "LicensingAPIImplementation.h"

std::ostream& operator<<(std::ostream& os, const LicenseReserveFailure& a)
{
	os << "STUB PRINT RESERVE FAILURE";
	return os;
}