#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

#include "openssl/hmac.h"
#include "openssl/evp.h"
#include "LicenseLoaderImplementation.h"

std::vector<License> LicenseLoaderImplementation::loadLicenses() {
	auto result = std::vector<License>();
	for (const auto& file : std::filesystem::directory_iterator("../licenses")) {
		try {
			if (!file.is_regular_file()) continue;
			if (!file.path().string().ends_with(".dat")) continue;
			std::ifstream data_stream(file.path());
			std::stringstream data_buffer;
			data_buffer << data_stream.rdbuf();
			std::ifstream signature_stream(file.path().string().append(".signature"));
			std::stringstream signature_buffer;
			signature_buffer << signature_stream.rdbuf();
			License license;
			license.setRaw(data_buffer.str());
			license.setSignature(signature_buffer.str());
			std::string line;
			std::getline(data_buffer, line);
			license.setId(line);
			std::getline(data_buffer, line);
			std::stringstream context_stream(line);
			std::string context;
			auto contexts = std::vector<std::string>();
			while (std::getline(context_stream, context, ',')) {
				contexts.push_back(context);
			}
			std::getline(data_buffer, line);
			std::stringstream entitlement_stream(line);
			std::string entitlement;
			license.setContexts(contexts);
			auto entitlements = std::vector<std::string>();
			while (std::getline(entitlement_stream, entitlement, ',')) {
				entitlements.push_back(entitlement);
			}
			license.setEntitlements(entitlements);

			if (validateSignature(license)) {
				license.print();
				result.push_back(license);
			}
		}
		catch (const std::exception&) {
			std::cout << "Skipping invalid or corrupt license file: " << file << std::endl;
			std::cout << "Please obtain a new copy of this license and place it in the 'licenses' directory" << std::endl;
		}
	}

	int entitlements = 0, contexts = 0;
	for (License& license : result) {
		entitlements += license.getEntitlements().size();
		contexts += license.getContexts().size();
	}
	std::cout << "Loaded " << result.size() << " licenses usable in " << contexts << " contexts containing " << entitlements << " total entitlements." << std::endl;

	return result;
}


bool LicenseLoaderImplementation::validateSignature(License license) {
	char* key = _strdup("4ea1ba54b46ee441616d5ad6a058e4bc");
	int keyLength = strlen(key);
	unsigned char* data = (unsigned char*)_strdup(license.getRaw().c_str());
	int dataLength = strlen((char*)data);

	unsigned char* result = nullptr;
	unsigned int resultLength = -1;

	result = HMAC(EVP_sha256(), key, keyLength, data, dataLength, result, &resultLength);

	std::string target = license.getSignature();
	std::string hex;
	char const chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int index = 0; index < resultLength; index++) {
		hex += chars[(result[index] & 0xF0) >> 4];
		hex += chars[(result[index] & 0x0F) >> 0];
	}


	if (target.length() != hex.length()) {
		std::cout << "License signature length mismatch for license with ID '" << license.getId() << "'. Expected " << target.length() << " but found " << hex.length() << "." << std::endl;
		return false;
	}

	for (auto index = 0; index < hex.length(); index++) {
		if (target[index] != hex[index]) {
			std::cout << "License signature mismatch for license with ID '" << license.getId() << "'. Expected " << target << " but found " << hex << "." << std::endl;
			return false;
			return false;
		}
	}

	return true;
}