// C++11 test suite for sqlite3VdbeMemNumerify (vdbemem.c) using real SQLite internal types.
// This test is designed to be run in a project where SQLite internals are available
// (sqliteInt.h, vdbeInt.h, etc.). It avoids terminating assertions and reports
// failures to stdout for easy integration into a custom test harness.

#include <vector>
#include <cmath>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cassert>


extern "C" {
}

// Lightweight test reporting
static std::vector<std::string> g_failures;

#define TEST_CHECK(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string(msg)); } } while(0)
#define TEST_NAME(name) static const char* g_test_name = name;

// Helper function to establish a Mem object for testing
struct MemTestHelper {
    Mem m;
    char* zbuf;
    MemTestHelper() : zbuf(nullptr) { std::memset(&m, 0, sizeof(Mem)); m.db = 0; }
    ~MemTestHelper() { if(zbuf) delete[] zbuf; }

    // Prepare Mem with a string payload
    void setFromString(const char* s, int n, unsigned int flags, unsigned char enc = SQLITE_UTF8) {
        if(zbuf) delete[] zbuf;
        int len = (s ? std::strlen(s) : 0);
        if(n > 0) len = n;
        zbuf = new char[len + 1];
        if(s) std::memcpy(zbuf, s, len);
        zbuf[len] = '\0';
        m.z = zbuf;
        m.n = len;
        m.enc = enc;
        m.flags = flags;
        // reset value storage
        m.u.i = 0;
        m.u.r = 0.0;
        m.xDel = nullptr;
        // Mem.db is left as 0 to satisfy the assertions in sqlite3VdbeMemNumerify when no DB is attached.
        m.db = 0;
        // Ensure a clean state for other fields that may be used by the function
        m.flags &= ~(MEM_Str | MEM_Blob | MEM_Zero);
        m.flags &= ~MEM_IntReal; // don't predefine composite types
    }

    // Convenience for numeric payloads
    void setFromCStr(const char* s, unsigned int flags) {
        setFromString(s, -1, flags);
    }
};

// Test 1: String "456" should numerify to MEM_Int with i=456 and clear Str/Blob/Zero
void test_numerify_int_branch() {
    TEST_NAME("test_numerify_int_branch");
    MemTestHelper mh;
    mh.setFromCStr("456", MEM_Str);

    int rc = sqlite3VdbeMemNumerify(&mh.m);

    TEST_CHECK(rc == SQLITE_OK, "return code should be SQLITE_OK");
    // Should become integer
    TEST_CHECK((mh.m.flags & MEM_Int) != 0, "Flags should include MEM_Int after numerify");
    // Str/Blob/Zero should be cleared
    TEST_CHECK((mh.m.flags & (MEM_Str | MEM_Blob | MEM_Zero)) == 0, "MEM_Str/MEM_Blob/MEM_Zero should be cleared");
    // Value should be 456
    TEST_CHECK(mh.m.u.i == 456, "Integer value should be 456 after numerify");
}

// Test 2: String "3.14" should numerify to MEM_Real with r approximately 3.14
void test_numerify_real_branch() {
    TEST_NAME("test_numerify_real_branch");
    MemTestHelper mh;
    mh.setFromCStr("3.14", MEM_Str);

    int rc = sqlite3VdbeMemNumerify(&mh.m);

    TEST_CHECK(rc == SQLITE_OK, "return code should be SQLITE_OK");
    // Should become real
    TEST_CHECK((mh.m.flags & MEM_Real) != 0, "Flags should include MEM_Real after numerify");
    // Str/Blob/Zero should be cleared
    TEST_CHECK((mh.m.flags & (MEM_Str | MEM_Blob | MEM_Zero)) == 0, "MEM_Str/MEM_Blob/MEM_Zero should be cleared");
    // Value should be approximately 3.14
    TEST_CHECK(std::abs(mh.m.u.r - 3.14) < 1e-9, "Real value should be approximately 3.14 after numerify");
}

// Test 3: Pre-existing MEM_Int should remain unchanged (path where (MEM_Int|MEM_Real|MEM_IntReal|MEM_Null) != 0)
void test_preserve_existing_int_flag() {
    TEST_NAME("test_preserve_existing_int_flag");
    MemTestHelper mh;
    // Pre-set as MEM_Int with value 7
    mh.m.flags = MEM_Int;
    mh.m.u.i = 7;
    mh.m.z = nullptr;
    mh.m.n = 0;
    mh.m.enc = SQLITE_UTF8;
    mh.m.db = 0;

    int rc = sqlite3VdbeMemNumerify(&mh.m);

    TEST_CHECK(rc == SQLITE_OK, "return code should be SQLITE_OK");
    // Should still be MEM_Int
    TEST_CHECK((mh.m.flags & MEM_Int) != 0, "MEM_Int should remain set");
    // Value should remain 7
    TEST_CHECK(mh.m.u.i == 7, "Integer value should remain unchanged when MEM_Int is already set");
    // Str/Blob/Zero should be cleared (even if they were not set)
    TEST_CHECK((mh.m.flags & (MEM_Str | MEM_Blob | MEM_Zero)) == 0, "MEM_Str/MEM_Blob/MEM_Zero should be cleared");
}

// Driver to run all tests and report
int main() {
    // Run tests
    test_numerify_int_branch();
    test_numerify_real_branch();
    test_preserve_existing_int_flag();

    // Report
    if(g_failures.empty()) {
        std::cout << "All tests passed (" << 3 << " tests).\n";
        return 0;
    } else {
        std::cout << g_failures.size() << " test(s) failed:\n";
        for(const auto& s : g_failures) {
            std::cout << " - " << s << "\n";
        }
        return 1;
    }
}