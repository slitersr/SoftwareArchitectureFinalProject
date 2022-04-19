#pragma once

#include <iostream>
#include <filesystem>
#include <string>

//#include "boost/algorithm/hex.hpp"
#include "openssl/hmac.h"
#include "openssl/evp.h"
#include "LicenseLoaderImplementation.h"

std::vector<License> LicenseLoaderImplementation::loadLicenses() {
	for (const auto& path : std::filesystem::directory_iterator("./")) {
		std::cout << path << std::endl;
	}
	std::cout << "[stub] Loading licenses from file" << std::endl;
	return std::vector<License>();
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