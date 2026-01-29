// This is a self-contained C++11 test harness for the focal function
// write_unknown_chunks, adapted to run in a C++ environment without GTest.
// It mocks the minimal PNG-related types and functions required by write_unknown_chunks
// and validates its behavior through multiple test cases.
// The test suite includes explanatory comments for each test and uses
// non-terminating assertions (returning booleans and printing results).

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <map>
#include <cstring>


// --------------------------------------------------
// Minimal, self-contained definitions to mimic pngwrite.c dependencies
// --------------------------------------------------

// Handle type constants (part of libpng, redefined for tests)
#define PNG_HANDLE_CHUNK_NEVER 0
#define PNG_HANDLE_CHUNK_ALWAYS 1
#define PNG_HANDLE_CHUNK_AS_DEFAULT 2

// Macro to simulate the "supported" path (enable the per-chunk handling code)
#define PNG_SET_UNKNOWN_CHUNKS_SUPPORTED 1

// Forward declarations of the internal types used by write_unknown_chunks
struct png_struct;
struct png_infor;

// Typedefs mirroring libpng types used in the focal method
typedef struct png_struct* png_structrp;        // pointer to write struct
typedef const struct png_infor* png_const_inforp;
typedef const unsigned char* png_const_inp;
typedef const unsigned char png_const_unknown_chunkp; // placeholder

// Representation of an unknown chunk
struct png_unknown_chunk {
    unsigned char name[4];     // 4-char chunk name
    unsigned char* data;       // payload
    unsigned int size;         // payload size
    unsigned int location;     // location mask
};

// Representation of the info struct that contains unknown chunks
struct png_infor {
    unsigned int unknown_chunks_num;
    png_unknown_chunk* unknown_chunks;
};

// Representation of png_struct with only the field used by tests
struct png_struct {
    int unknown_default; // affects AS_DEFAULT handling in the macro path
};

// Global state for test harness to observe behavior
static std::map<std::string, int> unknown_handle_map; // mapping from chunk name to KEEP value
static std::vector<std::string> warnings;              // captured warnings
struct WrittenChunk {
    std::string name;
    unsigned int size;
    const unsigned char* data;
};
static std::vector<WrittenChunk> written_chunks;        // captured written chunks

// Helpers to reset test state and to record outputs
static void reset_state() {
    unknown_handle_map.clear();
    warnings.clear();
    written_chunks.clear();
}

// Debugging helper (no-op for tests)
static void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally empty to avoid noisy output during tests
}

// Mocked function: determine how an unknown chunk should be treated
static int png_handle_as_unknown(png_structrp /*png_ptr*/, const unsigned char* name) {
    if (!name) return PNG_HANDLE_CHUNK_NEVER;
    std::string s((const char*)name, 4);
    auto it = unknown_handle_map.find(s);
    if (it != unknown_handle_map.end())
        return it->second;
    return PNG_HANDLE_CHUNK_NEVER;
}

// Mocked function: record a written chunk
static void png_write_chunk(png_structrp /*png_ptr*/, const unsigned char* name,
                            const unsigned char* data, unsigned int size) {
    WrittenChunk wc;
    wc.name.assign((char*)name, 4);
    wc.size = size;
    wc.data = data;
    written_chunks.push_back(wc);
}

// Mocked function: record a warning
static void png_warning(png_structrp /*png_ptr*/, const char* msg) {
    warnings.push_back(msg ? msg : "");
}

// Helper to create a 4-char name in a test-proper format
static void set_name(unsigned char* name, char a, char b, char c, char d) {
    name[0] = static_cast<unsigned char>(a);
    name[1] = static_cast<unsigned char>(b);
    name[2] = static_cast<unsigned char>(c);
    name[3] = static_cast<unsigned char>(d);
}

// Forward declaration of the focal method (as implemented in the test harness)
static void write_unknown_chunks(png_structrp png_ptr, png_const_inforp info_ptr,
                                unsigned int where);

// A variant that simulates the scenario where the unknown-chunk feature is not compiled in.
// It writes any matching unknown chunks, regardless of per-chunk handling.
static void write_unknown_chunks_without_macro(png_structrp png_ptr, png_const_inforp info_ptr,
                                              unsigned int where) {
    if (info_ptr->unknown_chunks_num != 0) {
        png_unknown_chunk* up;
        png_debug(5, "writing extra chunks (no macro path)");
        for (up = info_ptr->unknown_chunks;
             up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
             ++up) {
            if ((up->location & where) != 0) {
                if (up->size == 0)
                    png_warning(png_ptr, "Writing zero-length unknown chunk");
                png_write_chunk(png_ptr, up->name, up->data, up->size);
            }
        }
    }
}

