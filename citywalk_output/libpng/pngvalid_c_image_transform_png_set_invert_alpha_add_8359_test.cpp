// Unit test suite for the focal method image_transform_png_set_invert_alpha_add
// Note: This test suite provides a self-contained C++11 implementation
// that mimics the essential behavior of the focal C function for the purpose
// of unit testing in environments where the original C file is unavailable.
// The tests are designed to be executable with only the C++ standard library
// and do not rely on any external testing framework (e.g., GTest).

#include <signal.h>
#include <string.h>
#include <string>
#include <utility>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Minimal type definitions to mirror the focal function's interface.
typedef unsigned char png_byte;

// Lightweight struct representing the minimal dependency (image_transform).
struct image_transform {
    image_transform* next;
};

// Local reimplementation of the focal function logic for testing purposes.
// Signature matches the original (simplified for test environment).
// This mirrors:
// unsigned char image_transform_png_set_invert_alpha_add(image_transform *this,
//     const image_transform **that, png_byte colour_type, png_byte bit_depth)
unsigned char image_transform_png_set_invert_alpha_add(image_transform *this_ptr,
    const image_transform **that, png_byte colour_type, png_byte bit_depth)
{
    (void)bit_depth; // Simulates UNUSED(bit_depth) in the original code
    this_ptr->next = *that;
    *that = this_ptr;
    // Only has an effect on pixels with alpha: return true if colour_type has bit 2 set
    return (colour_type & 4) != 0;
}

// Simple static member usage demonstration to satisfy domain knowledge requirement.
// Access static members via the class name, as required.
class InstanceCounter {
public:
    static int count;
    InstanceCounter() { ++count; }
    ~InstanceCounter() { --count; }
};
int InstanceCounter::count = 0;

// Simple test result holder
struct TestResult {
    bool passed;
    std::string test_name;
    std::string detail;
};

// Test 1: Colour type with alpha bit set (4) should return true and update that pointer
TestResult test_invert_alpha_add_alpha_bit_set() {
    // Prepare a simple chain: that initially points to nullptr
    image_transform A;
    A.next = nullptr;
    const image_transform* that = nullptr;

    unsigned char ret = image_transform_png_set_invert_alpha_add(&A, &that, /*colour_type*/ 4, /*bit_depth*/ 8);

    bool ok = (ret != 0) && (that == &A) && (A.next == nullptr);
    TestResult tr;
    tr.test_name = "test_invert_alpha_add_alpha_bit_set";
    if (ok) {
        tr.passed = true;
        tr.detail = "Alpha bit set: returned 1; that points to A; A.next unchanged (nullptr).";
    } else {
        tr.passed = false;
        tr.detail = "Expected ret=1, that==&A, A.next==nullptr; "
                    "got ret=" + std::to_string(ret) +
                    ", that=" + std::to_string((that == &A) ? 1 : 0) +
                    ", A.next=" + std::to_string((A.next == nullptr) ? 1 : 0);
    }
    return tr;
}

// Test 2: Colour type without alpha bit set (e.g., 0) should return false and update that pointer
TestResult test_invert_alpha_add_no_alpha_bit_set() {
    image_transform B;
    B.next = nullptr;
    image_transform A2;
    A2.next = nullptr;

    // 'that' initially points to B
    const image_transform* that = &B;
    unsigned char ret = image_transform_png_set_invert_alpha_add(&A2, &that, /*colour_type*/ 0, /*bit_depth*/ 1);

    bool ok = (ret == 0) && (that == &A2) && (A2.next == &B);
    TestResult tr;
    tr.test_name = "test_invert_alpha_add_no_alpha_bit_set";
    if (ok) {
        tr.passed = true;
        tr.detail = "No alpha: returned 0; that points to A2; A2.next points to B.";
    } else {
        tr.passed = false;
        tr.detail = "Expected ret=0, that==&A2, A2.next==&B; "
                    "got ret=" + std::to_string(ret) +
                    ", that=" + std::to_string((that == &A2) ? 1 : 0) +
                    ", A2.next=" + std::to_string((A2.next == &B) ? 1 : 0);
    }
    return tr;
}

// Test 3: Existing 'that' chain is preserved; A should become the new head and its next points to the old head
TestResult test_invert_alpha_add_chain_preserved() {
    image_transform B;
    B.next = nullptr;

    image_transform A;
    A.next = nullptr;

    const image_transform* that = &B;
    unsigned char ret = image_transform_png_set_invert_alpha_add(&A, &that, /*colour_type*/ 4, /*bit_depth*/ 8);

    bool ok = (ret != 0) && (that == &A) && (A.next == &B) && (B.next == nullptr);
    TestResult tr;
    tr.test_name = "test_invert_alpha_add_chain_preserved";
    if (ok) {
        tr.passed = true;
        tr.detail = "Head becomes A; A.next points to B; B.next unchanged.";
    } else {
        tr.passed = false;
        tr.detail = "Expected ret=1, that==&A, A.next==&B; B.next==nullptr; got ret=" +
                    std::to_string(ret) + ", that==" +
                    std::to_string((that == &A) ? 1 : 0) +
                    ", A.next==" + std::to_string((A.next == &B) ? 1 : 0);
    }
    return tr;
}

