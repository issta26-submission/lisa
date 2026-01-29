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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Test a sequence of zlib API usages: checksum, simple compress/uncompress and streaming deflate/inflate.
// Returns 66 on success, non-66 on failure.
int test_zlib_api_sequence() {
    const char input_data[] = "The quick brown fox jumps over the lazy dog";
    const size_t input_len = strlen(input_data);

    // 1) Compute checksums on the original data
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input_data), static_cast<uInt>(input_len));

    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input_data), static_cast<uInt>(input_len));

    // 2) Use compress2 (simple API) to compress
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_len = bound;
    int ret = compress2(comp_buf.data(), &comp_len,
                        reinterpret_cast<const Bytef*>(input_data),
                        static_cast<uLong>(input_len),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // 3) Use uncompress to decompress and verify content and checksums
    std::vector<Bytef> decomp_buf(input_len);
    uLongf decomp_len = static_cast<uLongf>(input_len);
    ret = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK || decomp_len != input_len) {
        return -2;
    }
    if (std::memcmp(decomp_buf.data(), input_data, input_len) != 0) {
        return -3;
    }
    // verify checksums on decompressed data
    uLong adler_decomp = adler32(0L, Z_NULL, 0);
    adler_decomp = adler32(adler_decomp, decomp_buf.data(), static_cast<uInt>(decomp_len));
    if (adler_decomp != adler_orig) {
        return -4;
    }
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp_buf.data(), static_cast<uInt>(decomp_len));
    if (crc_decomp != crc_orig) {
        return -5;
    }

    // 4) Streaming deflate: compress using z_stream (incremental API)
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    defstrm.zalloc = Z_NULL;
    defstrm.zfree = Z_NULL;
    defstrm.opaque = Z_NULL;
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -6;
    }

    // Prepare output buffer with a safe upper bound
    size_t stream_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> stream_comp;
    stream_comp.resize(stream_bound);

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_data));
    defstrm.avail_in = static_cast<uInt>(input_len);
    defstrm.next_out = stream_comp.data();
    defstrm.avail_out = static_cast<uInt>(stream_comp.size());

    // Perform deflate with Z_FINISH; handle case when output buffer needs expansion
    while (true) {
        ret = deflate(&defstrm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret == Z_OK) {
            // Need more output space: expand buffer
            size_t used = stream_comp.size() - defstrm.avail_out;
            size_t new_size = stream_comp.size() * 2;
            stream_comp.resize(new_size);
            defstrm.next_out = stream_comp.data() + used;
            // avail_out might exceed uInt; cast safely (our sizes are small)
            defstrm.avail_out = static_cast<uInt>(stream_comp.size() - used);
            continue;
        } else {
            deflateEnd(&defstrm);
            return -7;
        }
    }
    size_t stream_comp_len = stream_comp.size() - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 5) Streaming inflate: decompress the stream produced above
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    infstrm.zalloc = Z_NULL;
    infstrm.zfree = Z_NULL;
    infstrm.opaque = Z_NULL;
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return -8;
    }

    std::vector<Bytef> stream_decomp;
    stream_decomp.resize(input_len); // we expect original size

    infstrm.next_in = stream_comp.data();
    infstrm.avail_in = static_cast<uInt>(stream_comp_len);
    infstrm.next_out = stream_decomp.data();
    infstrm.avail_out = static_cast<uInt>(stream_decomp.size());

    while (true) {
        ret = inflate(&infstrm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret == Z_OK) {
            // Need more output (shouldn't happen here because we sized output appropriately)
            size_t used = stream_decomp.size() - infstrm.avail_out;
            size_t new_size = stream_decomp.size() * 2;
            stream_decomp.resize(new_size);
            infstrm.next_out = stream_decomp.data() + used;
            infstrm.avail_out = static_cast<uInt>(stream_decomp.size() - used);
            continue;
        } else {
            inflateEnd(&infstrm);
            return -9;
        }
    }
    size_t stream_decomp_len = stream_decomp.size() - infstrm.avail_out;
    inflateEnd(&infstrm);

    if (stream_decomp_len != input_len) {
        return -10;
    }
    if (std::memcmp(stream_decomp.data(), input_data, input_len) != 0) {
        return -11;
    }

    // verify checksums again on the streaming-decompressed data
    uLong adler_stream = adler32(0L, Z_NULL, 0);
    adler_stream = adler32(adler_stream, stream_decomp.data(), static_cast<uInt>(stream_decomp_len));
    if (adler_stream != adler_orig) {
        return -12;
    }
    uLong crc_stream = crc32(0L, Z_NULL, 0);
    crc_stream = crc32(crc_stream, stream_decomp.data(), static_cast<uInt>(stream_decomp_len));
    if (crc_stream != crc_orig) {
        return -13;
    }

    // All tests passed
    return 66;
}