#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"

#include "License.h"
#include "LicenseLoaderImplementation.h"

LicenseLoaderImplementation create() {
	return LicenseLoaderImplementation();
}

TEST(LicenseLoaderTest, TestValidateCorrectSignature) {
	auto loader = create();

	auto license = License();
	license.setRaw("this is some test data");
	license.setSignature("04307d0695f3945c42382ddc18af03fd6a72d46b90fe69aa0d1ccddfc9dbd6da");

	auto result = loader.validateSignature(license);
	EXPECT_TRUE(result);
}

TEST(LicenseLoaderTest, TestValidateIncorrectSignature) {
	auto loader = create();

	auto license = License();
	license.setRaw("this is some test data");
	license.setSignature("invalid");
	license.setId("License with invalid signature");

	auto result = loader.validateSignature(license);
	EXPECT_FALSE(result);
}

TEST(LicenseLoaderTest, TestValidateIncorrectSignatureData) {
	auto loader = create();

	auto license = License();
	license.setRaw("this is some modified test data");
	license.setSignature("04307d0695f3945c42382ddc18af03fd6a72d46b90fe69aa0d1ccddfc9dbd6da");
	license.setId("License with invalid signature");

	auto result = loader.validateSignature(license);
	EXPECT_FALSE(result);
}