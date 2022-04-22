#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/optional.hpp"
#include "boost/optional/optional_io.hpp"

#include "LicensingAPIImplementation.h"
#include "StubLicenseLoader.h"
#include "MockLicenseClient.h"

License license(std::string id, std::vector<std::string> contexts, std::vector<std::string> entitlements) {
	License result;
	result.setId(id);
	result.setEntitlements(entitlements);
	result.setContexts(contexts);
	return result;
}

static License basic = license("test_basic", { "ui", "automation" }, { "basic" });
static License modeling = license("test_modeling", { "ui", "automation" }, { "solid_modeling" });
static License operations = license("test_operations", { "ui", "automation" }, { "part_operations" });

LicensingAPIImplementation create(MockLicenseClient& client, std::vector<License> licenses) {
	for (License& license : licenses) {
		license.setCurrentContexts(0);
	}
	auto result = LicensingAPIImplementation(&client, new StubLicenseLoader(licenses));
	result.initialize();
	return result;
}

TEST(LicenseAPITest, TestUnreserveNotReservedFail) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	auto result = api.unreserve("anything", "anything");
	EXPECT_FALSE(result);
}

TEST(LicenseAPITest, TestReserveNoneInstalledFail) {
	MockLicenseClient client;
	auto api = create(client, {  });

	auto result = api.reserve("anything", "anything");
	EXPECT_TRUE(result.is_initialized());
}

TEST(LicenseAPITest, TestDisallowedEntitlementFail) {
	MockLicenseClient client;
	auto api = create(client, { basic, modeling, operations });

	auto result = api.reserve("ui", "anything");
	EXPECT_TRUE(result.is_initialized());
}

TEST(LicenseAPITest, TestDisallowedContextFail) {
	MockLicenseClient client;
	auto api = create(client, { basic, modeling, operations });

	auto result = api.reserve("anything", "basic");
	EXPECT_TRUE(result.is_initialized());
}

TEST(LicenseAPITest, TestReserveOnceSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(1);

	auto result = api.reserve("ui", "basic");
	EXPECT_FALSE(result.is_initialized());
}

TEST(LicenseAPITest, TestUnreserveReserveOnceSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_basic", "basic")).Times(1);

	auto result = api.reserve("ui", "basic");
	EXPECT_FALSE(result.is_initialized());

	auto unreserved = api.unreserve("ui", "basic");
	EXPECT_TRUE(unreserved);
}

TEST(LicenseAPITest, TestUnreserveReserveTwiceSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(2);
	EXPECT_CALL(client, unreserveEntitlement("test_basic", "basic")).Times(1);

	auto first = api.reserve("ui", "basic");
	EXPECT_FALSE(first.is_initialized());

	auto unreserved = api.unreserve("ui", "basic");
	EXPECT_TRUE(unreserved);

	auto second = api.reserve("ui", "basic");
	EXPECT_FALSE(second.is_initialized());
}

TEST(LicenseAPITest, TestTwiceUnreserveReserveTwiceSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(2);
	EXPECT_CALL(client, unreserveEntitlement("test_basic", "basic")).Times(2);

	auto first = api.reserve("ui", "basic");
	EXPECT_FALSE(first.is_initialized());

	auto unreserved = api.unreserve("ui", "basic");
	EXPECT_TRUE(unreserved);

	auto second = api.reserve("ui", "basic");
	EXPECT_FALSE(second.is_initialized());

	auto unreservedAgain = api.unreserve("ui", "basic");
	EXPECT_TRUE(unreservedAgain);
}

TEST(LicenseAPITest, TestReserveSameTwiceSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(1);

	auto first = api.reserve("ui", "basic");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("ui", "basic");
	EXPECT_FALSE(second.is_initialized());
}

TEST(LicenseAPITest, TestUnreserveReserveSameStackedSuccess) {
	MockLicenseClient client;
	auto api = create(client, { basic });

	EXPECT_CALL(client, reserveEntitlement("test_basic", "basic")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_basic", "basic")).Times(1);

	auto first = api.reserve("ui", "basic");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("ui", "basic");
	EXPECT_FALSE(second.is_initialized());

	EXPECT_TRUE(api.unreserve("ui", "basic"));
	EXPECT_TRUE(api.unreserve("ui", "basic"));
}

TEST(LicenseAPITest, TestUnreserveReserveSameStackedContextsSuccess) {
	MockLicenseClient client;
	auto api = create(client, { modeling, operations });

	EXPECT_CALL(client, reserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_modeling", "solid_modeling")).Times(1);

	auto first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));
	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));
}

