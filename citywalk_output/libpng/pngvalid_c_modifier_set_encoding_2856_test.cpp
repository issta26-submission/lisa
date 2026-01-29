// C++11 unit test harness for the C function: modifier_set_encoding(png_modifier *pm)
// This test suite exercises the logic of setting encoding indices and gamma values.
// It relies on the actual C implementation being linked (modifier_set_encoding).
// The test constructs a minimal environment that mirrors the dependent structures used by the focal method.

#include <cassert>
#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Forward declare the C function under test (to be linked with the actual implementation)
extern "C" void modifier_set_encoding(struct png_modifier *pm);

// Minimal reproduction of the dependent types used by modifier_set_encoding
typedef struct color_encoding {
    double gamma;
} color_encoding;

typedef struct png_modifier {
    double current_gamma;
    color_encoding *current_encoding;
    unsigned int encoding_ignored;
    unsigned int encoding_counter;
    unsigned int ngammas;
    double *gammas;              // array of screen gammas (size ngammas)
    unsigned int nencodings;
    color_encoding *encodings;     // array of encodings (size nencodings)
} png_modifier;

// Helper: create a png_modifier instance with given gamma tables and encodings
static png_modifier* create_modifier(unsigned int encoding_counter,
                                     const std::vector<double>& gammas,
                                     const std::vector<double>& enc_gamma_values)
{
    png_modifier* pm = new png_modifier();
    pm->current_gamma = 0.0;
    pm->current_encoding = nullptr;
    pm->encoding_ignored = 0;
    pm->encoding_counter = encoding_counter;
    pm->gammas = nullptr;
    pm->encodings = nullptr;
    pm->ngammas = static_cast<unsigned int>(gammas.size());
    pm->nencodings = static_cast<unsigned int>(enc_gamma_values.size());

    // Allocate and copy gammas
    if (gammas.size() > 0) {
        pm->gammas = new double[gammas.size()];
        std::memcpy(pm->gammas, gammas.data(), gammas.size() * sizeof(double));
    }

    // Allocate and copy encodings
    if (enc_gamma_values.size() > 0) {
        pm->encodings = new color_encoding[enc_gamma_values.size()];
        for (size_t i = 0; i < enc_gamma_values.size(); ++i)
            pm->encodings[i].gamma = enc_gamma_values[i];
    }

    // Initialize current_encoding to nullptr to reflect "Set in rules" behavior
    pm->current_encoding = nullptr;

    return pm;
}

// Helper: destroy a modifier to avoid memory leaks
static void destroy_modifier(png_modifier* pm)
{
    if (!pm) return;
    delete[] pm->gammas;
    delete[] pm->encodings;
    delete pm;
}

// Simple non-terminating assertion helper: prints on failure but continues
static bool assert_eq_double(double a, double b, double tol, const std::string& msg)
{
    bool ok = std::fabs(a - b) <= tol;
    if (!ok) {
        std::cerr << "ASSERT FAIL: " << msg
                  << " | expected " << b << ", got " << a << "\n";
    }
    return ok;
}

static bool assert_ptr_eq(const void* a, const void* b, const std::string& msg)
{
    bool ok = (a == b);
    if (!ok) {
        std::cerr << "ASSERT FAIL: " << msg
                  << " | expected " << b << ", got " << a << "\n";
    }
    return ok;
}

// Test 1: encoding_counter == 0 should reset to zero values
// - current_gamma == 0
// - current_encoding == nullptr
// - encoding_ignored == 0
static bool test_modifier_set_encoding_counter_zero()
{
    // Arrange
    // ngammas and encodings are irrelevant when counter == 0, but provide sensible values
    std::vector<double> gammas = {2.0, 0.5, 0.25};            // not used
    std::vector<double> enc_gamma_values = {1.2, 1.5};         // not used
    png_modifier* pm = create_modifier(0, gammas, enc_gamma_values);

    // Pre-condition: ensure current fields have non-default values to prove they are overwritten
    pm->current_gamma = 9.9;
    pm->current_encoding = reinterpret_cast<color_encoding*>(0xDEADBEEF);
    pm->encoding_ignored = 1;

    // Act
    modifier_set_encoding(pm);

    // Assert
    bool ok = true;
    ok &= assert_eq_double(pm->current_gamma, 0.0, 1e-12, "test1: current_gamma should be 0 when encoding_counter == 0");
    ok &= assert_ptr_eq(pm->current_encoding, nullptr, "test1: current_encoding should be nullptr when encoding_counter == 0");
    ok &= (pm->encoding_ignored == 0);

    // Cleanup
    destroy_modifier(pm);
    return ok;
}

