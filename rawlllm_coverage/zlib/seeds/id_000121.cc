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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct zlib usage sequence:
    //  - deflateInit / deflate (loop, expanding output buffer as needed) / deflateEnd
    //  - inflateInit / inflate (loop, expanding output buffer as needed) / inflateEnd
    // It compresses a sample string, decompresses it, and verifies equality.
    //
    // Returns 66 on success, other negative values indicate errors.

    const char *original = "The quick brown fox jumps over the lazy dog";
    size_t original_len = strlen(original);

    // --- Compression (deflate) ---
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    defstrm.zalloc = Z_NULL;
    defstrm.zfree = Z_NULL;
    defstrm.opaque = Z_NULL;

    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -1;

    // input
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstrm.avail_in = static_cast<uInt>(original_len);

    // output buffer (growable)
    const size_t CHUNK = 128;
    std::vector<unsigned char> comp;
    comp.resize(CHUNK);
    defstrm.next_out = comp.data();
    defstrm.avail_out = static_cast<uInt>(comp.size());

    // Loop until deflate signals Z_STREAM_END
    while (true) {
        ret = deflate(&defstrm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) { deflateEnd(&defstrm); return -2; }

        // Need more output space
        if (defstrm.avail_out == 0) {
            size_t old_size = comp.size();
            comp.resize(old_size + CHUNK);
            defstrm.next_out = comp.data() + old_size;
            defstrm.avail_out = static_cast<uInt>(CHUNK);
        } else {
            // Shouldn't normally reach here without finishing, but continue loop.
        }
    }

    // Calculate compressed size
    size_t compressed_size = comp.size() - defstrm.avail_out;

    ret = deflateEnd(&defstrm);
    if (ret != Z_OK) return -3;

    // --- Decompression (inflate) ---
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    infstrm.zalloc = Z_NULL;
    infstrm.zfree = Z_NULL;
    infstrm.opaque = Z_NULL;

    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return -4;

    // input = compressed data
    infstrm.next_in = comp.data();
    infstrm.avail_in = static_cast<uInt>(compressed_size);

    // output buffer (start with a reasonable size, grow if needed)
    std::vector<unsigned char> decomp;
    decomp.resize(original_len + CHUNK); // likely enough, can expand
    infstrm.next_out = decomp.data();
    infstrm.avail_out = static_cast<uInt>(decomp.size());

    while (true) {
        ret = inflate(&infstrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            // Z_BUF_ERROR can occur when more output space is needed; handle by expanding
            if (ret == Z_BUF_ERROR) {
                // fall through to expand buffer below
            } else {
                inflateEnd(&infstrm);
                return -5;
            }
        }

        if (infstrm.avail_out == 0) {
            size_t old_size = decomp.size();
            decomp.resize(old_size + CHUNK);
            infstrm.next_out = decomp.data() + old_size;
            infstrm.avail_out = static_cast<uInt>(CHUNK);
        } else {
            // If inflate returned Z_OK but still has available output, continue
            // (loop will continue until Z_STREAM_END)
        }
    }

    size_t decompressed_size = decomp.size() - infstrm.avail_out;

    ret = inflateEnd(&infstrm);
    if (ret != Z_OK) return -6;

    // Verify decompressed contents equal the original
    if (decompressed_size != original_len) return -7;
    if (memcmp(decomp.data(), original, original_len) != 0) return -8;

    // All zlib operations succeeded and data matches
    return 66;
}