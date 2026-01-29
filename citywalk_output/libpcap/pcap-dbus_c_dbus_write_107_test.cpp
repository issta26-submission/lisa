// Test suite for dbus_write(pcap_t *handle, const void *buf, int size)
// This test is designed to be used in a C++11 environment without GoogleTest.
// It relies on the project’s internal headers (pcap-int.h, pcap-dbus.h) and
// provides lightweight C-callable mocks for the DBus APIs used by dbus_write.
// The tests are executed via a simple main() entry point.

#include <string.h>
#include <cassert>
#include <time.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>


// Include project headers to get the correct type definitions for pcap_t
// and the pcap-dbus private structures used by dbus_write.

// Forward declarations for the DBus API. In the real project, these come from
// <dbus/dbus.h>, but for unit testing we provide lightweight C-linkage mocks.
// We assume the build environment will use real DBus types from the project headers.
extern "C" {

// Global switch to control whether dbus_message_demarshal should fail or succeed.
// The test harness toggles this to exercise true/false branches of the demarshal call.
bool g_demarshal_should_fail = false;

// Forward-declare fake DBus types to be used by mocks.
// These types are opaque to the test machinery; dbus_write only requires pointers
// to them and their associated functions.
typedef struct DBusMessage DBusMessage;
typedef struct DBusConnection DBusConnection;

// Mocked DBusError structure is provided by the project's dbus.h,
// but we assume its layout includes a 'message' field used by dbus_write
// when reporting demarshal failures.
}

// Internal helper structure to emulate a DBus connection in tests.
// We avoid relying on the real DBus internals; instead we fake a connection
// object whose state can be inspected by the tests (sent/ flushed).
struct FakeDBusConnection {
    bool sent;
    bool flushed;
};

// We will reinterpret_cast the address of FakeDBusConnection to DBusConnection*
// before passing to dbus_write. This is safe in unit tests since we only
// use the pointer as an opaque handle to record calls from the mocks below.

static FakeDBusConnection g_fake_conn;

// Simple sentinel to represent a fake DBusMessage (the actual content is not
// important for the test; we just need a non-null pointer to indicate success).
static DBusMessage* const kFakeDBusMessageSent = reinterpret_cast<DBusMessage*>(0x1);

// Lightweight helper to reset fake connection flags.
static void reset_fake_conn() {
    g_fake_conn.sent = false;
    g_fake_conn.flushed = false;
}

// Implementations of the mocked DBus APIs with C linkage so they can be
// found by the linker when dbus_write is compiled along with the real code.
// We rely on the fact that the project only uses these functions for the side
// effects (state updates) and not for any real DBus semantics.

extern "C" {

// Provide a DBusMessage* returned by dbus_message_demarshal on success.
// On failure, set the error message to a known string via the DBusError object.
DBusMessage* dbus_message_demarshal(const void* buf, int size, DBusError* error) {
    if (g_demarshal_should_fail) {
        // Simulate a demarshal failure and populate the error message.
        // The real code uses error.message to print into errbuf.
        if (error != nullptr) {
            // Point to a string literal to ensure lifetime
            error->message = "mock error";
        }
        return nullptr;
    }
    // Success path: return a non-null sentinel message
    (void)buf; // suppress unused warning in case size is optimized out
    (void)size;
    return kFakeDBusMessageSent;
}

// Free a DBusError (no-op for test purposes)
void dbus_error_free(DBusError* error) {
    // No dynamic allocation in the test, nothing to free.
    (void)error;
}

// Send a DBusMessage on a DBusConnection (record that it was sent)
void dbus_connection_send(DBusConnection* conn, DBusMessage* msg, void* timeout) {
    (void)timeout;
    // Interpret the opaque pointer as our FakeDBusConnection storage
    FakeDBusConnection* f = reinterpret_cast<FakeDBusConnection*>(conn);
    if (f) {
        f->sent = true;
        (void)msg; // message content not used in test
    }
}

// Flush a DBusConnection (record flush)
void dbus_connection_flush(DBusConnection* conn) {
    FakeDBusConnection* f = reinterpret_cast<FakeDBusConnection*>(conn);
    if (f) {
        f->flushed = true;
    }
}

// Unreference a DBusMessage (no special handling needed in test)
void dbus_message_unref(DBusMessage* msg) {
    (void)msg;
}

} // extern "C"