// Test 2: encoding_counter <= ngammas uses reciprocal gamma from gammas[]
// - current_encoding remains nullptr
// - current_gamma == 1 / gammas[encoding_counter-1]
static bool test_modifier_set_encoding_gamma_from_gammas()
{
    // Arrange
    // ngammas = 3, gammas[0..2] = {2.0, 0.5, 0.25}
    std::vector<double> gammas = {2.0, 0.5, 0.25};
    std::vector<double> enc_gamma_values = {1.0, 2.0, 3.0}; // not used in this path
    png_modifier* pm = create_modifier(2, gammas, enc_gamma_values); // encoding_counter = 2 -> <= ngammas

    // Pre-condition
    pm->current_gamma = -1.0;
    pm->current_encoding = nullptr;

    // Act
    modifier_set_encoding(pm);

    // Assert
    double expected = 1.0 / gammas[pm->encoding_counter - 1]; // gammas[1] = 0.5 -> 2.0
    bool ok = true;
    ok &= assert_eq_double(pm->current_gamma, expected, 1e-12, "test2: current_gamma should be reciprocal of gammas[encoding_counter-1]");
    ok &= (pm->current_encoding == nullptr);

    // Cleanup
    destroy_modifier(pm);
    return ok;
}

// Test 3: encoding_counter > ngammas and i < nencodings uses encodings[i].gamma
// - current_gamma == encodings[i].gamma
// - current_encoding points to encodings[i]
static bool test_modifier_set_encoding_encodings_index_within_range()
{
    // Arrange
    // ngammas = 1, nencodings = 4, encodings.gamma = {0.0, 2.25, 4.0, 5.5}
    std::vector<double> gammas = {1.0, 2.0}; // small, not used in this path
    std::vector<double> enc_gamma_values = {0.0, 2.25, 4.0, 5.5};
    png_modifier* pm = create_modifier(3, gammas, enc_gamma_values); // encoding_counter = ngammas + i = 1 + 2 = 3? We'll set to 3 to pick i=2
    // We want i = encoding_counter - ngammas = 2, so encoding_counter must be ngammas + 2 = 3
    pm->encoding_counter = 3;

    // Pre-condition
    pm->current_gamma = -1.0;
    pm->current_encoding = nullptr;

    // Act
    modifier_set_encoding(pm);

    // Assert
    double expected = enc_gamma_values[2]; // i = 2
    bool ok = true;
    ok &= assert_eq_double(pm->current_gamma, expected, 1e-12, "test3: current_gamma should be encodings[i].gamma when i < nencodings");
    ok &= assert_ptr_eq(static_cast<const void*>(pm->current_encoding), static_cast<const void*>(&pm->encodings[2]),
                         "test3: current_encoding should point to encodings+i when i < nencodings");

    // Cleanup
    destroy_modifier(pm);
    return ok;
}

// Test 4: encoding_counter results in i >= nencodings triggers modulo and current_gamma = 1
// - current_gamma == 1.0
// - current_encoding points to encodings + (i % nencodings)
static bool test_modifier_set_encoding_encodings_index_overflow()
{
    // Arrange
    // ngammas = 0, nencodings = 2, encodings.gamma = {3.0, 4.0}
    std::vector<double> gammas = {}; // not used
    std::vector<double> enc_gamma_values = {3.0, 4.0};
    png_modifier* pm = create_modifier(4, gammas, enc_gamma_values); // encoding_counter = 4
    // ici: i = encoding_counter - ngammas = 4
    // i >= nencodings (2) true; after modulo i = 0, current_encoding = encodings+0

    // Pre-condition
    pm->current_gamma = -2.0;
    pm->current_encoding = nullptr;

    // Act
    modifier_set_encoding(pm);

    // Assert
    bool ok = true;
    ok &= assert_eq_double(pm->current_gamma, 1.0, 1e-12, "test4: current_gamma should be 1 when i >= nencodings (linear case)");
    ok &= assert_ptr_eq(static_cast<void*>(pm->current_encoding), static_cast<void*>(pm->encodings + 0),
                        "test4: current_encoding should be encodings + (i % nencodings) when overflow");

    // Cleanup
    destroy_modifier(pm);
    return ok;
}

// Main: run all tests and summarize results
int main() {
    int passed = 0;
    int total  = 0;

    std::cout << "Starting unit tests for modifier_set_encoding...\n";

    if (test_modifier_set_encoding_counter_zero()) {
        ++passed;
    } else {
        std::cout << "Test 1 FAILED\n";
    }
    ++total;

    if (test_modifier_set_encoding_gamma_from_gammas()) {
        ++passed;
    } else {
        std::cout << "Test 2 FAILED\n";
    }
    ++total;

    if (test_modifier_set_encoding_encodings_index_within_range()) {
        ++passed;
    } else {
        std::cout << "Test 3 FAILED\n";
    }
    ++total;

    if (test_modifier_set_encoding_encodings_index_overflow()) {
        ++passed;
    } else {
        std::cout << "Test 4 FAILED\n";
    }
    ++total;

    std::cout << "Tests completed: " << passed << " / " << total << " passed.\n";
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}