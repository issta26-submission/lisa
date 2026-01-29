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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary includes (allowed inside the function for a self-contained snippet)
    #include <zlib.h>
    #include <vector>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    const char* src = "The quick brown fox jumps over the lazy dog";
    size_t src_len = strlen(src); // do not include trailing null in compressed data

    // 1) Simple compress/uncompress using compress2() and uncompress()
    uLongf bound = compressBound(static_cast<uLong>(src_len));
    std::vector<Bytef> comp_buf(bound);
    int ret = compress2(comp_buf.data(), &bound, reinterpret_cast<const Bytef*>(src),
                        static_cast<uLong>(src_len), Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;
    comp_buf.resize(bound);

    // Verify with uncompress()
    std::vector<Bytef> dec_buf(src_len);
    uLongf dec_len = static_cast<uLongf>(src_len);
    ret = uncompress(dec_buf.data(), &dec_len, comp_buf.data(), comp_buf.size());
    if (ret != Z_OK) return -2;
    if (dec_len != src_len) return -3;
    if (std::memcmp(dec_buf.data(), src, src_len) != 0) return -4;

    // 2) CRC32 and Adler32 checks
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));
    // Just ensure they return something non-zero for this data (not strictly necessary)
    if (crc == 0 || adl == 0) return -5;

    // 3) Deflate to produce gzip-wrapped output (deflateInit2 with windowBits = MAX_WBITS + 16)
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    ret = deflateInit2(&dstream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY); // gzip wrapper
    if (ret != Z_OK) return -6;

    dstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    dstream.avail_in = static_cast<uInt>(src_len);

    std::vector<Bytef> gz_comp;
    gz_comp.resize(1024);
    dstream.next_out = gz_comp.data();
    dstream.avail_out = static_cast<uInt>(gz_comp.size());

    while (dstream.avail_in > 0) {
        ret = deflate(&dstream, Z_NO_FLUSH);
        if (ret != Z_OK) {
            deflateEnd(&dstream);
            return -7;
        }
        if (dstream.avail_out == 0) {
            size_t old = gz_comp.size();
            gz_comp.resize(old * 2);
            dstream.next_out = gz_comp.data() + old;
            dstream.avail_out = static_cast<uInt>(old);
        }
    }
    // Finish
    for (;;) {
        ret = deflate(&dstream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            deflateEnd(&dstream);
            return -8;
        }
        if (dstream.avail_out == 0) {
            size_t old = gz_comp.size();
            gz_comp.resize(old * 2);
            dstream.next_out = gz_comp.data() + old;
            dstream.avail_out = static_cast<uInt>(old);
        }
    }
    size_t gz_comp_len = dstream.total_out;
    deflateEnd(&dstream);
    gz_comp.resize(gz_comp_len);

    // 4) Inflate the gzip-wrapped output (inflateInit2 with MAX_WBITS + 16)
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    ret = inflateInit2(&istream, MAX_WBITS + 16); // expect gzip wrapper
    if (ret != Z_OK) return -9;

    istream.next_in = gz_comp.data();
    istream.avail_in = static_cast<uInt>(gz_comp_len);

    std::vector<Bytef> gz_dec;
    gz_dec.resize(src_len);
    istream.next_out = gz_dec.data();
    istream.avail_out = static_cast<uInt>(gz_dec.size());

    for (;;) {
        ret = inflate(&istream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&istream);
            return -10;
        }
        if (istream.avail_out == 0) {
            size_t old = gz_dec.size();
            gz_dec.resize(old * 2);
            istream.next_out = gz_dec.data() + old;
            istream.avail_out = static_cast<uInt>(old);
        }
    }
    size_t gz_dec_len = istream.total_out;
    inflateEnd(&istream);

    if (gz_dec_len != src_len) return -11;
    if (std::memcmp(gz_dec.data(), src, src_len) != 0) return -12;

    // 5) gzFile API: write to a gzip file and read it back
    const char* fname = "test_zlib_api_sequence_temp.gz";
    gzFile gz_w = gzopen(fname, "wb");
    if (!gz_w) return -13;
    int written = gzwrite(gz_w, src, static_cast<unsigned int>(src_len));
    if (written != static_cast<int>(src_len)) {
        gzclose(gz_w);
        return -14;
    }
    gzclose(gz_w);

    gzFile gz_r = gzopen(fname, "rb");
    if (!gz_r) return -15;
    std::vector<char> file_read(src_len);
    int read = gzread(gz_r, file_read.data(), static_cast<unsigned int>(src_len));
    gzclose(gz_r);
    if (read != static_cast<int>(src_len)) return -16;
    if (std::memcmp(file_read.data(), src, src_len) != 0) return -17;

    // Remove temporary file (best-effort)
    std::remove(fname);

    // 6) zlibVersion()
    const char* version = zlibVersion();
    if (!version) return -18;

    // If all checks passed, return the required success code
    return 66;
}