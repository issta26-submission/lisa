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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here to make this function self-contained if inserted into a single file.
    // (Including inside the function is allowed by the preprocessor.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    const char* src = "The quick brown fox jumps over the lazy dog. zlib test string. "
                      "Repeat to increase size: The quick brown fox jumps over the lazy dog. "
                      "End of test.";
    size_t src_len = std::strlen(src);

    // Prepare deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstream.avail_in = static_cast<uInt>(src_len);

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1;
    }
    bool def_inited = true;

    // Output buffer for compressed data (grow if needed)
    std::vector<unsigned char> comp;
    comp.resize(256);
    defstream.next_out = comp.data();
    defstream.avail_out = static_cast<uInt>(comp.size());

    // Perform compression
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return 2;
        }
        // Need more output space
        if (defstream.avail_out == 0) {
            size_t old = comp.size();
            comp.resize(old * 2);
            defstream.next_out = comp.data() + old;
            defstream.avail_out = static_cast<uInt>(comp.size() - old);
        }
    }

    size_t comp_size = comp.size() - defstream.avail_out;

    // Clean up deflate
    if (def_inited) {
        deflateEnd(&defstream);
        def_inited = false;
    }

    // Prepare inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = comp.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 3;
    }
    bool inf_inited = true;

    // Output buffer for decompressed data (grow if needed)
    std::vector<unsigned char> decomp;
    decomp.resize(256);
    infstream.next_out = decomp.data();
    infstream.avail_out = static_cast<uInt>(decomp.size());

    // Perform decompression
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret == Z_OK) {
            // If we filled the output buffer, expand it
            if (infstream.avail_out == 0) {
                size_t old = decomp.size();
                decomp.resize(old * 2);
                infstream.next_out = decomp.data() + old;
                infstream.avail_out = static_cast<uInt>(decomp.size() - old);
            } else {
                // Need more input but none left -> error
                inflateEnd(&infstream);
                return 4;
            }
            continue;
        }
        // Other errors
        inflateEnd(&infstream);
        return 5;
    }

    size_t decomp_size = decomp.size() - infstream.avail_out;

    // Clean up inflate
    if (inf_inited) {
        inflateEnd(&infstream);
        inf_inited = false;
    }

    // Verify decompressed data matches original
    if (decomp_size != src_len) {
        return 6;
    }
    if (std::memcmp(decomp.data(), src, src_len) != 0) {
        return 7;
    }

    // Success: return 66 as requested
    return 66;
}