#pragma once

#include <string>
#include <boost/optional.hpp>
#include "LicensingDLL.h"
#include "LicenseReserveFailure.h"

class dll LicensingAPI {
public:
	/**
	* Initilizies the licensing system and attempts to load all local licenses.
	* Invalid licenses will not be loaded.
	*/
	virtual void initialize() = 0;
	/**
	* Attempts to reserve a permit for an entitlement in a given context using
	* some available local license. 
	*/
	virtual boost::optional<LicenseReserveFailure> reserve(std::string context, std::string entitlement) = 0;
	/**
	* Used when unreserving a previously reserved license for an entitlement
	* in a given context.
	* 
	* Returns true if the license was unreserved. Returns false if the target
	* license was not reserved.
	*/
	virtual bool unreserve(std::string context, std::string entitlement) = 0;
	/**
	* Used when an entitlement is expected to be reserved at this point in the application's
	* control flow, and terminates the program if this is not the case, such as when
	* previous reservation errors were not correctly handled.
	*/
	virtual void check(std::string entitlement) = 0;
	/**
	* Unreserves all outstanding entitlements and shuts down the licensing system.
	*/
	virtual void shutdown() = 0;
};