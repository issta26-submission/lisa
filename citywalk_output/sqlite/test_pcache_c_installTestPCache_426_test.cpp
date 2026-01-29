/*
  This file provides a lightweight, non-GTest C++11 test suite for the focal
  function installTestPCache defined in test_pcache.c. The tests exercise
  installation and uninstallation of the PCACHE2 configuration as exercised by
  sqlite3_config, exercising both branches of the flag-switch and ensuring the
  configuration switch is observable via the public sqlite3_config interface.

  Notes:
  - We use a minimal in-house testing harness with non-terminating checks.
  - We rely on the public SQLite API (sqlite3_config) to observe changes to the PCache2 configuration.
  - We do not touch private static members; we infer behavior through the observable API.
  - We assume the environment provides sqlite3.h and links against the SQLite library.
  - The focal function is declared extern "C" to enable linkage from C++.
*/

#include <string.h>
#include <vector>
#include <assert.h>
#include <sstream>
#include <sqlite3.h>
#include <string>
#include <iostream>


// Forward declaration of the focal C function to be tested.
// It is defined in test_pcache.c (C source in the project under test).
extern "C" void installTestPCache(
    int installFlag,
    unsigned discardChance,
    unsigned prngSeed,
    unsigned highStress
);

// Lightweight non-terminating test assertion helper
struct TestContext {
    int total = 0;
    int passed = 0;
    int failed = 0;
    std::vector<std::string> logs;

    void expect(bool condition, const std::string& note) {
        ++total;
        if (condition) {
            ++passed;
        } else {
            ++failed;
            logs.push_back("ASSERTION FAILED: " + note);
        }
    }

    bool hasFailures() const { return failed > 0; }

    std::string summarize() const {
        std::ostringstream oss;
        oss << "Test Summary: total=" << total
            << ", passed=" << passed
            << ", failed=" << failed << "\n";
        for (const auto& s : logs) {
            oss << s << "\n";
        }
        return oss.str();
    }
};

// Helper function: fetch the current PCACHE2 configuration from sqlite
static sqlite3_pcache_methods2 getCurrentPCacheConfig() {
    sqlite3_pcache_methods2 cfg;
    // GETPCACHE2 writes into cfg
    if (sqlite3_config(SQLITE_CONFIG_GETPCACHE2, &cfg) != SQLITE_OK) {
        // Return a zero-initialized struct on error to indicate failure to observe state
        sqlite3_pcache_methods2 zero = {};
        return zero;
    }
    return cfg;
}

// Namespace to organize tests
namespace pcache_test {

void testInstallToggle(TestContext& ctx) {
    // Capture the default PCache2 configuration before any install
    sqlite3_pcache_methods2 before;
    if (sqlite3_config(SQLITE_CONFIG_GETPCACHE2, &before) != SQLITE_OK) {
        ctx.logs.push_back("ERROR: Failed to fetch initial PCACHE2 configuration.");
        ctx.total++;
        ctx.failed++;
        return;
    }

    // Step 1: Install PCache2
    installTestPCache(/*installFlag*/ 1, /*discardChance*/ 50, /*prngSeed*/ 12345, /*highStress*/ 0);

    // Step 2: Observe that the current configuration has changed
    sqlite3_pcache_methods2 afterInstall;
    if (sqlite3_config(SQLITE_CONFIG_GETPCACHE2, &afterInstall) != SQLITE_OK) {
        ctx.logs.push_back("ERROR: Failed to fetch PCACHE2 configuration after install.");
        ctx.total++;
        ctx.failed++;
        return;
    }
    bool changedOnInstall = (before.xCreate != afterInstall.xCreate);
    ctx.expect(changedOnInstall, "Installing PCache2 should replace the PCache with the test PCache (xCreate pointer change).");

    // Step 3: Uninstall PCache2 (restore to previous default saved in install path)
    installTestPCache(/*installFlag*/ 0, /*discardChance*/ 50, /*prngSeed*/ 12345, /*highStress*/ 0);

    sqlite3_pcache_methods2 afterUninstall;
    if (sqlite3_config(SQLITE_CONFIG_GETPCACHE2, &afterUninstall) != SQLITE_OK) {
        ctx.logs.push_back("ERROR: Failed to fetch PCACHE2 configuration after uninstall.");
        ctx.total++;
        ctx.failed++;
        return;
    }
    bool restoredOnUninstall = (afterUninstall.xCreate == before.xCreate);
    ctx.expect(restoredOnUninstall, "Uninstall should restore the original PCache2 (xCreate pointer equals pre-install).");
}

// This test ensures idempotency of repeated installs (the code path should be a no-op
// if installFlag matches the internal isInstalled flag).
void testInstallIdempotence(TestContext& ctx) {
    // Try to install for the first time
    sqlite3_pcache_methods2 beforeFirstInstall = getCurrentPCacheConfig();
    installTestPCache(/*installFlag*/ 1, /*discardChance*/ 0, /*prngSeed*/ 0, /*highStress*/ 1);

    sqlite3_pcache_methods2 afterFirstInstall = getCurrentPCacheConfig();
    ctx.expect(beforeFirstInstall.xCreate != afterFirstInstall.xCreate,
               "First install should change PCache2 (xCreate pointer should differ from previous).");

    // Run install again with the same flag; expect no change (no re-install)
    installTestPCache(/*installFlag*/ 1, /*discardChance*/ 0, /*prngSeed*/ 0, /*highStress*/ 1);
    sqlite3_pcache_methods2 afterSecondInstall = getCurrentPCacheConfig();

    ctx.expect(afterFirstInstall.xCreate == afterSecondInstall.xCreate,
               "Second install with same flag should be a no-op (PCache2 should remain the same).");
}

// Additional sanity test: uninstall when already uninstalled should be a no-op
void testUninstallNoOp(TestContext& ctx) {
    // Capture current state
    sqlite3_pcache_methods2 before = getCurrentPCacheConfig();

    // Ensure we are in a deployed state (uninstall once to be safe)
    installTestPCache(/*installFlag*/ 0, /*discardChance*/ 0, /*prngSeed*/ 0, /*highStress*/ 0);
    // Now packet the uninstall again (no-op)
    installTestPCache(/*installFlag*/ 0, /*discardChance*/ 0, /*prngSeed*/ 0, /*highStress*/ 0);

    sqlite3_pcache_methods2 afterNoOp = getCurrentPCacheConfig();
    ctx.expect(before.xCreate == afterNoOp.xCreate, "Uninstalling when already uninstalled should be a no-op and keep PCache2 unchanged.");
}

} // namespace pcache_test

int main() {
    using namespace pcache_test;

    TestContext ctx;

    // Run tests in sequence with explanatory comments
    // Test 1: Validate toggle install/uninstall changes observable PCache2 state
    testInstallToggle(ctx);

    // Test 2: Validate idempotence of repeat install calls
    testInstallIdempotence(ctx);

    // Test 3: Validate uninstall when already uninstalled is a no-op
    testUninstallNoOp(ctx);

    // Report results
    std::cout << ctx.summarize();

    return ctx.hasFailures() ? 1 : 0;
}