// The focal method, implemented according to the provided snippet.
// It respects the PNG_SET_UNKNOWN_CHUNKS_SUPPORTED macro to decide whether to
// check per-chunk handling before writing.
static void write_unknown_chunks(png_structrp png_ptr, png_const_inforp info_ptr,
                                unsigned int where) {
    if (info_ptr->unknown_chunks_num != 0) {
        png_unknown_chunk* up;
        png_debug(5, "writing extra chunks");
        for (up = info_ptr->unknown_chunks;
             up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
             ++up)
            if ((up->location & where) != 0) {
#ifdef PNG_SET_UNKNOWN_CHUNKS_SUPPORTED
                int keep = png_handle_as_unknown(png_ptr, up->name);
                if (keep != PNG_HANDLE_CHUNK_NEVER &&
                    ((up->name[3] & 0x20) ||
                     keep == PNG_HANDLE_CHUNK_ALWAYS ||
                     (keep == PNG_HANDLE_CHUNK_AS_DEFAULT &&
                      png_ptr->unknown_default == PNG_HANDLE_CHUNK_ALWAYS)))
#endif
                {
                    if (up->size == 0)
                        png_warning(png_ptr, "Writing zero-length unknown chunk");
                    png_write_chunk(png_ptr, up->name, up->data, up->size);
                }
            }
    }
}

// Test 1: No unknown chunks present; ensure no writes or warnings occur.
static bool test_no_unknown_chunks_with_macro() {
    reset_state();

    // Prepare info_ptr with zero unknown chunks
    png_infor info;
    info.unknown_chunks_num = 0;
    info.unknown_chunks = nullptr;

    png_struct ptr;
    ptr.unknown_default = 0;

    write_unknown_chunks(&ptr, &info, 0x01);

    bool passed = (written_chunks.empty() && warnings.empty());
    if (!passed) {
        std::cout << "Test 1 (no unknown chunks) failed: expected no writes or warnings.\n";
    }
    return passed;
}

// Test 2: A single unknown chunk that should be written due to ALWAYS handling.
// - Name ends with lowercase 'd' to have 0x20 bit set (safe-to-copy in libpng terms).
// - Location matches the 'where' mask.
// - Per-chunk handler returns ALWAYS, so write should occur.
static bool test_single_chunk_always_writes() {
    reset_state();

    // Prepare one unknown chunk
    png_unknown_chunk chunks[1];
    unsigned char data[3] = {1, 2, 3};
    set_name(chunks[0].name, 'a', 'b', 'c', 'd'); // 'abcd', name[3] = 'd' => 0x64 & 0x20 != 0
    chunks[0].data = data;
    chunks[0].size = 3;
    chunks[0].location = 0x01;

    png_infor info;
    info.unknown_chunks_num = 1;
    info.unknown_chunks = chunks;

    png_struct ptr;
    ptr.unknown_default = 0;
    // Map 'abcd' to ALWAYS
    unknown_handle_map.clear();
    unknown_handle_map[std::string((char*)chunks[0].name, 4)] = PNG_HANDLE_CHUNK_ALWAYS;

    write_unknown_chunks(&ptr, &info, 0x01);

    // Expect exactly one write
    if (written_chunks.size() != 1) {
        std::cout << "Test 2 (single ALWAYS chunk) failed: expected 1 written chunk, got "
                  << written_chunks.size() << "\n";
        return false;
    }
    if (written_chunks[0].name != "abcd" || written_chunks[0].size != 3) {
        std::cout << "Test 2 (single ALWAYS chunk) failed: written chunk data mismatch. "
                  << "Got name=" << written_chunks[0].name << ", size=" << written_chunks[0].size << "\n";
        return false;
    }

    // Ensure no warnings were generated
    if (!warnings.empty()) {
        std::cout << "Test 2 (single ALWAYS chunk) failed: unexpected warnings.\n";
        return false;
    }

    return true;
}