// Helper test macros for simple test naming and result reporting
#define TEST_PASS printf("[PASS] %s\n", __func__);
#define TEST_FAIL printf("[FAIL] %s\n", __func__);

// Test 1: dbus_write should succeed when demarshal returns a valid message
// This validates the "success" branch of the function: it should call
// dbus_connection_send, dbus_connection_flush and return 0.
static void test_dbus_write_success() {
    // Prepare a legitimate handle with priv->conn pointing to our fake connection
    pcap_t handle;
    memset(&handle, 0, sizeof(handle));

    // The private per-file structure defined by the library
    pcap_dbus priv;
    memset(&priv, 0, sizeof(priv));

    // Link the fake connection into the pcap_dbus private structure
    priv.conn = reinterpret_cast<DBusConnection*>(&g_fake_conn);

    // Attach private data to the public handle
    handle.priv = &priv;

    // Reset imported fake connection state
    reset_fake_conn();

    // Ensure demarshal succeeds
    g_demarshal_should_fail = false;

    // Prepare a sample buffer
    const char test_buf[] = "valid_message";
    int test_size = static_cast<int>(std::strlen(test_buf));

    // Call the focal function
    int ret = dbus_write(&handle, test_buf, test_size);

    // Assertions
    bool passed = (ret == 0) && g_fake_conn.sent && g_fake_conn.flushed;
    if (passed) {
        TEST_PASS;
    } else {
        TEST_FAIL;
        std::cerr << "dbus_write did not perform as expected on success path.\n";
        if (ret != 0) std::cerr << "Expected return 0, got " << ret << "\n";
        if (!g_fake_conn.sent) std::cerr << "dbus_connection_send was not invoked.\n";
        if (!g_fake_conn.flushed) std::cerr << "dbus_connection_flush was not invoked.\n";
    }
}

// Test 2: dbus_write should report an error and return -1 when demarshal fails
// This verifies the error-path of the function where the error string is
// written to handle->errbuf as "dbus_message_demarshal() failed: <error>"
static void test_dbus_write_failure() {
    // Prepare a legitimate handle with priv->conn pointing to our fake connection
    pcap_t handle;
    memset(&handle, 0, sizeof(handle));

    pcap_dbus priv;
    memset(&priv, 0, sizeof(priv));

    priv.conn = reinterpret_cast<DBusConnection*>(&g_fake_conn);
    handle.priv = &priv;

    // Reset and configure the fake connection state
    reset_fake_conn();

    // Force demarshal to fail
    g_demarshal_should_fail = true;

    // Clear error buffer in the pcap_t to observe written error
    // The PCAP library defines PCAP_ERRBUF_SIZE; using a large enough buffer
    // is assumed. We clear existing content.
    if (handle.errbuf) {
        handle.errbuf[0] = '\0';
    }

    const char test_buf[] = "bad_message";
    int test_size = static_cast<int>(std::strlen(test_buf));

    // Call the focal function
    int ret = dbus_write(&handle, test_buf, test_size);

    // Expected: failure (-1) and appropriate error message in errbuf
    bool has_error_written = (handle.errbuf[0] != '\0');
    // Basic content check (the exact formatting depends on snprintf in the code)
    bool error_contains_mock = false;
    if (has_error_written) {
        std::string err(handle.errbuf);
        error_contains_mock = (err.find("dbus_message_demarshal() failed:") != std::string::npos) &&
                            (err.find("mock error") != std::string::npos);
    }

    bool passed = (ret == -1) && has_error_written && error_contains_mock;
    if (passed) {
        TEST_PASS;
    } else {
        TEST_FAIL;
        if (ret != -1) std::cerr << "Expected return -1 on failure, got " << ret << "\n";
        if (!has_error_written) std::cerr << "Expected errbuf to be populated with error message.\n";
        if (!error_contains_mock)
            std::cerr << "Expected error message to contain 'dbus_message_demarshal() failed:' and 'mock error'.\n";
        // For deeper debugging, print the actual errbuf
        if (has_error_written) std::cerr << "errbuf: " << handle.errbuf << "\n";
    }
}

int main() {
    // Run the two unit tests in a straightforward manner.
    // In a real CI, you would want to capture return codes and summarize results.
    test_dbus_write_success();
    test_dbus_write_failure();

    // Simple finalization message
    std::cout << "Unit tests for dbus_write completed.\n";
    return 0;
}