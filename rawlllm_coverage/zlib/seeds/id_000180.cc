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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers via preprocessor directives so this function is self-contained
    // (Preprocessor directives can appear anywhere; they are handled before compilation.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    const char *src = "The quick brown fox jumps over the lazy dog";
    const uLong src_len = static_cast<uLong>(std::strlen(src) + 1); // include terminating NUL for exact round-trip

    // 1) Deflate (stream) -> Inflate (stream) sequence
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    defstrm.zalloc = Z_NULL;
    defstrm.zfree = Z_NULL;
    defstrm.opaque = Z_NULL;

    bool def_initialized = false;
    bool inf_initialized = false;

    if (deflateInit(&defstrm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 1;
    }
    def_initialized = true;

    // allocate worst-case output buffer for compressed data
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstrm.avail_in = static_cast<uInt>(src_len);
    defstrm.next_out = comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(comp_buf.size());

    int def_ret = deflate(&defstrm, Z_FINISH);
    if (def_ret != Z_STREAM_END) {
        if (def_initialized) deflateEnd(&defstrm);
        return 2;
    }
    uLong comp_size = comp_buf.size() - defstrm.avail_out;
    // end the deflate stream
    if (deflateEnd(&defstrm) != Z_OK) {
        return 3;
    }
    def_initialized = false;

    // Inflate the data compressed above
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    infstrm.zalloc = Z_NULL;
    infstrm.zfree = Z_NULL;
    infstrm.opaque = Z_NULL;
    if (inflateInit(&infstrm) != Z_OK) {
        return 4;
    }
    inf_initialized = true;

    std::vector<unsigned char> decomp_buf(src_len); // we expect original size
    infstrm.next_in = comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(comp_size);
    infstrm.next_out = decomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(decomp_buf.size());

    int inf_ret = inflate(&infstrm, Z_FINISH);
    if (inf_ret != Z_STREAM_END) {
        if (inf_initialized) inflateEnd(&infstrm);
        return 5;
    }
    // finalize inflate
    if (inflateEnd(&infstrm) != Z_OK) {
        return 6;
    }
    inf_initialized = false;

    // verify round-trip equality (including terminating NUL)
    if (std::memcmp(decomp_buf.data(), src, src_len) != 0) {
        return 7;
    }

    // 2) compress2 / uncompress (convenience API)
    uLongf comp2_bound = compressBound(src_len);
    std::vector<unsigned char> comp2_buf(comp2_bound);
    uLongf comp2_size = comp2_bound;
    if (compress2(comp2_buf.data(), &comp2_size, reinterpret_cast<const Bytef*>(src), src_len, Z_BEST_COMPRESSION) != Z_OK) {
        return 8;
    }

    std::vector<unsigned char> uncomp_buf(src_len);
    uLongf uncomp_size = src_len;
    if (uncompress(uncomp_buf.data(), &uncomp_size, comp2_buf.data(), comp2_size) != Z_OK) {
        return 9;
    }
    if (uncomp_size != src_len) {
        return 10;
    }
    if (std::memcmp(uncomp_buf.data(), src, src_len) != 0) {
        return 11;
    }

    // 3) checksum helpers: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src), src_len);

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(src), src_len);

    // basic sanity check: checksums should be non-zero for this data set (extremely unlikely to be zero)
    if (crc == 0 || adl == 0) {
        return 12;
    }

    // If we reached here, all zlib API sequences succeeded
    return 66;
}