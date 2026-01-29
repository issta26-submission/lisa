#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Minimal scaffolding to simulate libpng-like environment for testing
typedef unsigned char png_byte;
typedef unsigned int png_uint_32;
typedef const unsigned char* png_const_bytep;
typedef unsigned char* png_bytep;
typedef const char* png_const_charp;
typedef char* png_charp;
typedef void* png_structrp;
typedef struct png_struct_s png_struct;
typedef png_struct* png_structpp;
typedef png_structrp png_structrp;
typedef unsigned long png_alloc_size_t;

static const png_uint_32 PNG_COMPRESSION_TYPE_BASE = 0;
static const png_uint_32 png_iCCP = 0x69434350; // 'iCCP'
static const int Z_OK = 0;

// Forward declarations for dependencies used by the focal function
struct z_stream_s { const char* msg; };
struct png_struct_s {
    z_stream_s zstream;
}; 
static void png_debug(int, const char*) {}
static void png_error(png_structrp png_ptr, const char* msg);

// Helper exception for error signaling in tests
class PngError : public std::exception {
public:
    explicit PngError(const std::string& m) : message(m) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

// Mocked dependencies (stubs) to drive the focal function behavior

static png_uint_32 png_get_uint_32(png_const_bytep p)
{
    // little-endian read
    return  (static_cast<png_uint_32>(p[0])
            | (static_cast<png_uint_32>(p[1]) << 8)
            | (static_cast<png_uint_32>(p[2]) << 16)
            | (static_cast<png_uint_32>(p[3]) << 24));
}

static png_uint_32 png_check_keyword(png_structrp png_ptr, png_const_charp name, png_byte* new_name)
{
    if (name == nullptr) return 0;
    size_t len = std::strlen(name);
    if (len == 0 || len > 80) return 0;
    // Copy keyword into new_name
    std::memcpy(new_name, name, len);
    return static_cast<png_uint_32>(len);
}

struct compression_state {
    png_uint_32 output_len;
};

static void png_text_compress_init(compression_state* comp, png_const_bytep input, png_alloc_size_t input_len)
{
    (void)input;
    (void)input_len;
    if (comp) comp->output_len = 0;
}

static int png_text_compress(png_structrp png_ptr, png_uint_32 chunk_name, compression_state* comp, png_uint_32 prefix_len)
{
    (void)png_ptr;
    (void)chunk_name;
    (void)prefix_len;
    if (comp) comp->output_len = 4; // pretend compressed data length
    return Z_OK;
}

static void png_write_chunk_header(png_structrp png_ptr, png_uint_32 chunk_name, png_uint_32 length)
{
    (void)png_ptr; (void)chunk_name; (void)length;
}
static void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length)
{
    (void)png_ptr; (void)data; (void)length;
}
static void png_write_compressed_data_out(png_structrp png_ptr, compression_state* comp)
{
    (void)png_ptr; (void)comp;
}
static void png_write_chunk_end(png_structrp png_ptr)
{
    (void)png_ptr;
}
static void png_debug_1()
{
    // helper to keep signature similar
}

// Public API used by focal method
static void png_error(png_structrp png_ptr, const char* msg)
{
    (void)png_ptr;
    throw PngError(msg);
}

// FOCAL_METHOD replicated (as given) for testing
png_uint_32 png_get_uint_32(png_const_bytep p);

png_write_iCCP(png_structrp png_ptr, png_const_charp name,
    png_const_bytep profile, png_uint_32 profile_len)
{
{
   png_uint_32 name_len;
   png_byte new_name[81]; /* 1 byte for the compression byte */
   compression_state comp;
   png_uint_32 temp;
   png_debug(1, "in png_write_iCCP");
   /* These are all internal problems: the profile should have been checked
    * before when it was stored.
    */
   if (profile == NULL)
      png_error(png_ptr, "No profile for iCCP chunk"); /* internal error */
   if (profile_len < 132)
      png_error(png_ptr, "ICC profile too short");
   if (png_get_uint_32(profile) != profile_len)
      png_error(png_ptr, "Incorrect data in iCCP");
   temp = (png_uint_32) (*(profile+8));
   if (temp > 3 && (profile_len & 0x03))
      png_error(png_ptr, "ICC profile length invalid (not a multiple of 4)");
   {
      png_uint_32 embedded_profile_len = png_get_uint_32(profile);
      if (profile_len != embedded_profile_len)
         png_error(png_ptr, "Profile length does not match profile");
   }
   name_len = png_check_keyword(png_ptr, name, new_name);
   if (name_len == 0)
      png_error(png_ptr, "iCCP: invalid keyword");
   new_name[++name_len] = PNG_COMPRESSION_TYPE_BASE;
   /* Make sure we include the NULL after the name and the compression type */
   ++name_len;
   png_text_compress_init(&comp, profile, profile_len);
   /* Allow for keyword terminator and compression byte */
   if (png_text_compress(png_ptr, png_iCCP, &comp, name_len) != Z_OK)
      png_error(png_ptr, png_ptr->zstream.msg);
   png_write_chunk_header(png_ptr, png_iCCP, name_len + comp.output_len);
   png_write_chunk_data(png_ptr, new_name, name_len);
   png_write_compressed_data_out(png_ptr, &comp);
   png_write_chunk_end(png_ptr);
}
}

