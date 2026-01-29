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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/*
 * Compress a test string with deflate APIs and then decompress with inflate APIs.
 * Performs error checking and resource cleanup. Returns 66 on success, non-66 on failure.
 */
int test_zlib_api_sequence() {
    const char *original = "The quick brown fox jumps over the lazy dog. "
                           "This text will be compressed and then decompressed "
                           "to validate a correct zlib API sequence.";
    uLong orig_size = static_cast<uLong>(std::strlen(original));

    /* --- Compression (deflate) --- */
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    uLong bound = compressBound(orig_size);
    Bytef *comp_buf = static_cast<Bytef*>(std::malloc(bound));
    if (!comp_buf) {
        deflateEnd(&defstream);
        return -2;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstream.avail_in = static_cast<uInt>(orig_size);
    defstream.next_out = comp_buf;
    defstream.avail_out = static_cast<uInt>(bound);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        std::free(comp_buf);
        deflateEnd(&defstream);
        return -3;
    }
    uLong comp_size = defstream.total_out;

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return -4;
    }

    /* --- Decompression (inflate) --- */
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return -5;
    }

    Bytef *out_buf = static_cast<Bytef*>(std::malloc(orig_size));
    if (!out_buf) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        return -6;
    }

    infstream.next_in = comp_buf;
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = out_buf;
    infstream.avail_out = static_cast<uInt>(orig_size);

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        std::free(out_buf);
        std::free(comp_buf);
        inflateEnd(&infstream);
        return -7;
    }
    uLong out_size = infstream.total_out;

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        std::free(out_buf);
        std::free(comp_buf);
        return -8;
    }

    /* --- Verify decompressed data matches original --- */
    bool match = (out_size == orig_size) &&
                 (std::memcmp(out_buf, original, orig_size) == 0);

    std::free(out_buf);
    std::free(comp_buf);

    if (!match) {
        return -9;
    }

    /* Success */
    return 66;
}