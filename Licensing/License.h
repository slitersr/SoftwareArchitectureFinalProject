#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "LicensingDLL.h"

class dll License {
private:
	std::string id;
	std::vector<std::string> entitlements;
	std::vector<std::string> contexts;
	std::string signature;

public:
	std::string getId() {
		return id;
	}

	void setId(std::string id) {
		this->id = id;
	}

	bool hasEntitlement(std::string entitlement) {
		for (std::string allowed : entitlements) {
			if (allowed == entitlement) {
				return true;
			}
		}
		return false;
	}

	void setEntitlements(std::vector<std::string> entitlements) {
		this->entitlements = entitlements;
	}

	bool allowContext(std::string context) {
		for (std::string allowed : contexts) {
			if (allowed == context) {
				return true;
			}
		}
		return false;
	}

	void setContexts(std::vector<std::string> contexts) {
		this->contexts = contexts;
	}

	std::string getSignature() {
		return signature;
	}

	void setSignature(std::string signature) {
		this->signature = signature;
	}
};