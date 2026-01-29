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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of common zlib APIs:
    // - zlibVersion()
    // - compressBound(), compress2(), uncompress()
    // - adler32(), crc32()
    // - deflateInit(), deflate(), deflateEnd()
    // - inflateInit(), inflate(), inflateEnd()
    //
    // It validates round-trips and returns 66 on success, non-66 on failure.

    // Prepare sample data (repeated to make compression effective).
    const char *sample = "The quick brown fox jumps over the lazy dog. ";
    std::string orig;
    for (int i = 0; i < 64; ++i) orig += sample; // make data sizable
    uLong orig_size = static_cast<uLong>(orig.size());

    // Query zlib version (basic API usage)
    const char *ver = zlibVersion();
    if (!ver) return 1;

    // 1) Simple one-shot compress/uncompress using compress2/uncompress
    uLong bound = compressBound(orig_size);
    std::vector<Bytef> comp_buf(bound);
    uLong comp_size = bound;
    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef *>(orig.data()), orig_size,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 2;
    comp_buf.resize(comp_size);

    std::vector<Bytef> decomp_buf(orig_size);
    uLong decomp_size = orig_size;
    ret = uncompress(decomp_buf.data(), &decomp_size, comp_buf.data(), comp_size);
    if (ret != Z_OK) return 3;
    if (decomp_size != orig_size) return 4;
    if (std::memcmp(decomp_buf.data(), orig.data(), orig_size) != 0) return 5;

    // 2) Compute checksums (adler32 and crc32)
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef *>(orig.data()), orig_size);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef *>(orig.data()), orig_size);
    (void)adler; // values computed for demonstration; not asserted further
    (void)crc;

    // 3) Streaming deflate (deflateInit / deflate / deflateEnd)
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 6;
    dstream.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(orig.data()));
    dstream.avail_in = orig_size;
    std::vector<Bytef> streamed_comp(compressBound(orig_size));
    dstream.next_out = streamed_comp.data();
    dstream.avail_out = static_cast<uInt>(streamed_comp.size());
    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&dstream); return 7; }
    uLong streamed_comp_size = dstream.total_out;
    deflateEnd(&dstream);
    streamed_comp.resize(streamed_comp_size);

    // 4) Streaming inflate (inflateInit / inflate / inflateEnd)
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    ret = inflateInit(&istream);
    if (ret != Z_OK) return 8;
    istream.next_in = streamed_comp.data();
    istream.avail_in = static_cast<uInt>(streamed_comp_size);
    std::vector<Bytef> streamed_decomp(orig_size);
    istream.next_out = streamed_decomp.data();
    istream.avail_out = static_cast<uInt>(streamed_decomp.size());
    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&istream); return 9; }
    uLong streamed_decomp_size = istream.total_out;
    inflateEnd(&istream);
    if (streamed_decomp_size != orig_size) return 10;
    if (std::memcmp(streamed_decomp.data(), orig.data(), orig_size) != 0) return 11;

    // If all checks passed, return the success code requested.
    return 66;
}