# Licensing System Conclusion

Total time spent: 20 hours.

## Requirements

This project fully accomplished the requirements.

### Design Document

We started with a detailed design document, found in the `DESIGN_DOCUMENT.md` file. Since so much care was taken with the initial design, the implementation was easy and very few changes had to be made.

### Read Licenses From File

Our licensing system reads license files from the "licenses" directory. Each license has a `.dat` file and an associated `.signature` file. The signature file contains an HMAC signature of the data in the license file signed with a secret key. The licensing system uses this signature to verify that the user has not tampered with the license file. We implemented this functionality by installing the OppenSSH library using the NuGet package manager. OpenSSH has functions for validating an HMAC signature with minimal additional code.

Three example licenses are included in the "licenses" directory. Here is an example of the "basic" license.

`basic_license.dat`

```
test_license
ui,automation
basic
```

The first line is the ID of the license. The second line contains a comma separated list of contexts in which this license may be used, and the third line contains a comma separated list of entitlements that may be reserved for this license.

`basic_license.dat.signature`

```
d18402bad8555d6fff99b1c1653470bcb49a539e721aae666168b84cc6dfb5a3
```

This is the HMAC signature for the above license data signed with our secret key. You can't modify the license file because the signature won't match, and you can't forge the signature because you don't know the secret key. Try it! The licensing system will not load a license with an invalid signature.

The code for this portion of the project can be found in `Licensing/LicenseLoaderImplementation.cpp`.

### Licensing Library

Our licensing library supports all of the required feature. It has methods for initialization, which loads the licenses from disk, shutdown, which prevents any new entitlements from being reserved, and of course, reserving (checking out) and unreserving (checking in) entitlements. The library is contained in a DLL file, with headers that export the appropriate symbols. The library is accessed using a singleton pattern. A static method returning a static shared pointer to the library's API is available in the `Licensing.h` header. This ensures that the internal state of the licensing system is consistent, since multiple copies of the API would prevent the library from knowing about all of the entitlements that are reserved. Additionally, a `check` method was added to the API that allows internal code to ensure that a designated entitlement has been reserved previously. If the entitlement is not reserved, messages will be printed to the console and the program will terminate. This is designed to protect internal code that may be accessed from automation APIs. Users are expected to reserve the necessary licenses, but we can't trust them to do that. These randomly inserted checks will prevent users of the automation APIs from being able to use internal code without reserving the appropriate entitlements.

As an example, the licensing library was integrated into the UI library of the main application. The integration looks like this.

Initializing the library:

```cpp
void UI::Init()
{
	CoreSession::GetInstance().Licensing()->initialize();

	CoreSession::GetInstance().SetupDefaultObservers();
....
```

Reserving a license prior to starting the UI:

```cpp
void UI::StartGUILoop()
{
	auto result = CoreSession::GetInstance().Licensing()->reserve("ui", "basic");
	if (result.is_initialized()) {
		CoreSession::GetInstance().CreateMessage("Unable to reserve basic UI license.");
		CoreSession::GetInstance().CreateMessage(result.get().getMessage());
		CoreSession::GetInstance().CreateMessage("Not starting UI.");
		return;
	}

	int WorkFlowToRun = 1;
....
```

Unreserving the UI license and shutting down the library on UI shutdown:

```cpp
void UI::ShutDownGUILoop()
{
	CoreSession::GetInstance().Licensing()->unreserve("ui", "basic");

	observer4->RemoveMeFromTheList();
	observer5->RemoveMeFromTheList();

	CoreSession::GetInstance().Licensing()->shutdown();
}
....
```

Randomly inserted compliance checks as discussed above:

```cpp
    //Setup Journaling File
	SetJournalingLangauge(JournalingLanguage::CPP);
	StartJournaling(BasePath() + "\\JournaledCPPFileProject\\SampleJournal.txt");

	// Randomly inserted check. Terminate the program if the right entitlements aren't reserved.
	// Prevents automation code from using our internal code for free.
	CoreSession::GetInstance().Licensing()->check("part_operations");
....
```

### Contexts

The licensing library internally keeps track of which licenses have been reserved under which contexts. The library will attempt to reserve an entitlement from the remote server (using the license client) the first time that any given entitlement is reserved for a context. That license can be reserved for any number of additional contexts that it supports, and the licensing systems knows not to communicate with the server since the appropriate entitlement is already reserved. Once an entitlement is checked in for a license in _all_ of the contexts that it was checked out in, the licensing system will realize that there are no outstanding references to the license and unreserve the entitlement for that license on the remote server.

There is extra complexity from the fact that licenses are only authorized for specific contexts. It is possible for an entitlement to be reserved in a specific context, but the license that provided that entitlement doesn't support the requested context. The licensing library is smart enough to try to find a different installed license that is compatible with the requested context and reserve it from the remote server. Even though it is for the same entitlement, this second reservation is reference counted independently of the first, and will be unreserved at the appropriate time just as the first license. Any number of licenses for a single entitlement may be reserved at any time, since there is no limit on the number of contexts. The system will _always_ prefer to reuse an entitlement that has already been reserved for a license rather than checking out a different license, when the contexts are compatible.

The behavior of this interleaved reserving and unreserving mechanism is rigorously tested in our test suite and leverages mocks to ensure that the remote server is accessed the correct number of times, even in complex scenarios.

The code for this part of the project is located in `LicensingAPIImplementation.cpp`.

### Testing

We created an extensive test suite with over 350 lines of tests. All functionality and combinations of reserving and unreserving licenses that we could think of were tested. The tests use stubs and mocks when appropriate. GMock is used to verify the behavior of mocks by tracking ensuring that methods are called exactly as often as expected. Stubs are used when behavior validation isn't necessary.

The test suite is located in the `LicensingUnitTest` folder.

## Future Enhancements

The most important feature is the ability to communicate with a remote server to ensure that the appropriate number of licenses are reserved for a specific user across many computers. The license file signing adds some security, but it does not prevent the user from copying the file to many different computers without paying. Our `LicenseClient` API is designed for connecting to an external server, but the implementation included with this project does not do that, and rather assumes that any entitlement that is valid for a license is always available to be reserved.