TEST(LicenseAPITest, TestUnreserveReserveMultiStackedFullUnreserveContextsFIFOSuccess) {
	MockLicenseClient client;
	auto api = create(client, { modeling, operations });

	EXPECT_CALL(client, reserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, reserveEntitlement("test_operations", "part_operations")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_operations", "part_operations")).Times(1);

	auto first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	auto third = api.reserve("ui", "part_operations");
	EXPECT_FALSE(third.is_initialized());

	auto fourth = api.reserve("automation", "part_operations");
	EXPECT_FALSE(fourth.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "part_operations"));
	EXPECT_TRUE(api.unreserve("ui", "part_operations"));
	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));
	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));
}

TEST(LicenseAPITest, TestUnreserveReserveMultiStackedFullUnreserveContextsInterleaveSuccess) {
	MockLicenseClient client;
	auto api = create(client, { modeling, operations });

	EXPECT_CALL(client, reserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, reserveEntitlement("test_operations", "part_operations")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_operations", "part_operations")).Times(1);

	auto first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));

	auto third = api.reserve("ui", "part_operations");
	EXPECT_FALSE(third.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));

	auto fourth = api.reserve("automation", "part_operations");
	EXPECT_FALSE(fourth.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "part_operations"));
	EXPECT_TRUE(api.unreserve("ui", "part_operations"));
}

TEST(LicenseAPITest, TestUnreserveTwiceReserveMultiStackedFullUnreserveContextsInterleaveSuccess) {
	MockLicenseClient client;
	auto api = create(client, { modeling, operations });

	EXPECT_CALL(client, reserveEntitlement("test_modeling", "solid_modeling")).Times(2);
	EXPECT_CALL(client, unreserveEntitlement("test_modeling", "solid_modeling")).Times(1);
	EXPECT_CALL(client, reserveEntitlement("test_operations", "part_operations")).Times(1);
	EXPECT_CALL(client, unreserveEntitlement("test_operations", "part_operations")).Times(1);

	auto first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));

	auto third = api.reserve("ui", "part_operations");
	EXPECT_FALSE(third.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));

	auto fourth = api.reserve("automation", "part_operations");
	EXPECT_FALSE(fourth.is_initialized());

	auto fifth = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(fifth.is_initialized());

	auto sixth = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(sixth.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "part_operations"));
	EXPECT_TRUE(api.unreserve("ui", "part_operations"));
}

TEST(LicenseAPITest, TestFullTwiceUnreserveTwiceReserveMultiStackedFullUnreserveContextsInterleaveSuccess) {
	MockLicenseClient client;
	auto api = create(client, { modeling, operations });

	EXPECT_CALL(client, reserveEntitlement("test_modeling", "solid_modeling")).Times(2);
	EXPECT_CALL(client, unreserveEntitlement("test_modeling", "solid_modeling")).Times(2);
	EXPECT_CALL(client, reserveEntitlement("test_operations", "part_operations")).Times(2);
	EXPECT_CALL(client, unreserveEntitlement("test_operations", "part_operations")).Times(2);

	auto first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	auto second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));

	auto third = api.reserve("ui", "part_operations");
	EXPECT_FALSE(third.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));

	auto fourth = api.reserve("automation", "part_operations");
	EXPECT_FALSE(fourth.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "part_operations"));
	EXPECT_TRUE(api.unreserve("ui", "part_operations"));

	first = api.reserve("ui", "solid_modeling");
	EXPECT_FALSE(first.is_initialized());

	second = api.reserve("automation", "solid_modeling");
	EXPECT_FALSE(second.is_initialized());

	third = api.reserve("ui", "part_operations");
	EXPECT_FALSE(third.is_initialized());

	fourth = api.reserve("automation", "part_operations");
	EXPECT_FALSE(fourth.is_initialized());

	EXPECT_TRUE(api.unreserve("automation", "part_operations"));
	EXPECT_TRUE(api.unreserve("ui", "part_operations"));
	EXPECT_TRUE(api.unreserve("automation", "solid_modeling"));
	EXPECT_TRUE(api.unreserve("ui", "solid_modeling"));
}

std::ostream& operator<<(std::ostream& os, const LicenseReserveFailure& m) {
	return os << "LicenseReserveFailure(context=" << m.getContext() << ",entitlement=" << m.getEntitlement() << ",allowContinue=" << m.getAllowContinue() << ",message=" << m.getMessage() << ")";
}
