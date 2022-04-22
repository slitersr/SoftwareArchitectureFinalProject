#pragma once

#include <vector>

#include "LicenseLoader.h"

class StubLicenseLoader : public LicenseLoader {
private:
	std::vector<License> licenses;

public:
	StubLicenseLoader(std::vector<License> licenses) : licenses(licenses) {}

	virtual std::vector<License> loadLicenses() {
		return licenses;
	}

	virtual bool validateSignature(License license) {
		return true;
	}
};