// End of focal function

// Test harness

static bool test_no_profile()
{
    try {
        // profile is NULL -> should trigger error
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        png_write_iCCP(dummy, "keyword", static_cast<png_const_bytep>(nullptr), 100);
        // If we get here, it did not throw
        return false;
    } catch (const PngError& e) {
        return std::string(e.what()) == "No profile for iCCP chunk";
    } catch (...) {
        return false;
    }
}

static bool test_profile_too_short()
{
    try {
        // profile non-NULL but too short
        unsigned char profile[5] = {0};
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        png_write_iCCP(dummy, "keyword", profile, 131);
        return false;
    } catch (const PngError& e) {
        return std::string(e.what()) == "ICC profile too short";
    } catch (...) {
        return false;
    }
}

static bool test_incorrect_data_in_iCCP()
{
    try {
        // profile_len >= 132, but embedded length (first 4 bytes) does not match
        unsigned char profile[132];
        // embedded length set to 200 (0xC8 0x00 0x00 0x00 little-endian)
        profile[0] = 0xC8; profile[1] = 0x00; profile[2] = 0x00; profile[3] = 0x00;
        // ensure offset 8 byte has a small value to bypass later condition
        profile[8] = 0;
        // rest zeros
        std::memset(profile+4, 0, 128);
        // profile_len 132
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        png_write_iCCP(dummy, "validname", profile, 132);
        return false;
    } catch (const PngError& e) {
        // Expect incorrect data message
        return std::string(e.what()) == "Incorrect data in iCCP";
    } catch (...) {
        return false;
    }
}

static bool test_length_invalid_multiple()
{
    try {
        // temp > 3 and profile_len not multiple of 4
        unsigned char profile[132];
        // embedded length equals 132
        profile[0] = 132; profile[1] = 0; profile[2] = 0; profile[3] = 0;
        // set offset 8 to 4 to satisfy temp > 3
        profile[8] = 4;
        std::memset(profile+4, 0, 128);
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        // choose non-multiple-of-4 length
        png_write_iCCP(dummy, "validname", profile, 133);
        return false;
    } catch (const PngError& e) {
        return std::string(e.what()) == "ICC profile length invalid (not a multiple of 4)";
    } catch (...) {
        return false;
    }
}

static bool test_invalid_keyword()
{
    try {
        // profile valid, but keyword invalid (empty)
        unsigned char profile[132];
        // embed 132
        profile[0] = 132; profile[1] = 0; profile[2] = 0; profile[3] = 0;
        profile[8] = 0;
        std::memset(profile+4, 0, 128);
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        png_write_iCCP(dummy, "", profile, 132);
        return false;
    } catch (const PngError& e) {
        return std::string(e.what()) == "iCCP: invalid keyword";
    } catch (...) {
        return false;
    }
}

static bool test_ok_path()
{
    try {
        unsigned char profile[132];
        // embedded length equals profile_len
        profile[0] = 132; profile[1] = 0; profile[2] = 0; profile[3] = 0;
        // temp small
        profile[8] = 0;
        std::memset(profile+4, 0, 128);
        png_structrp dummy = reinterpret_cast<png_structrp>(nullptr);
        png_write_iCCP(dummy, "validname", profile, 132);
        // If no exception, consider OK
        return true;
    } catch (...) {
        return false;
    }
}

int main()
{
    struct TestInfo {
        const char* name;
        bool (*func)();
    };

    std::vector<TestInfo> tests = {
        {"test_no_profile", test_no_profile},
        {"test_profile_too_short", test_profile_too_short},
        {"test_incorrect_data_in_iCCP", test_incorrect_data_in_iCCP},
        {"test_length_invalid_multiple", test_length_invalid_multiple},
        {"test_invalid_keyword", test_invalid_keyword},
        {"test_ok_path", test_ok_path}
    };

    int passed = 0;
    for (const auto& t : tests) {
        bool res = t.func();
        std::cout << (res ? "[PASS] " : "[FAIL] ") << t.name << "\n";
        if (res) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << tests.size() << " tests passed.\n";
    return (passed == (int)tests.size()) ? 0 : 1;
}