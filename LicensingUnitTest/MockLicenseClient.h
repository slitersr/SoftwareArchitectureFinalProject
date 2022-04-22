#pragma once

#include "gmock/gmock.h"
#include "boost/optional/optional_io.hpp"
#include "LicenseClient.h"

class MockLicenseClient : public LicenseClient {
public:
	MOCK_METHOD(boost::optional<LicenseReserveFailure>, reserveEntitlement, (std::string license, std::string entitlement), (override));
	MOCK_METHOD(void, unreserveEntitlement, (std::string license, std::string entitlement), (override));
};

std::ostream& operator<<(std::ostream& os, const LicenseReserveFailure& m);