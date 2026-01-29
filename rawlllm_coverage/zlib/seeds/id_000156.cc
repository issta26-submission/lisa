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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Compress a small buffer with deflate() and then decompress with inflate().
// Returns 66 on success, other non-zero codes on errors.
int test_zlib_api_sequence() {
    const char* input = "The quick brown fox jumps over the lazy dog";
    const uLong input_len = static_cast<uLong>(std::strlen(input) + 1); // include terminating NUL

    // --- deflate (compress) setup ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1;

    // Get an initial upper bound for compressed size
    uLong out_bound = deflateBound(&defstream, input_len);
    Bytef* comp = static_cast<Bytef*>(std::malloc(out_bound));
    if (!comp) { deflateEnd(&defstream); return 2; }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = input_len;
    defstream.next_out = comp;
    defstream.avail_out = out_bound;

    // Perform compression, expanding output buffer if needed
    ret = deflate(&defstream, Z_FINISH);
    while (ret == Z_OK) {
        // need more output space
        uLong used = out_bound - defstream.avail_out;
        uLong new_bound = out_bound * 2;
        Bytef* newbuf = static_cast<Bytef*>(std::realloc(comp, new_bound));
        if (!newbuf) {
            std::free(comp);
            deflateEnd(&defstream);
            return 3;
        }
        comp = newbuf;
        defstream.next_out = comp + used;
        defstream.avail_out = new_bound - used;
        out_bound = new_bound;
        ret = deflate(&defstream, Z_FINISH);
    }

    if (ret != Z_STREAM_END) { // compression didn't finish properly
        std::free(comp);
        deflateEnd(&defstream);
        return 4;
    }

    uLong compressed_size = out_bound - defstream.avail_out;

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) { std::free(comp); return 5; }

    // --- inflate (decompress) setup ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) { std::free(comp); return 6; }

    infstream.next_in = comp;
    infstream.avail_in = compressed_size;

    // Allocate expected output buffer (original size + small slack)
    uLong out_alloc = input_len + 16;
    Bytef* decomp = static_cast<Bytef*>(std::malloc(out_alloc));
    if (!decomp) { inflateEnd(&infstream); std::free(comp); return 7; }

    infstream.next_out = decomp;
    infstream.avail_out = out_alloc;

    // Perform decompression, expanding output buffer if needed
    ret = inflate(&infstream, Z_FINISH);
    while (ret == Z_OK) {
        uLong used = out_alloc - infstream.avail_out;
        uLong new_out = out_alloc * 2;
        Bytef* newbuf = static_cast<Bytef*>(std::realloc(decomp, new_out));
        if (!newbuf) {
            std::free(decomp);
            inflateEnd(&infstream);
            std::free(comp);
            return 8;
        }
        decomp = newbuf;
        infstream.next_out = decomp + used;
        infstream.avail_out = new_out - used;
        out_alloc = new_out;
        ret = inflate(&infstream, Z_FINISH);
    }

    if (ret != Z_STREAM_END) {
        std::free(decomp);
        inflateEnd(&infstream);
        std::free(comp);
        return 9;
    }

    uLong decompressed_size = out_alloc - infstream.avail_out;

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        std::free(decomp);
        std::free(comp);
        return 10;
    }

    // Verify round-trip equality (including the terminating NUL)
    if (decompressed_size != input_len || std::memcmp(decomp, input, input_len) != 0) {
        std::free(decomp);
        std::free(comp);
        return 11;
    }

    // Clean up and report success
    std::free(decomp);
    std::free(comp);
    return 66;
}