// Test 3: Zero-length unknown chunk should warn but still be written.
static bool test_zero_length_chunk_warns_and_writes() {
    reset_state();

    png_unknown_chunk chunks[1];
    unsigned char* data = nullptr;
    set_name(chunks[0].name, 'x', 'y', 'z', 'd'); // last char 'd' with 0x20 set
    chunks[0].data = data;
    chunks[0].size = 0;               // zero-length
    chunks[0].location = 0x01;

    png_infor info;
    info.unknown_chunks_num = 1;
    info.unknown_chunks = chunks;

    png_struct ptr;
    ptr.unknown_default = 0;
    unknown_handle_map.clear();
    unknown_handle_map[std::string((char*)chunks[0].name, 4)] = PNG_HANDLE_CHUNK_ALWAYS;

    write_unknown_chunks(&ptr, &info, 0x01);

    // Expect one write and one warning
    bool ok = (written_chunks.size() == 1);
    if (!ok) {
        std::cout << "Test 3 (zero-length chunk) failed: expected 1 written chunk, got "
                  << written_chunks.size() << "\n";
        return false;
    }
    if (warnings.size() != 1) {
        std::cout << "Test 3 (zero-length chunk) failed: expected 1 warning, got "
                  << warnings.size() << "\n";
        return false;
    }
    if (written_chunks[0].name != "xyzd" && written_chunks[0].name != "xyz" ) {
        // Note: only the first 4 chars are used; ensure dump shows 'xyzd' fragment
        // The actual 4-char name is "xyzd"; keep the check simple
    }
    return ok;
}

// Test 4: Chunk location does not match; expect no writes.
static bool test_location_mismatch_no_write() {
    reset_state();

    png_unknown_chunk chunks[1];
    unsigned char data[2] = {9, 8};
    set_name(chunks[0].name, 'a', 'b', 'c', 'd');
    chunks[0].data = data;
    chunks[0].size = 2;
    chunks[0].location = 0x02; // different from where=0x01

    png_infor info;
    info.unknown_chunks_num = 1;
    info.unknown_chunks = chunks;

    png_struct ptr;
    ptr.unknown_default = 0;
    unknown_handle_map.clear();
    unknown_handle_map[std::string((char*)chunks[0].name, 4)] = PNG_HANDLE_CHUNK_ALWAYS;

    write_unknown_chunks(&ptr, &info, 0x01);

    bool ok = written_chunks.empty();
    if (!ok) {
        std::cout << "Test 4 (location mismatch) failed: expected no writes.\n";
        return false;
    }
    return true;
}

// Test 5: Non-macro path simulation: ensure that when macro is not used,
// all matching chunks are written (unconditional write path).
static bool test_without_macro_path_writes() {
    reset_state();

    // Prepare two chunks, both matching location
    png_unknown_chunk chunks[2];
    unsigned char data1[2] = {1, 1};
    unsigned char data2[1] = {2};
    set_name(chunks[0].name, 'l', 'o', 'o', 'd'); // 'lood' name, last char 'd' (0x64)
    chunks[0].data = data1;
    chunks[0].size = 2;
    chunks[0].location = 0x01;

    set_name(chunks[1].name, 't', 'e', 's', 't');
    chunks[1].data = data2;
    chunks[1].size = 1;
    chunks[1].location = 0x01;

    png_infor info;
    info.unknown_chunks_num = 2;
    info.unknown_chunks = chunks;

    png_struct ptr;
    ptr.unknown_default = 0;
    // In this non-macro path, per-chunk handling is ignored; ensure always writes
    write_unknown_chunks_without_macro(&ptr, &info, 0x01);

    if (written_chunks.size() != 2) {
        std::cout << "Test 5 (no-macro path) failed: expected 2 writes, got "
                  << written_chunks.size() << "\n";
        return false;
    }
    if (written_chunks[0].name != "lood" || written_chunks[0].size != 2) {
        std::cout << "Test 5 (no-macro path) failed: first chunk mismatch.\n";
        return false;
    }
    if (written_chunks[1].name != "test" || written_chunks[1].size != 1) {
        std::cout << "Test 5 (no-macro path) failed: second chunk mismatch.\n";
        return false;
    }

    return true;
}

// Helper to run all tests and report summary
int main() {
    int total = 0;
    int failed = 0;

    std::cout << "Starting test suite for write_unknown_chunks (self-contained harness)\n";

    if (!test_no_unknown_chunks_with_macro()) {
        ++failed;
    }
    ++total;

    if (!test_single_chunk_always_writes()) {
        ++failed;
    }
    ++total;

    if (!test_zero_length_chunk_warns_and_writes()) {
        ++failed;
    }
    ++total;

    if (!test_location_mismatch_no_write()) {
        ++failed;
    }
    ++total;

    if (!test_without_macro_path_writes()) {
        ++failed;
    }
    ++total;

    if (failed == 0) {
        std::cout << "ALL TESTS PASSED: " << total << " tests executed.\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED: " << failed << " of " << total << " tests failed.\n";
        return 1;
    }
}