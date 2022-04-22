#pragma once

#include <string>
#include "LicensingDLL.h"
#include "License.h"

class dll LicenseReserveFailure {
private:
	std::string context;
	std::string entitlement;
	bool allowContinue;
	std::string message;
public:
	const std::string getContext() const {
		return context;
	}

	void setContext(std::string context) {
		this->context = context;
	}

	const std::string getEntitlement() const {
		return entitlement;
	}

	void setEntitlement(std::string entitlement) {
		this->entitlement = entitlement;
	}

	const bool getAllowContinue() const {
		return allowContinue;
	}

	void setAllowContinue(bool allowContinue) {
		this->allowContinue = allowContinue;
	}

	const std::string getMessage() const {
		return message;
	}

	void setMessage(std::string message) {
		this->message = message;
	}
};
