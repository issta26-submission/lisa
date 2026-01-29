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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when inserted into a file.
    // Note: #include directives are valid anywhere in a translation unit.
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Prepare input data (small, deterministic, somewhat compressible)
    const std::string input_base = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 8; ++i) input += input_base; // repeat to increase size

    // Use uLong to match zlib API types
    uLong in_len = static_cast<uLong>(input.size());

    // Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), in_len);

    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input.data()), in_len);

    // 1) Simple one-shot compress() / uncompress() sequence
    uLong bound = compressBound(in_len);
    std::vector<Bytef> comp_buf(bound);
    uLong comp_len = bound;
    int ret = compress(comp_buf.data(), &comp_len,
                       reinterpret_cast<const Bytef*>(input.data()), in_len);
    if (ret != Z_OK) return 1; // compress failed

    std::vector<Bytef> decomp_buf(in_len);
    uLong decomp_len = in_len;
    ret = uncompress(decomp_buf.data(), &decomp_len,
                     comp_buf.data(), comp_len);
    if (ret != Z_OK) return 2; // uncompress failed
    if (decomp_len != in_len) return 3;
    if (std::memcmp(decomp_buf.data(), input.data(), in_len) != 0) return 4;

    // 2) Streaming deflate (z_stream + deflateInit / deflate / deflateEnd)
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    defstrm.next_in = const_cast<Bytef*>(
        reinterpret_cast<const Bytef*>(input.data()));
    defstrm.avail_in = in_len;

    // Ensure enough output space using compressBound
    uLong def_out_bound = compressBound(in_len);
    std::vector<Bytef> def_out(def_out_bound);
    defstrm.next_out = def_out.data();
    defstrm.avail_out = static_cast<uInt>(def_out.size());

    // Use Z_FINISH to finish in one (sized) call; check for Z_STREAM_END
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up and report error
        deflateEnd(&defstrm);
        return 6;
    }
    size_t deflated_size = def_out.size() - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 3) Streaming inflate (z_stream + inflateInit / inflate / inflateEnd)
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    ret = inflateInit(&instrm);
    if (ret != Z_OK) return 7;

    instrm.next_in = def_out.data();
    instrm.avail_in = static_cast<uInt>(deflated_size);

    std::vector<Bytef> inf_out(in_len);
    instrm.next_out = inf_out.data();
    instrm.avail_out = static_cast<uInt>(inf_out.size());

    ret = inflate(&instrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&instrm);
        return 8;
    }
    size_t inflated_size = inf_out.size() - instrm.avail_out;
    inflateEnd(&instrm);

    if (inflated_size != in_len) return 9;
    if (std::memcmp(inf_out.data(), input.data(), in_len) != 0) return 10;

    // Extra validation: recompute checksums on decompressed streaming result and compare
    uLong crc2 = crc32(0L, Z_NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(inf_out.data()), static_cast<uInt>(inflated_size));
    if (crc2 != crc) return 11;

    uLong adler2 = adler32(0L, Z_NULL, 0);
    adler2 = adler32(adler2, reinterpret_cast<const Bytef*>(inf_out.data()), static_cast<uInt>(inflated_size));
    if (adler2 != adler) return 12;

    // All tests passed
    return 66;
}