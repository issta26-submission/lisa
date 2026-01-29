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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // sample input
    const char* input = "The quick brown fox jumps over the lazy dog. zlib API sequence test.";
    uLong input_len = static_cast<uLong>(strlen(input)); // number of bytes (no null)

    // 1) Simple one-shot compress/uncompress using compress()/uncompress()
    uLong bound = compressBound(input_len);
    std::vector<Bytef> comp1(bound);
    uLongf comp1_len = bound;
    int ret = compress(comp1.data(), &comp1_len, reinterpret_cast<const Bytef*>(input), input_len);
    if (ret != Z_OK) return 1;

    std::vector<Bytef> decomp1(input_len);
    uLongf decomp1_len = input_len;
    ret = uncompress(decomp1.data(), &decomp1_len, comp1.data(), comp1_len);
    if (ret != Z_OK) return 2;
    if (decomp1_len != input_len) return 3;
    if (std::memcmp(decomp1.data(), input, input_len) != 0) return 4;

    // 2) Compute checksums: CRC32 and Adler-32
    uLong crc = crc32(0L, Z_NULL, 0); // initial
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), input_len);

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), input_len);

    // Simple sanity: checksums should be non-zero for non-empty input
    if (crc == 0 || adl == 0) return 5;

    // 3) Streaming compress with deflate() / deflateEnd() using z_stream
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 6;

    // Demonstrate deflateBound with initialized stream
    uLong def_bound = deflateBound(&strm, input_len);
    std::vector<Bytef> comp2(def_bound);

    strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    strm.avail_in = static_cast<uInt>(input_len);
    strm.next_out = comp2.data();
    strm.avail_out = static_cast<uInt>(comp2.size());

    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&strm); return 7; }
    uLong comp2_len = strm.total_out;
    deflateEnd(&strm);

    // 4) Streaming inflate with inflate() / inflateEnd()
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;

    ret = inflateInit(&istrm);
    if (ret != Z_OK) return 8;

    std::vector<Bytef> decomp2(input_len);
    istrm.next_in = comp2.data();
    istrm.avail_in = static_cast<uInt>(comp2_len);
    istrm.next_out = decomp2.data();
    istrm.avail_out = static_cast<uInt>(decomp2.size());

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&istrm); return 9; }
    uLong decomp2_len = istrm.total_out;
    inflateEnd(&istrm);

    if (decomp2_len != input_len) return 10;
    if (std::memcmp(decomp2.data(), input, input_len) != 0) return 11;

    // 5) Demonstrate gzip wrapping with deflateInit2 / inflateInit2
    z_stream gzstrm;
    std::memset(&gzstrm, 0, sizeof(gzstrm));
    gzstrm.zalloc = Z_NULL;
    gzstrm.zfree = Z_NULL;
    gzstrm.opaque = Z_NULL;

    // windowBits = 15 + 16 to write a gzip header instead of zlib header
    ret = deflateInit2(&gzstrm, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) return 12;

    uLong gz_bound = deflateBound(&gzstrm, input_len);
    std::vector<Bytef> comp_gz(gz_bound);

    gzstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    gzstrm.avail_in = static_cast<uInt>(input_len);
    gzstrm.next_out = comp_gz.data();
    gzstrm.avail_out = static_cast<uInt>(comp_gz.size());

    ret = deflate(&gzstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&gzstrm); return 13; }
    uLong comp_gz_len = gzstrm.total_out;
    deflateEnd(&gzstrm);

    // Inflate with automatic header detection (gzip/zlib) using 15 + 32
    z_stream igz;
    std::memset(&igz, 0, sizeof(igz));
    igz.zalloc = Z_NULL;
    igz.zfree = Z_NULL;
    igz.opaque = Z_NULL;

    ret = inflateInit2(&igz, 15 + 32);
    if (ret != Z_OK) return 14;

    std::vector<Bytef> out_gz(input_len);
    igz.next_in = comp_gz.data();
    igz.avail_in = static_cast<uInt>(comp_gz_len);
    igz.next_out = out_gz.data();
    igz.avail_out = static_cast<uInt>(out_gz.size());

    ret = inflate(&igz, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&igz); return 15; }
    uLong out_gz_len = igz.total_out;
    inflateEnd(&igz);

    if (out_gz_len != input_len) return 16;
    if (std::memcmp(out_gz.data(), input, input_len) != 0) return 17;

    // All zlib API sequences used above succeeded and validated
    return 66;
}