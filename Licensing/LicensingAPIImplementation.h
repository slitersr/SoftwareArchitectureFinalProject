#pragma once

#include "LicensingDLL.h"
#include "LicensingAPI.h"
#include "LicenseClient.h"
#include "LicenseLoader.h"

class dll LicensingAPIImplementation : public LicensingAPI {
private:
	LicenseClient* client;
	LicenseLoader* loader;

public:
	LicensingAPIImplementation(LicenseClient* client, LicenseLoader* loader) : client(client), loader(loader) {}

	virtual ~LicensingAPIImplementation() {
		delete client;
		delete loader;
	}

	virtual void initialize();
	virtual boost::optional<LicenseReserveFailure> reserve(std::string context, std::string entitlement);
	virtual void unreserve(std::string context, std::string entitlement);
	virtual void check(std::string context, std::string entitlement);
	virtual void shutdown();
};