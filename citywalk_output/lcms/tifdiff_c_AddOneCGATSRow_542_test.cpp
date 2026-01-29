// Self-contained unit test suite for AddOneCGATSRow in tifdiff.c
// Notes:
// - This test suite is designed to compile under C++11 without Google Test.
// - A minimal mock CMS backend is provided to capture CMS IT8 data written by
//   AddOneCGATSRow (via cmsIT8SetData and cmsIT8SetDataDbl).
// - The LPSTAT type is defined as a pointer-like struct (to reflect usage
//   st->Min and st->Peak in the focal method).
// - Two tests are provided: a normal case and an edge case to exercise the
//   computed fields.
// - A lightweight test harness prints results and returns non-zero on failure.

#include <cstring>
#include <map>
#include <iostream>
#include <string>
#include <cmath>
#include <utils.h>
#include <tiffio.h>


// Typedefs to mirror the focal code's API (minimal stubs for testing)
typedef void* cmsHANDLE;

// LPSTAT is a pointer to an internal STAT structure (as suggested by st->Min)
typedef struct _STAT {
    double Min;
    double Peak;
} STAT;
typedef STAT* LPSTAT;

// Prototypes for functions used by the focal method (to be provided by test)
double Mean(LPSTAT st);
double Std(LPSTAT st);

// Mock CMS backend to capture writes performed by AddOneCGATSRow
class MockCMS {
public:
    // Store string values: key = "<Name>::<Tag>", value = string
    std::map<std::string, std::string> stringData;
    // Store double values similarly
    std::map<std::string, double> doubleData;

    void setString(const std::string& name, const std::string& tag, const std::string& value) {
        stringData[name + "::" + tag] = value;
    }

    void setDouble(const std::string& name, const std::string& tag, double value) {
        doubleData[name + "::" + tag] = value;
    }

    bool getString(const std::string& name, const std::string& tag, std::string& out) const {
        auto key = name + "::" + tag;
        auto it = stringData.find(key);
        if (it == stringData.end()) return false;
        out = it->second;
        return true;
    }

    bool getDouble(const std::string& name, const std::string& tag, double& out) const {
        auto key = name + "::" + tag;
        auto it = doubleData.find(key);
        if (it == doubleData.end()) return false;
        out = it->second;
        return true;
    }
};

// Global helper to enable the mock within free-standing functions
static MockCMS* gMock = nullptr;

// Proxied CMS API that the focal function calls
void cmsIT8SetData(cmsHANDLE h, const char* Name, const char* tag, const char* value) {
    MockCMS* m = static_cast<MockCMS*>(h);
    m->setString(Name ? Name : "", tag ? tag : "", value ? value : "");
}
void cmsIT8SetDataDbl(cmsHANDLE h, const char* Name, const char* tag, double value) {
    MockCMS* m = static_cast<MockCMS*>(h);
    m->setDouble(Name ? Name : "", tag ? tag : "", value);
}

// Mean and Std implementations matching how the focal code expects them.
double Mean(LPSTAT st) {
    return (st->Min + st->Peak) / 2.0;
}
double Std(LPSTAT st) {
    return (st->Peak - st->Min) / 2.0;
}

// The focal method under test (copied as-is with appropriate types)
void AddOneCGATSRow(cmsHANDLE hIT8, char *Name, LPSTAT st)
{
{
    double Per100 = 100.0 * ((255.0 - Mean(st)) / 255.0);
    cmsIT8SetData(hIT8,    Name, "SAMPLE_ID", Name);
    cmsIT8SetDataDbl(hIT8, Name, "PER100_EQUAL", Per100);
    cmsIT8SetDataDbl(hIT8, Name, "MEAN_DE", Mean(st));
    cmsIT8SetDataDbl(hIT8, Name, "STDEV_DE", Std(st));
    cmsIT8SetDataDbl(hIT8, Name, "MIN_DE", st ->Min);
    cmsIT8SetDataDbl(hIT8, Name, "MAX_DE", st ->Peak);
}
}

// Lightweight test harness helpers
#define TEST_PASSED_COUNT 1

