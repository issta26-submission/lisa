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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary headers (allowed inside a function for a self-contained snippet)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input to compress and then decompress
    std::string input = "The quick brown fox jumps over the lazy dog. "
                        "This string will be compressed using zlib and then decompressed.";

    // --- Compression using zlib deflate APIs ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1;

    uLong src_len = static_cast<uLong>(input.size());
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> compbuf(static_cast<size_t>(bound));

    // zlib expects Bytef* for next_in
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(src_len);
    defstream.next_out = compbuf.data();
    defstream.avail_out = static_cast<uInt>(compbuf.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 2;
    }

    uLong comp_size = static_cast<uLong>(compbuf.size()) - defstream.avail_out;
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return 3;

    // --- Decompression using zlib inflate APIs ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 4;

    std::vector<unsigned char> decompbuf(static_cast<size_t>(src_len)); // we know the original size
    infstream.next_in = compbuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decompbuf.data();
    infstream.avail_out = static_cast<uInt>(decompbuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 5;
    }

    uLong decomp_size = static_cast<uLong>(decompbuf.size()) - infstream.avail_out;
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) return 6;

    // Verify round-trip equality
    if (decomp_size != src_len) return 7;
    if (std::memcmp(decompbuf.data(), input.data(), static_cast<size_t>(src_len)) != 0) return 8;

    // All good
    return 66;
}