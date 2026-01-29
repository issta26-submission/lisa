#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when copied into a single file.
    // Preprocessor includes are allowed anywhere; they are handled before compilation.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input data to compress and then decompress.
    const std::string src = "The quick brown fox jumps over the lazy dog. "
                            "This is a test string for zlib compression/decompression. "
                            "Repeat the sentence a few times to increase size. "
                            "The quick brown fox jumps over the lazy dog. "
                            "The quick brown fox jumps over the lazy dog.";

    // Prepare z_stream for compression (deflate)
    z_stream def;
    std::memset(&def, 0, sizeof(def));
    int ret = deflateInit(&def, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // Failed to initialize deflate
        return ret;
    }

    // Set input for compression
    def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
    def.avail_in = static_cast<uInt>(src.size()); // safe because src.size() is small here

    // Allocate output buffer large enough using compressBound
    uLong bound = compressBound(static_cast<uLong>(src.size()));
    std::vector<unsigned char> compBuf(bound);
    def.next_out = compBuf.data();
    def.avail_out = static_cast<uInt>(bound);

    // Perform compression; since buffer is large enough, a single call with Z_FINISH is fine
    ret = deflate(&def, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Either need more output or some error occurred
        deflateEnd(&def);
        return ret;
    }
    // Actual compressed size
    size_t compSize = def.total_out;
    deflateEnd(&def);

    // Prepare z_stream for decompression (inflate)
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    ret = inflateInit(&inf);
    if (ret != Z_OK) {
        return ret;
    }

    // Set input to the compressed data
    inf.next_in = compBuf.data();
    inf.avail_in = static_cast<uInt>(compSize);

    // Prepare output buffer expecting original size
    std::vector<unsigned char> decompBuf(src.size() + 1); // +1 for safety
    inf.next_out = decompBuf.data();
    inf.avail_out = static_cast<uInt>(decompBuf.size());

    // Perform decompression
    ret = inflate(&inf, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&inf);
        return ret;
    }
    size_t decompSize = inf.total_out;
    inflateEnd(&inf);

    // Validate that decompressed size and contents match the original
    if (decompSize != src.size()) {
        return -1; // size mismatch
    }
    if (std::memcmp(decompBuf.data(), src.data(), src.size()) != 0) {
        return -2; // content mismatch
    }

    // All good; return required success code
    return 66;
}