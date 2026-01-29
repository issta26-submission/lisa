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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers needed for this function
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>
    #include <cstdio>

    // Sample input data
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test string to exercise zlib APIs. "
                              "Repeat: The quick brown fox jumps over the lazy dog.";

    // 1) Use compressBound + compress2 (simple one-shot API)
    uLong src_len = static_cast<uLong>(input.size());
    uLong bound = compressBound(src_len);
    std::vector<Bytef> comp_buf(bound);

    uLongf comp_size = bound;
    int c_ret = compress2(comp_buf.data(), &comp_size,
                          reinterpret_cast<const Bytef*>(input.data()), src_len,
                          Z_BEST_COMPRESSION);
    if (c_ret != Z_OK) {
        return 1;
    }
    comp_buf.resize(comp_size);

    // 2) Uncompress with uncompress (one-shot)
    std::vector<Bytef> decomp_buf(src_len);
    uLongf decomp_size = src_len;
    int u_ret = uncompress(decomp_buf.data(), &decomp_size,
                           comp_buf.data(), comp_size);
    if (u_ret != Z_OK || decomp_size != src_len) {
        return 2;
    }
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) {
        return 3;
    }

    // 3) Stream-based deflate (deflateInit / deflate / deflateEnd)
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    dstream.zalloc = Z_NULL;
    dstream.zfree = Z_NULL;
    dstream.opaque = Z_NULL;

    if (deflateInit(&dstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 4;
    }

    dstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    dstream.avail_in = src_len;

    uLongf def_out_bound = compressBound(src_len);
    std::vector<Bytef> def_out(def_out_bound);
    dstream.next_out = def_out.data();
    dstream.avail_out = def_out_bound;

    int def_ret = deflate(&dstream, Z_FINISH);
    if (def_ret != Z_STREAM_END) {
        deflateEnd(&dstream);
        return 5;
    }
    uLongf deflated_size = def_out_bound - dstream.avail_out;
    deflateEnd(&dstream);
    def_out.resize(deflated_size);

    // 4) Stream-based inflate (inflateInit / inflate / inflateEnd)
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    istream.zalloc = Z_NULL;
    istream.zfree = Z_NULL;
    istream.opaque = Z_NULL;

    if (inflateInit(&istream) != Z_OK) {
        return 6;
    }

    istream.next_in = def_out.data();
    istream.avail_in = static_cast<uInt>(def_out.size());

    std::vector<Bytef> infl_out(src_len);
    istream.next_out = infl_out.data();
    istream.avail_out = static_cast<uInt>(infl_out.size());

    // inflate in a loop until stream end
    int inf_ret = Z_OK;
    for (;;) {
        inf_ret = inflate(&istream, Z_NO_FLUSH);
        if (inf_ret == Z_STREAM_END) {
            break;
        } else if (inf_ret != Z_OK) {
            inflateEnd(&istream);
            return 7;
        }
        if (istream.avail_out == 0) {
            // output buffer too small (shouldn't happen here), fail-safe
            inflateEnd(&istream);
            return 8;
        }
    }
    uLongf inflated_size = infl_out.size() - istream.avail_out;
    inflateEnd(&istream);
    if (inflated_size != src_len) {
        return 9;
    }
    if (std::memcmp(infl_out.data(), input.data(), src_len) != 0) {
        return 10;
    }

    // 5) CRC32 and Adler-32 checks
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), src_len);

    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, reinterpret_cast<const Bytef*>(infl_out.data()), inflated_size);

    if (crc_orig != crc_decomp) {
        return 11;
    }

    uLong adl_orig = adler32(0L, Z_NULL, 0);
    adl_orig = adler32(adl_orig, reinterpret_cast<const Bytef*>(input.data()), src_len);

    uLong adl_decomp = adler32(0L, Z_NULL, 0);
    adl_decomp = adler32(adl_decomp, reinterpret_cast<const Bytef*>(infl_out.data()), inflated_size);

    if (adl_orig != adl_decomp) {
        return 12;
    }

    // 6) gzFile API: write compressed data to a .gz file and read it back
    const char* fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) {
        return 13;
    }
    int wrote = gzwrite(gz, input.data(), static_cast<unsigned int>(input.size()));
    if (wrote != static_cast<int>(input.size())) {
        gzclose(gz);
        remove(fname);
        return 14;
    }
    gzclose(gz);

    gzFile gz_r = gzopen(fname, "rb");
    if (!gz_r) {
        remove(fname);
        return 15;
    }
    std::vector<char> gz_read(input.size());
    int read_bytes = gzread(gz_r, gz_read.data(), static_cast<unsigned int>(gz_read.size()));
    gzclose(gz_r);
    remove(fname);

    if (read_bytes != static_cast<int>(input.size())) {
        return 16;
    }
    if (std::memcmp(gz_read.data(), input.data(), input.size()) != 0) {
        return 17;
    }

    // If we reached here, all API sequences exercised worked as expected
    return 66;
}