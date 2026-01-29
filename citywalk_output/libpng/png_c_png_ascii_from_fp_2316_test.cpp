// Minimal C++11 unit test suite for the focal C routine: png_ascii_from_fp
// This test provides a lightweight, self-contained environment by
// re-implementing the necessary dependencies and the focal function itself.
// It verifies important branches and behaviors without relying on
// external libraries (GTest not used).

#include <string>
#include <cfloat>
#include <cmath>
#include <pngpriv.h>
#include <iostream>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <cstring>
#include <exception>
#include <cstddef>


// ---------------------------
// FOCAL_METHOD DEPENDENCIES
// ---------------------------

// Lightweight stubs to mimic libpng types for the test environment.
typedef struct png_struct_def png_struct;
typedef const png_struct_def* png_const_structrp;
typedef png_struct_def* png_structrp;
typedef char* png_charp;
typedef const char* png_const_charp;

// Custom exception to catch "png_error" invocations in tests.
class PngError : public std::exception {
public:
    explicit PngError(const char* msg) : _msg(msg ? msg : "") {}
    const char* what() const noexcept override { return _msg.c_str(); }
private:
    std::string _msg;
};

// Global hook used by the focal function to signal an error.
// Our tests will catch PngError to validate error paths.
extern "C" {
void png_error(png_const_structrp /*png_ptr*/, const char* error)
{
    throw PngError(error);
}
}

// Power-of-10 helper used by the focal function (as in the original codebase).
double png_pow10(int power)
{
    // Simple, reasonably fast implementation using std::pow.
    return std::pow(10.0, static_cast<double>(power));
}

// The focal function under test (reproduced in this test file for isolation).
// Note: This is a faithful reproduction of the provided algorithm segment.
// It relies on the aforementioned dependencies (png_error, png_pow10, etc.).
void png_ascii_from_fp(png_const_structrp png_ptr, png_charp ascii, std::size_t size,
    double fp, unsigned int precision)
{
{
   // Use a standard-headers based approach for numeric constants
   if (precision < 1)
      precision = DBL_DIG;
   if (precision > DBL_DIG+1)
      precision = DBL_DIG+1;
   if (size >= precision+5) 
   {
      if (fp < 0)
      {
         fp = -fp;
         *ascii++ = 45; 
         --size;
      }
      if (fp >= DBL_MIN && fp <= DBL_MAX)
      {
         int exp_b10;   
         double base;   
         (void)frexp(fp, &exp_b10); /* exponent to base 2 */
         exp_b10 = (exp_b10 * 77) >> 8; /* <= exponent to base 10 */
         base = png_pow10(exp_b10); 
         while (base < DBL_MIN || base < fp)
         {
            double test = png_pow10(exp_b10+1);
            if (test <= DBL_MAX)
            {
               ++exp_b10; base = test;
            }
            else
               break;
         }
         fp /= base;
         while (fp >= 1)
         {
            fp /= 10; ++exp_b10;
         }
         {
            unsigned int czero, clead, cdigits;
            char exponent[10];
            if (exp_b10 < 0 && exp_b10 > -3) 
            {
               czero = 0U-exp_b10; 
               exp_b10 = 0;      
            }
            else
               czero = 0;    
            clead = czero; 
            cdigits = 0;   
            do
            {
               double d;
               fp *= 10;
               if (cdigits+czero+1 < precision+clead)
                  fp = modf(fp, &d);
               else
               {
                  d = floor(fp + .5);
                  if (d > 9)
                  {
                     if (czero > 0)
                     {
                        --czero; d = 1;
                        if (cdigits == 0) --clead;
                     }
                     else
                     {
                        while (cdigits > 0 && d > 9)
                        {
                           int ch = *--ascii;
                           if (exp_b10 != (-1))
                              ++exp_b10;
                           else if (ch == 46)
                           {
                              ch = *--ascii; ++size;
                              exp_b10 = 1;
                           }
                           --cdigits;
                           d = ch - 47;  /* I.e. 1+(ch-48) */
                        }
                        if (d > 9)  /* cdigits == 0 */
                        {
                           if (exp_b10 == (-1))
                           {
                              int ch = *--ascii;
                              if (ch == 46)
                              {
                                 ++size; exp_b10 = 1;
                              }
                           }
                           else
                              ++exp_b10;
                           d = 1;
                        }
                     }
                  }
                  fp = 0; /* Guarantees termination below. */
               }
               if (d == 0)
               {
                  ++czero;
                  if (cdigits == 0) ++clead;
               }
               else
               {
                  cdigits += czero - clead;
                  clead = 0;
                  while (czero > 0)
                  {
                     if (exp_b10 != (-1))
                     {
                        if (exp_b10 == 0)
                        {
                           *ascii++ = 46; --size;
                        }
                        --exp_b10;
                     }
                     *ascii++ = 48; --czero;
                  }
                  if (exp_b10 != (-1))
                  {
                     if (exp_b10 == 0)
                     {
                        *ascii++ = 46; --size;
                     }
                     --exp_b10;
                  }
                  *ascii++ = (char)(48 + (int)d); ++cdigits;
               }
            }
            while (cdigits+czero < precision+clead && fp > DBL_MIN);
            if (exp_b10 >= (-1) && exp_b10 <= 2)
            {
               while (exp_b10-- > 0) *ascii++ = 48;
               *ascii = 0;
               return;
            }
            size -= cdigits;
            *ascii++ = 69; --size;    /* 'E' */
            {
               unsigned int uexp_b10;
               if (exp_b10 < 0)
               {
                  *ascii++ = 45; --size; /* '-' sign for exponent */
                  uexp_b10 = 0U-exp_b10;
               }
               else
                  uexp_b10 = 0U+exp_b10;
               cdigits = 0;
               while (uexp_b10 > 0)
               {
                  exponent[cdigits++] = (char)(48 + uexp_b10 % 10);
                  uexp_b10 /= 10;
               }
            }
            if (size > cdigits)
            {
               while (cdigits > 0) *ascii++ = exponent[--cdigits];
               *ascii = 0;
               return;
            }
         }
      }
      else if (!(fp >= DBL_MIN))
      {
         *ascii++ = 48; /* '0' */
         *ascii = 0;
         return;
      }
      else
      {
         *ascii++ = 105; /* 'i' */
         *ascii++ = 110; /* 'n' */
         *ascii++ = 102; /* 'f' */
         *ascii = 0;
         return;
      }
   }
   /* Here on buffer too small. */
   png_error(png_ptr, "ASCII conversion buffer too small");
}
}

