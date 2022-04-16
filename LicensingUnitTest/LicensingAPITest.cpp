#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"

#include "LicensingAPIImplementation.h"

TEST(APIImplementation, TestInitialization) {
	auto implementation = LicensingAPIImplementation();

	implementation.initialize();
}

TEST(APIImplementation, TestValidReserve) {
	auto implementation = LicensingAPIImplementation();

	auto result = implementation.reserve("ui", "solid_modeling");
	EXPECT_EQ(result, boost::none, "failed to reserve entitlement");
}


TEST(APIImplementation, TestInvalidReserveContext) {
	auto implementation = LicensingAPIImplementation();

	auto result = implementation.reserve("invalid_context", "solid_modeling");
	EXPECT_EQ(result.is_initialized(), true, "authorized invalid license");
}

TEST(APIImplementation, TestInvalidReserveEntitlement) {
	auto implementation = LicensingAPIImplementation();

	auto result = implementation.reserve("ui", "invalid_entitlement");
	EXPECT_EQ(result.is_initialized(), true, "authorized invalid license");
}

TEST(APIImplementation, TestMultiValidReserve) {
	auto implementation = LicensingAPIImplementation();

	auto result_first = implementation.reserve("ui", "solid_modeling");
	EXPECT_EQ(result_first, boost::none, "failed to reserve entitlement");

	auto result_second = implementation.reserve("ui", "part_operations");
	EXPECT_EQ(result_second, boost::none, "failed to reserve entitlement");
}

TEST(APIImplementation, TestMultiInvalidReserve) {
	auto implementation = LicensingAPIImplementation();

	auto result_first = implementation.reserve("ui", "solid_modeling");
	EXPECT_EQ(result_first, boost::none, "failed to reserve entitlement");

	auto result_second = implementation.reserve("ui", "solid_modeling");
	EXPECT_EQ(result_second.is_initialized(), true, "authorized duplicated license");
}

TEST(APIImplementation, TestUnreserve) {
	auto implementation = LicensingAPIImplementation();

	implementation.reserve("ui", "solid_modeling");
	implementation.unreserve("ui", "solid_modeling");
}

TEST(APIImplementation, TestShutdown) {
	auto implementation = LicensingAPIImplementation();

	implementation.shutdown();
}

std::ostream& operator<<(std::ostream& os, const LicenseReserveFailure& a)
{
	os << "STUB PRINT RESERVE FAILURE";
	return os;
}