#pragma once

#include "LicenseClient.h"

class dll LicenseClientImplementation : public LicenseClient {
public:
	virtual ~LicenseClientImplementation() {}
	virtual boost::optional<LicenseReserveFailure> reserveEntitlement(std::string license, std::string entitlement);
	virtual void unreserveEntitlement(std::string license, std::string entitlement);
};