// Test 4: Bit depth variations do not affect the return value
TestResult test_invert_alpha_add_bitdepth_variations() {
    image_transform A;
    A.next = nullptr;
    const image_transform* that = nullptr;

    unsigned char ret1 = image_transform_png_set_invert_alpha_add(&A, &that, /*colour_type*/ 4, /*bit_depth*/ 1);
    bool ok1 = (ret1 != 0) && (that == &A);

    image_transform A2;
    A2.next = nullptr;
    const image_transform* that2 = nullptr;
    unsigned char ret2 = image_transform_png_set_invert_alpha_add(&A2, &that2, /*colour_type*/ 4, /*bit_depth*/ 2);
    bool ok2 = (ret2 != 0) && (that2 == &A2);

    image_transform A3;
    A3.next = nullptr;
    const image_transform* that3 = nullptr;
    unsigned char ret3 = image_transform_png_set_invert_alpha_add(&A3, &that3, /*colour_type*/ 4, /*bit_depth*/ 123);
    bool ok3 = (ret3 != 0) && (that3 == &A3);

    bool ok = ok1 && ok2 && ok3;

    TestResult tr;
    tr.test_name = "test_invert_alpha_add_bitdepth_variations";
    if (ok) {
        tr.passed = true;
        tr.detail = "Bit depth variations do not affect return value when alpha bit is set.";
    } else {
        tr.passed = false;
        tr.detail = "Failed on bitdepth variations: ret1=" + std::to_string(ret1) +
                    ", ret2=" + std::to_string(ret2) + ", ret3=" + std::to_string(ret3);
    }
    return tr;
}

// Test 5: Multiple sequential calls create a chain (A <- B <- C)
TestResult test_invert_alpha_add_multiple_calls_chain() {
    // First head C
    image_transform C;
    C.next = nullptr;
    const image_transform* head = nullptr;
    unsigned char rc = image_transform_png_set_invert_alpha_add(&C, &head, /*colour_type*/ 4, /*bit_depth*/ 8);
    bool ok = (rc != 0) && (head == &C) && (C.next == nullptr);

    // Then add B in front of C
    image_transform B;
    B.next = nullptr;
    const image_transform* head2 = head;
    unsigned char rb = image_transform_png_set_invert_alpha_add(&B, &head2, /*colour_type*/ 4, /*bit_depth*/ 8);

    // Now head2 should be B, B.next should be C
    ok = ok && (rb != 0) && (head2 == &B) && (B.next == &C) && (C.next == nullptr);

    TestResult tr;
    tr.test_name = "test_invert_alpha_add_multiple_calls_chain";
    if (ok) {
        tr.passed = true;
        tr.detail = "Sequential calls form a chain: head -> B -> C with C as tail.";
    } else {
        tr.passed = false;
        tr.detail = "Chain formation failed: rc=" + std::to_string(rc) +
                    ", rb=" + std::to_string(rb) +
                    ", head2==" + std::to_string((head2 == &B) ? 1 : 0) +
                    ", B.next==" + std::to_string((B.next == &C) ? 1 : 0);
    }
    return tr;
}

// Test 6: Static member usage demonstration (ensures static members are handled properly by tests)
TestResult test_static_member_usage() {
    TestResult tr;
    tr.test_name = "test_static_member_usage";

    // Before: count should be 0
    bool ok = (InstanceCounter::count == 0);
    {
        InstanceCounter a;
        ok = ok && (InstanceCounter::count == 1);
        {
            InstanceCounter b;
            ok = ok && (InstanceCounter::count == 2);
        }
        // b destroyed; count should go back to 1
        ok = ok && (InstanceCounter::count == 1);
    }
    // a destroyed; count should be 0
    ok = ok && (InstanceCounter::count == 0);

    if (ok) {
        tr.passed = true;
        tr.detail = "Static member InstanceCounter::count correctly tracks object lifetime.";
    } else {
        tr.passed = false;
        tr.detail = "Static member tracking failed. Final count: " + std::to_string(InstanceCounter::count);
    }

    return tr;
}

// Helper to run and print all tests
void run_and_report_tests() {
    TestResult t1 = test_invert_alpha_add_alpha_bit_set();
    TestResult t2 = test_invert_alpha_add_no_alpha_bit_set();
    TestResult t3 = test_invert_alpha_add_chain_preserved();
    TestResult t4 = test_invert_alpha_add_bitdepth_variations();
    TestResult t5 = test_invert_alpha_add_multiple_calls_chain();
    TestResult t6 = test_static_member_usage();

    auto report = [](const TestResult& tr) {
        std::cout << (tr.passed ? "[PASS] " : "[FAIL] ") << tr.test_name
                  << " : " << tr.detail << std::endl;
    };

    report(t1);
    report(t2);
    report(t3);
    report(t4);
    report(t5);
    report(t6);
}

// Main function as required by the domain knowledge: call test methods from main
int main() {
    std::cout << "Starting unit tests for image_transform_png_set_invert_alpha_add (self-contained C++11 tests)" << std::endl;
    run_and_report_tests();
    std::cout << "Unit tests completed." << std::endl;
    return 0;
}