#pragma once

#include <string>
#include "LicensingDLL.h"
#include "License.h"

class dll LicenseReserveFailure {
private:
	License license;
	std::string context;
	std::string entitlement;
	bool allowContinue;
	std::string message;
public:
	License getLicense() {
		return license;
	}

	void setLicense(License license) {
		this->license = license;
	}

	std::string getContext() {
		return context;
	}

	void setContext(std::string context) {
		this->context = context;
	}

	std::string getEntitlement() {
		return entitlement;
	}

	void setEntitlement(std::string entitlement) {
		this->entitlement = entitlement;
	}

	bool getAllowContinue() {
		return allowContinue;
	}

	void setAllowContinue(bool allowContinue) {
		this->allowContinue = allowContinue;
	}

	std::string getMessage() {
		return message;
	}

	void setMessage(std::string message) {
		this->message = message;
	}
};
