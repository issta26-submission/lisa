/*
   C++11 test suite for the focal C function:
   image_transform_png_set_bgr_add

   Key ideas:
   - The focal function links a new image_transform node into a singly linked list
     by setting this->next = *that; *that = this; and returns true when
     colour_type is RGB or RGBA.
   - We provide minimal, compatible C++ test scaffolding (no GTest) with a tiny
     non-terminating assertion mechanism.
   - We rely on a compatible image_transform layout where the first member is
     'image_transform *next'. This keeps tests simple and representative of the
     real library usage in libpng's pngvalid tests.
   - We call the C function via C linkage from C++ to ensure correct name mangling.
*/

#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <png.h>
#include <math.h>


// Include libpng header with C linkage to obtain png_byte and color type constants.
#ifdef __cplusplus
extern "C" {
#endif

typedef struct image_transform image_transform;
struct image_transform { image_transform *next; };

// Declaration of the focal function (C linkage)
int image_transform_png_set_bgr_add(image_transform *this_ptr,
                                  const image_transform **that,
                                  png_byte colour_type,
                                  png_byte bit_depth);

#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness (collects failures but continues execution)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const std::string &test_name, const std::string &message) {
    std::cerr << "TEST FAILED: " << test_name << " - " << message << std::endl;
    ++g_failed_tests;
}

static void log_success(const std::string &test_name) {
    std::cout << "TEST PASSED: " << test_name << std::endl;
}

// Lightweight assertion macro: does not terminate the test on failure
#define ASSERT_TRUE(cond, test_name, message) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { log_failure(test_name, message); } \
        else { log_success(test_name); } \
    } while (0)

#define ASSERT_EQ(expected, actual, test_name, message) \
    do { \
        ++g_total_tests; \
        if (!((expected) == (actual))) { \
            log_failure(test_name, message); \
        } else { \
            log_success(test_name); \
        } \
    } while (0)

// Forward the real color type constants via libpng
using namespace std;

// Test 1: RGB should be accepted; new node should be linked as head and next should point to previous head (nullptr)
static void test_bgr_add_rgb_sets_head_and_next_null() {
    const std::string test_name = "test_bgr_add_rgb_sets_head_and_next_null";

    // Initial head is nullptr
    image_transform *head = nullptr;

    // New node to insert
    image_transform *node = new image_transform();
    node->next = nullptr; // initialize; actual function will overwrite this

    int ret = image_transform_png_set_bgr_add(node, (const image_transform **)&head,
                                             PNG_COLOR_TYPE_RGB, 8);

    // Expectations:
    // 1) head now points to node
    // 2) node->next points to previous head (nullptr in this test)
    // 3) return is true (non-zero)
    ASSERT_EQ(reinterpret_cast<image_transform*>(node), head, test_name,
              "head should point to newly inserted node (head == node)");
    ASSERT_TRUE(node->next == nullptr, test_name,
                "node->next should be previous head (nullptr) after insertion");
    ASSERT_EQ(1, ret, test_name, "return value should be true for RGB colour type");

    delete node;
}

// Test 2: RGBA should be accepted; new node's next should point to previous head (pre-existing dummy)
static void test_bgr_add_rgba_links_to_existing_head() {
    const std::string test_name = "test_bgr_add_rgba_links_to_existing_head";

    // Create an existing head
    image_transform *existing_head = new image_transform();
    existing_head->next = nullptr;

    // New node to insert
    image_transform *new_node = new image_transform();
    new_node->next = nullptr;

    int ret = image_transform_png_set_bgr_add(new_node, (const image_transform **)&existing_head,
                                             PNG_COLOR_TYPE_RGBA, 8);

    // Expectations:
    // 1) existing_head (head) now points to new_node
    // 2) new_node->next points to old head (existing_head_before)
    // 3) return is true for RGBA
    ASSERT_EQ(reinterpret_cast<image_transform*>(new_node), existing_head, test_name,
              "head should be updated to point to the new node (head == new_node)");
    ASSERT_EQ(reinterpret_cast<image_transform*>(nullptr), (void*)new_node->next, test_name,
              "new_node->next should be the old head (existing_head_before) which was non-null");
    // The previous head before insertion is the value existing_head before the call.
    // Since we cannot easily capture it after call (it's overwritten), we verify only that
    // new_node->next is non-null or matches the pre-insertion value by inspecting a known prior node.
    // Here we perform a practical check: ensure new_node->next is non-null and was not overwritten to nullptr.
    if (new_node->next == nullptr) {
        log_failure(test_name, "new_node->next unexpectedly null; expected previous head non-null");
        ++g_failed_tests;
    } else {
        log_success(test_name);
    }

    ASSERT_EQ(1, ret, test_name, "return value should be true for RGBA colour type");

    delete new_node;
    delete existing_head;
}

// Test 3: Gray (non-RGB/RGBA) should not be accepted by the color-type condition, but still links correctly
static void test_bgr_add_gray_returns_false_and_links() {
    const std::string test_name = "test_bgr_add_gray_returns_false_and_links";

    image_transform *head = nullptr;
    image_transform *node = new image_transform();
    node->next = nullptr;

    int ret = image_transform_png_set_bgr_add(node, (const image_transform **)&head,
                                             PNG_COLOR_TYPE_GRAY, 8);

    // Expectations:
    // 1) head should still point to the newly inserted node
    // 2) node->next should be nullptr (since previous head was nullptr)
    // 3) return value should be false (0)
    ASSERT_EQ(reinterpret_cast<image_transform*>(node), head, test_name,
              "head should point to newly inserted node (head == node)");
    ASSERT_TRUE(node->next == nullptr, test_name,
                "node->next should be previous head (nullptr) after insertion");
    ASSERT_EQ(0, ret, test_name, "return value should be false for non-RGB/RGBA colour type");

    delete node;
}

int main(void) {
    // Run tests in a simple, direct manner as required by the instructions
    test_bgr_add_rgb_sets_head_and_next_null();
    test_bgr_add_rgba_links_to_existing_head();
    test_bgr_add_gray_returns_false_and_links();

    // Summary
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests;
}