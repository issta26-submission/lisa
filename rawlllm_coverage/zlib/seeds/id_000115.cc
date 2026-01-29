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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers (allowed inside a function; keeps this function self-contained).
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Sample input data (repeated to be compressible).
    const char* input_cstr = "The quick brown fox jumps over the lazy dog. "
                             "The quick brown fox jumps over the lazy dog. "
                             "The quick brown fox jumps over the lazy dog.";
    const uLong input_len = static_cast<uLong>(strlen(input_cstr));

    // 1) Use compressBound and compress2 (simple one-shot compression).
    uLong bound = compressBound(input_len);
    std::vector<Bytef> compBuf(bound);
    uLongf compSize = bound;
    int ret = compress2(compBuf.data(), &compSize,
                        reinterpret_cast<const Bytef*>(input_cstr), input_len,
                        Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1; // compress failed

    // 2) Use uncompress to get back the original data.
    std::vector<Bytef> uncompBuf(input_len);
    uLongf uncomprLen = input_len;
    ret = uncompress(uncompBuf.data(), &uncomprLen, compBuf.data(), compSize);
    if (ret != Z_OK) return 2; // uncompress failed
    if (uncomprLen != input_len) return 3; // length mismatch
    if (std::memcmp(uncompBuf.data(), input_cstr, input_len) != 0) return 4; // content mismatch

    // 3) Use raw deflate/inflate stream APIs (z_stream) to compress and decompress.
    z_stream deflate_stream;
    std::memset(&deflate_stream, 0, sizeof(deflate_stream));
    ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // prepare input for deflate
    deflate_stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input_cstr));
    deflate_stream.avail_in = static_cast<uInt>(input_len); // safe since input_len small here

    // output buffer large enough: use compressBound
    std::vector<Bytef> deflateOut(bound);
    deflate_stream.next_out = deflateOut.data();
    deflate_stream.avail_out = static_cast<uInt>(deflateOut.size());

    ret = deflate(&deflate_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&deflate_stream);
        return 6; // deflate failed
    }
    // total bytes produced:
    uLong deflated_size = deflate_stream.total_out;
    ret = deflateEnd(&deflate_stream);
    if (ret != Z_OK) return 7;

    // 4) Inflate the deflated data using z_stream inflate API.
    z_stream inflate_stream;
    std::memset(&inflate_stream, 0, sizeof(inflate_stream));
    ret = inflateInit(&inflate_stream);
    if (ret != Z_OK) return 8;

    inflate_stream.next_in = deflateOut.data();
    inflate_stream.avail_in = static_cast<uInt>(deflated_size);

    std::vector<Bytef> inflateOut(input_len);
    inflate_stream.next_out = inflateOut.data();
    inflate_stream.avail_out = static_cast<uInt>(inflateOut.size());

    ret = inflate(&inflate_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&inflate_stream);
        return 9; // inflate failed
    }
    uLong inflated_size = inflate_stream.total_out;
    ret = inflateEnd(&inflate_stream);
    if (ret != Z_OK) return 10;

    if (inflated_size != input_len) return 11;
    if (std::memcmp(inflateOut.data(), input_cstr, input_len) != 0) return 12;

    // 5) Compute checksums: crc32 and adler32 on the original data.
    uLong crc = crc32(0L, Z_NULL, 0); // initialize CRC
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input_cstr), static_cast<uInt>(input_len));
    uLong adler = adler32(0L, Z_NULL, 0); // initialize Adler-32
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input_cstr), static_cast<uInt>(input_len));
    // Basic sanity: checksums should be non-zero for this input.
    if (crc == 0 && adler == 0) return 13;

    // 6) Query zlib version string and ensure not null/empty.
    const char* ver = zlibVersion();
    if (!ver || ver[0] == '\0') return 14;

    // All checks passed: return 66 per specification.
    (void)crc;   // silence unused-variable warnings if any
    (void)adler;
    (void)ver;
    return 66;
}