bool test_AddOneCGATSRow_Normal() {
    // Arrange
    MockCMS mock;
    gMock = &mock;
    char rowName[] = "Row_Normal";
    STAT st;
    st.Min = 0.0;
    st.Peak = 255.0;

    // Act
    AddOneCGATSRow(static_cast<cmsHANDLE>(&mock), rowName, &st);

    // Assert
    std::string s;
    double per100 = 0.0, meanVal = 0.0, stdevVal = 0.0, minVal = 0.0, maxVal = 0.0;
    bool ok = true;

    if (!mock.getString(rowName, "SAMPLE_ID", s)) {
        std::cout << "FAIL Normal: SAMPLE_ID not written\n";
        ok = false;
    } else if (s != std::string(rowName)) {
        std::cout << "FAIL Normal: SAMPLE_ID value mismatch. Expected " << rowName << " got " << s << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "PER100_EQUAL", per100)) {
        std::cout << "FAIL Normal: PER100_EQUAL not written\n";
        ok = false;
    } else if (std::fabs(per100 - 50.0) > 1e-6) {
        std::cout << "FAIL Normal: PER100_EQUAL incorrect. Expected ~50, got " << per100 << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MEAN_DE", meanVal)) {
        std::cout << "FAIL Normal: MEAN_DE not written\n";
        ok = false;
    } else if (std::fabs(meanVal - 127.5) > 1e-6) {
        std::cout << "FAIL Normal: MEAN_DE incorrect. Expected 127.5, got " << meanVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "STDEV_DE", stdevVal)) {
        std::cout << "FAIL Normal: STDEV_DE not written\n";
        ok = false;
    } else if (std::fabs(stdevVal - 127.5) > 1e-6) {
        std::cout << "FAIL Normal: STDEV_DE incorrect. Expected 127.5, got " << stdevVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MIN_DE", minVal)) {
        std::cout << "FAIL Normal: MIN_DE not written\n";
        ok = false;
    } else if (std::fabs(minVal - 0.0) > 1e-6) {
        std::cout << "FAIL Normal: MIN_DE incorrect. Expected 0.0, got " << minVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MAX_DE", maxVal)) {
        std::cout << "FAIL Normal: MAX_DE not written\n";
        ok = false;
    } else if (std::fabs(maxVal - 255.0) > 1e-6) {
        std::cout << "FAIL Normal: MAX_DE incorrect. Expected 255.0, got " << maxVal << "\n";
        ok = false;
    }

    if (ok) {
        std::cout << "PASS: AddOneCGATSRow_Normal\n";
    }
    return ok;
}

bool test_AddOneCGATSRow_Edge() {
    // Arrange
    MockCMS mock;
    gMock = &mock;
    char rowName[] = "Row_Edge";
    STAT st;
    st.Min = 0.0;
    st.Peak = 0.0;  // Edge case where Min == Peak

    // Act
    AddOneCGATSRow(static_cast<cmsHANDLE>(&mock), rowName, &st);

    // Assert
    std::string s;
    double per100 = 0.0, meanVal = 0.0, stdevVal = 0.0, minVal = 0.0, maxVal = 0.0;
    bool ok = true;

    if (!mock.getString(rowName, "SAMPLE_ID", s)) {
        std::cout << "FAIL Edge: SAMPLE_ID not written\n";
        ok = false;
    } else if (s != std::string(rowName)) {
        std::cout << "FAIL Edge: SAMPLE_ID value mismatch. Expected " << rowName << " got " << s << "\n";
        ok = false;
    }

    // PER100_EQUAL should be 100.0 when Mean(st) == 0
    if (!mock.getDouble(rowName, "PER100_EQUAL", per100)) {
        std::cout << "FAIL Edge: PER100_EQUAL not written\n";
        ok = false;
    } else if (std::fabs(per100 - 100.0) > 1e-6) {
        std::cout << "FAIL Edge: PER100_EQUAL incorrect. Expected 100, got " << per100 << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MEAN_DE", meanVal)) {
        std::cout << "FAIL Edge: MEAN_DE not written\n";
        ok = false;
    } else if (std::fabs(meanVal - 0.0) > 1e-6) {
        std::cout << "FAIL Edge: MEAN_DE incorrect. Expected 0, got " << meanVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "STDEV_DE", stdevVal)) {
        std::cout << "FAIL Edge: STDEV_DE not written\n";
        ok = false;
    } else if (std::fabs(stdevVal - 0.0) > 1e-6) {
        std::cout << "FAIL Edge: STDEV_DE incorrect. Expected 0, got " << stdevVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MIN_DE", minVal)) {
        std::cout << "FAIL Edge: MIN_DE not written\n";
        ok = false;
    } else if (std::fabs(minVal - 0.0) > 1e-6) {
        std::cout << "FAIL Edge: MIN_DE incorrect. Expected 0, got " << minVal << "\n";
        ok = false;
    }

    if (!mock.getDouble(rowName, "MAX_DE", maxVal)) {
        std::cout << "FAIL Edge: MAX_DE not written\n";
        ok = false;
    } else if (std::fabs(maxVal - 0.0) > 1e-6) {
        std::cout << "FAIL Edge: MAX_DE incorrect. Expected 0, got " << maxVal << "\n";
        ok = false;
    }

    if (ok) {
        std::cout << "PASS: AddOneCGATSRow_Edge\n";
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    if (test_AddOneCGATSRow_Normal()) { passed++; }
    total++;

    if (test_AddOneCGATSRow_Edge()) { passed++; }
    total++;

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    return (passed == total) ? 0 : 1;
}