// ---------------------------
// End of FOCAL_METHOD DEPENDENCIES
// ---------------------------

// ---------------------------
// Test Harness
// ---------------------------

static bool test_passed = true;

#define REQUIRE(cond, msg) do { \
    if (!(cond)) { \
        test_passed = false; \
        std::cerr << "FAILED: " << msg << "\n"; \
    } \
} while (0)

void test_BufferTooSmall_throws()
{
    // Purpose: Ensure that when the output buffer is too small, the focal
    // function signals an error via png_error (translated here to exception).
    char buf[16];
    // Fill to observe no memory corruption beyond buffer
    std::memset(buf, 0xAA, sizeof(buf));

    bool caught = false;
    try {
        // Force the "size >= precision+5" check to fail to trigger the error path.
        // Using a small buffer and a modest precision should trigger the error.
        png_ascii_from_fp(nullptr, buf, 2, 3.0, 6);
    } catch (const PngError& e) {
        caught = true;
        // Confirm the error message relates to buffer too small
        const char* msg = e.what();
        REQUIRE(msg != nullptr, "Error message should be non-null");
        REQUIRE(std::string(msg).find("buffer too small") != std::string::npos,
                "Error message should indicate buffer too small");
    } catch (...) {
        test_passed = false;
        std::cerr << "FAILED: Unexpected exception type for buffer-too-small case\n";
    }
    REQUIRE(caught, "png_error should have been invoked for small buffer");
}

void test_Negative_fp_starts_with_minus()
{
    // Purpose: When fp is negative, the output should begin with a minus sign.
    char buf[256];
    std::memset(buf, 0, sizeof(buf));
    // This should exercise the negative path and produce a signed number.
    png_ascii_from_fp(nullptr, buf, sizeof(buf), -12.34, 6);
    REQUIRE(std::strlen(buf) > 0, "Output buffer should be non-empty for negative fp");
    REQUIRE(buf[0] == '-', "Output should start with '-' for negative numbers");
}

void test_NanInf_outputs_inf()
{
    // Purpose: For NaN, the code should produce "inf" instead of a numeric value.
    char buf[16];
    std::memset(buf, 0, sizeof(buf));

    double nan_value = std::numeric_limits<double>::quiet_NaN();
    png_ascii_from_fp(nullptr, buf, sizeof(buf), nan_value, 6);

    // Expect first characters to be 'i','n','f','\0'
    REQUIRE(buf[0] == 'i', "Output should start with 'i' for NaN/Inf case");
    REQUIRE(buf[1] == 'n', "Second character should be 'n' for NaN/Inf case");
    REQUIRE(buf[2] == 'f', "Third character should be 'f' for NaN/Inf case");
    REQUIRE(buf[3] == '\0' || buf[3] == 0, "Terminating null expected after 'inf'");
}

void test_Normal_fp_output_not_crashing()
{
    // Purpose: Ensure a normal positive FP value does not crash and yields a terminated string.
    char buf[256];
    std::memset(buf, 0, sizeof(buf));

    double value = 1.234567;
    png_ascii_from_fp(nullptr, buf, sizeof(buf), value, 6);

    // The buffer should be a null-terminated string and not empty
    REQUIRE(std::strlen(buf) > 0, "Normal FP should produce a non-empty ASCII representation");
    // The first character should typically be a digit (or a minus if negative, which it's not here)
    REQUIRE((buf[0] >= '0' && buf[0] <= '9') || buf[0] == '-', "First character should be a digit or minus");
}

// Simple test runner
int main()
{
    std::cout << "Running unit tests for png_ascii_from_fp (self-contained test harness)...\n";

    test_BufferTooSmall_throws();
    test_Negative_fp_starts_with_minus();
    test_NanInf_outputs_inf();
    test_Normal_fp_output_not_crashing();

    if (test_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed. See above for details.\n";
        return 1;
    }
}