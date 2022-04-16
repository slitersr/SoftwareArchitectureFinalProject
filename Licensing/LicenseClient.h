#pragma once

#include <string>
#include <boost/optional.hpp>
#include "LicensingDLL.h"
#include "LicenseReserveFailure.h"

class dll LicenseClient {
public:
	virtual ~LicenseClient() {}
	virtual boost::optional<LicenseReserveFailure> reserveEntitlement(std::string license, std::string entitlement) = 0;
	virtual void unreserveEntitlement(std::string license, std::string entitlement) = 0;
};