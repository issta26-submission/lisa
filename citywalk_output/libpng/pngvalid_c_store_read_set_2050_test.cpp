// This file provides a self-contained test harness for the focal function
// store_read_set as described in the prompt. It includes a minimal, self-contained
// mock environment to exercise both control flow branches of the function
// (found in a list of stored files and not found). It uses plain C-style
// structures and a C linkage for the focal function, but the test harness is
// implemented in C++11 to leverage standard library facilities and a lightweight
// test runner (no Google Test required).

/*
Candidate Keywords extracted from the focal method and its dependencies:
- store_read_set
- png_store
- png_uint_32
- png_store_file
- IDAT_size
- IDAT_bits
- IDAT_len
- IDAT_pos
- IDAT_crc
- store_read_buffer_next
- standard_name_from_id
- safecat
- png_error
- pf (png_store_file)
- saved
- next
- id
- file not found
- msg buffer
- FILE_NAME_SIZE
*/

#include <cassert>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <exception>
#include <math.h>


// Lightweight test harness utilities
static int test_failed = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << "\n"; \
        ++test_failed; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "ASSERT EQUALS FAILED: " << (msg) \
                  << " | expected: " << (b) << " actual: " << (a) << "\n"; \
        ++test_failed; \
    } \
} while(0)


// Forward declare the minimal C-style environment needed by store_read_set
extern "C" {

// Typedefs and macros to mirror the C API used by the focal method
typedef unsigned int png_uint_32;
typedef struct png_store_file {
    png_uint_32 id;
    size_t IDAT_size;
    unsigned int IDAT_bits;
    struct png_store_file *next;
} png_store_file;

typedef struct png_store {
    png_store_file *saved;
    png_store_file *current;
    png_store_file *next;
    size_t IDAT_size;
    unsigned int IDAT_bits;
    size_t IDAT_len;
    size_t IDAT_pos;
    unsigned long IDAT_crc;
    void *pread; // placeholder for png_structp
} png_store;

#define FILE_NAME_SIZE 256

// Function prototypes used by the focal function
size_t standard_name_from_id(char *buffer, size_t bufsize, size_t pos, png_uint_32 id);
size_t safecat(char *buffer, size_t bufsize, size_t pos, const char *cat);
void png_error(void *ppIn, const char *msg);
void store_read_buffer_next(png_store *ps);
void store_read_set(png_store *ps, png_uint_32 id);

} // extern "C"

// Implementations for the mock environment (C linkage)
size_t standard_name_from_id(char *buffer, size_t bufsize, size_t pos, png_uint_32 id) {
    // Simple textual representation of the id: "id<id>"
    // Note: No bounds-checking for brevity in test scaffolding; enough for tests.
    int written = snprintf(buffer + pos, (bufsize > pos ? bufsize - pos : 0), "id%u", (unsigned)id);
    if (written < 0) written = 0;
    return pos + static_cast<size_t>(written);
}

size_t safecat(char *buffer, size_t bufsize, size_t pos, const char *cat) {
    if (buffer == nullptr || cat == nullptr) return pos;
    size_t i = 0;
    // Append cat starting at pos, respecting buffer limit
    while (cat[i] != '\0' && (pos + i) < (bufsize - 1)) {
        buffer[pos + i] = cat[i];
        ++i;
    }
    buffer[pos + i] = '\0';
    return pos + i;
}

