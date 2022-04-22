#pragma once

#include <vector>
#include <string>
#include <map>

#include "LicensingDLL.h"
#include "LicensingAPI.h"
#include "LicenseClient.h"
#include "LicenseLoader.h"

class dll LicensingAPIImplementation : public LicensingAPI {
private:
	LicenseClient* client;
	LicenseLoader* loader;
	std::vector<License> licenses;
	std::map<std::string, std::vector<License>> reservations;
	bool running = false;

	std::vector<License>& reservationsByEntitlement(std::string entitlement);

public:
	LicensingAPIImplementation(LicenseClient* client, LicenseLoader* loader) : client(client), loader(loader) {}

	virtual void initialize();
	virtual boost::optional<LicenseReserveFailure> reserve(std::string context, std::string entitlement);
	virtual bool unreserve(std::string context, std::string entitlement);
	virtual void check(std::string entitlement);
	virtual void shutdown();
};