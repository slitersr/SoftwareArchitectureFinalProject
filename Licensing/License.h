#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "LicensingDLL.h"

class dll License {
private:
	std::string raw;
	std::string id;
	std::vector<std::string> entitlements;
	std::vector<std::string> contexts;
	std::string signature;
	int currentContexts = 0;

public:
	std::string getRaw() {
		return raw;
	}

	void setRaw(std::string raw) {
		this->raw = raw;
	}

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

	std::vector<std::string> getEntitlements() {
		return entitlements;
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

	std::vector<std::string> getContexts() {
		return contexts;
	}

	std::string getSignature() {
		return signature;
	}

	void setSignature(std::string signature) {
		this->signature = signature;
	}

	int getCurrentContexts() {
		return currentContexts;
	}

	void setCurrentContexts(int currentContexts) {
		this->currentContexts = currentContexts;
	}

	void print() {
		std::cout << "======= LOADED LICENSE =======" << std::endl;
		std::cout << "ID: " << id << std::endl;
		std::cout << "Contexts: ";
		for (int i = 0; i < contexts.size(); i++) {
			std::cout << contexts[i];
			if (i == contexts.size() - 1) std::cout << std::endl;
			else std::cout << ", ";
		}
		std::cout << "Entitlements: ";
		for (int i = 0; i < entitlements.size(); i++) {
			std::cout << entitlements[i];
			if (i == entitlements.size() - 1) std::cout << std::endl;
			else std::cout << ", ";
		}
		std::cout << "Signature: " << signature << std::endl;
		std::cout << "==============================" << std::endl;
	}
};