// Custom exception type to mimic png_error halting behavior in tests
class PngTestError : public std::exception {
public:
    explicit PngTestError(const std::string &m) : message(m) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

static void png_error_thrower(void * /*ppIn*/, const char *msg) {
    // Throw C++ exception to emulate non-local exit from error path
    throw PngTestError(std::string(msg));
}

// Variant of png_error used by the focal function (with C linkage)
extern "C" void png_error(void *ppIn, const char *msg) {
    // Route to the exception-throwing helper
    png_error_thrower(ppIn, msg);
}

// A flag to verify that store_read_buffer_next was invoked
static bool buffer_read_next_was_called = false;
extern "C" void store_read_buffer_next(png_store *ps) {
    (void)ps; // unused in this minimal test stub
    buffer_read_next_was_called = true;
}

// The focal function under test, implemented as provided in C source
extern "C" void store_read_set(png_store *ps, png_uint_32 id) {
{
   png_store_file *pf = ps->saved;
   while (pf != NULL)
   {
      if (pf->id == id)
      {
         ps->current = pf;
         ps->next = NULL;
         ps->IDAT_size = pf->IDAT_size;
         ps->IDAT_bits = pf->IDAT_bits; /* just a cache */
         ps->IDAT_len = 0;
         ps->IDAT_pos = 0;
         ps->IDAT_crc = 0UL;
         store_read_buffer_next(ps);
         return;
      }
      pf = pf->next;
   }
   {
      size_t pos;
      char msg[FILE_NAME_SIZE+64];
      pos = standard_name_from_id(msg, sizeof msg, 0, id);
      pos = safecat(msg, sizeof msg, pos, ": file not found");
      png_error(ps->pread, msg);
   }
}
}

// Test helpers to construct test scenarios

struct TestNode {
    png_uint_32 id;
    size_t IDAT_size;
    unsigned int IDAT_bits;
    TestNode *next;
};

// Test 1: Found in the first node of the saved list
void test_store_read_set_found_first() {
    // Candidate Keywords: pf, saved, current, IDAT_size, IDAT_bits, IDAT_len, IDAT_pos, IDAT_crc, store_read_buffer_next
    buffer_read_next_was_called = false;

    // Build list: first node has the target id
    TestNode n1 = { 101, 37, 2, nullptr };
    TestNode n2 = { 202, 55, 4, nullptr };
    // Link list via png_store_file-like structure
    png_store_file pf1 = { n1.id, n1.IDAT_size, n1.IDAT_bits, nullptr };
    png_store_file pf2 = { n2.id, n2.IDAT_size, n2.IDAT_bits, nullptr };
    pf1.next = &pf2;

    png_store ps;
    ps.saved = &pf1;
    ps.current = nullptr;
    ps.next = nullptr;
    ps.IDAT_size = 0;
    ps.IDAT_bits = 0;
    ps.IDAT_len = 0;
    ps.IDAT_pos = 0;
    ps.IDAT_crc = 0;
    ps.pread = nullptr;

    // Act
    store_read_set(&ps, n1.id);

    // Assert
    ASSERT_TRUE(ps.current == &pf1, "store_read_set should set current to matched node (first)");
    ASSERT_TRUE(ps.next == nullptr, "store_read_set should set next to NULL when found");
    ASSERT_EQ(ps.IDAT_size, pf1.IDAT_size, "IDAT_size should be copied from stored file");
    ASSERT_EQ(ps.IDAT_bits, pf1.IDAT_bits, "IDAT_bits should be copied from stored file");
    ASSERT_EQ(ps.IDAT_len, 0, "IDAT_len should be reset to 0");
    ASSERT_EQ(ps.IDAT_pos, 0, "IDAT_pos should be reset to 0");
    ASSERT_EQ(ps.IDAT_crc, 0UL, "IDAT_crc should be reset to 0");
    ASSERT_TRUE(buffer_read_next_was_called, "store_read_buffer_next should be invoked when a match is found");
}

// Test 2: Found in the second node of the saved list
void test_store_read_set_found_second() {
    // Candidate Keywords: pf, saved, current, IDAT_size, IDAT_bits, IDAT_len, IDAT_pos, IDAT_crc
    buffer_read_next_was_called = false;

    // Build list: second node has the target id
    png_store_file pf1 = { 101, 12, 1, nullptr };
    png_store_file pf2 = { 202, 99, 3, nullptr };
    pf1.next = &pf2;

    png_store ps;
    ps.saved = &pf1;
    ps.current = nullptr;
    ps.next = nullptr;
    ps.IDAT_size = 0;
    ps.IDAT_bits = 0;
    ps.IDAT_len = 0;
    ps.IDAT_pos = 0;
    ps.IDAT_crc = 0;
    ps.pread = nullptr;

    // Act
    store_read_set(&ps, pf2.id);

    // Assert
    ASSERT_TRUE(ps.current == &pf2, "store_read_set should set current to matched node (second in list)");
    ASSERT_TRUE(ps.next == nullptr, "store_read_set should set next to NULL when found");
    ASSERT_EQ(ps.IDAT_size, pf2.IDAT_size, "IDAT_size should be copied from second node");
    ASSERT_EQ(ps.IDAT_bits, pf2.IDAT_bits, "IDAT_bits should be copied from second node");
    ASSERT_EQ(ps.IDAT_len, 0, "IDAT_len should be reset to 0 (second test)");
    ASSERT_EQ(ps.IDAT_pos, 0, "IDAT_pos should be reset to 0 (second test)");
    ASSERT_EQ(ps.IDAT_crc, 0UL, "IDAT_crc should be reset to 0 (second test)");
    ASSERT_TRUE(buffer_read_next_was_called, "store_read_buffer_next should be invoked when a match is found (second test)");
}

// Test 3: Not found in the saved list should trigger an error path
void test_store_read_set_not_found() {
    // Candidate Keywords: saved, current, next, IDAT_size, IDAT_bits, IDAT_len, IDAT_pos, IDAT_crc, standard_name_from_id, safecat, png_error
    // Build list with no matching id
    png_store_file pf = { 111, 0, 0, nullptr };

    png_store ps;
    ps.saved = &pf;
    ps.current = nullptr;
    ps.next = nullptr;
    ps.IDAT_size = 0;
    ps.IDAT_bits = 0;
    ps.IDAT_len = 0;
    ps.IDAT_pos = 0;
    ps.IDAT_crc = 0;
    ps.pread = nullptr;

    try {
        store_read_set(&ps, 9999); // non-existent id
        // If we reach here, no exception was thrown, which means error path not exercised
        std::cerr << "ERROR: png_error was not invoked for non-found id\n";
        ++test_failed;
    } catch (const PngTestError& e) {
        // Expected path: check the error message matches "id9999: file not found"
        std::string got(e.what());
        std::string expected = "id9999: file not found";
        ASSERT_TRUE(got == expected, "png_error should be called with a message indicating file not found");
    } catch (...) {
        std::cerr << "ERROR: Unexpected exception type caught for not found path\n";
        ++test_failed;
    }
}


// Main test driver: run all tests
int main() {
    test_store_read_set_found_first();
    test_store_read_set_found_second();
    test_store_read_set_not_found();

    if (test_failed == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "TESTS FAILED: " << test_failed << "\n";
        return 1;
    }
}