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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained for copy-paste usage.
    // (Including headers inside a function is legal in C/C++ preprocessing.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    const char* input = "The quick brown fox jumps over the lazy dog. "
                        "This string will be compressed and decompressed "
                        "using several zlib APIs to exercise the library.";
    // include null terminator to check full equality later
    size_t input_len = std::strlen(input) + 1;

    // 1) compress() / uncompress()
    uLong bound = compressBound((uLong)input_len);
    std::vector<Bytef> comp_buf((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int ret = compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(input), (uLong)input_len);
    if (ret != Z_OK) return -1;

    std::vector<Bytef> uncmp_buf(input_len);
    uLongf uncmp_len = (uLongf)input_len;
    ret = uncompress(uncmp_buf.data(), &uncmp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return -2;
    if (uncmp_len != input_len) return -3;
    if (std::memcmp(uncmp_buf.data(), input, input_len) != 0) return -4;

    // 2) deflate (streaming) -> deflateInit, deflate, deflateEnd
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;
    defstrm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input));
    defstrm.avail_in = (uInt)input_len;
    // allocate enough space using compressBound
    std::vector<Bytef> deflated((size_t)compressBound((uLong)input_len));
    defstrm.next_out = deflated.data();
    defstrm.avail_out = (uInt)deflated.size();
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&defstrm); return -6; }
    size_t deflated_size = deflated.size() - defstrm.avail_out;
    ret = deflateEnd(&defstrm);
    if (ret != Z_OK) return -7;

    // 3) inflate (streaming) -> inflateInit, inflate, inflateEnd
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    instrm.next_in = deflated.data();
    instrm.avail_in = (uInt)deflated_size;
    ret = inflateInit(&instrm);
    if (ret != Z_OK) return -8;
    std::vector<Bytef> inflated(input_len);
    instrm.next_out = inflated.data();
    instrm.avail_out = (uInt)inflated.size();
    ret = inflate(&instrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&instrm); return -9; }
    size_t inflated_size = inflated.size() - instrm.avail_out;
    ret = inflateEnd(&instrm);
    if (ret != Z_OK) return -10;
    if (inflated_size != input_len) return -11;
    if (std::memcmp(inflated.data(), input, input_len) != 0) return -12;

    // 4) crc32 / adler32 checks
    uLong crc_input = crc32(0L, Z_NULL, 0);
    crc_input = crc32(crc_input, reinterpret_cast<const Bytef*>(input), (uInt)input_len);
    uLong crc_inflated = crc32(0L, Z_NULL, 0);
    crc_inflated = crc32(crc_inflated, inflated.data(), (uInt)inflated_size);
    if (crc_input != crc_inflated) return -13;

    uLong ad_input = adler32(0L, Z_NULL, 0);
    ad_input = adler32(ad_input, reinterpret_cast<const Bytef*>(input), (uInt)input_len);
    uLong ad_inflated = adler32(0L, Z_NULL, 0);
    ad_inflated = adler32(ad_inflated, inflated.data(), (uInt)inflated_size);
    if (ad_input != ad_inflated) return -14;

    // 5) gzopen / gzwrite / gzread / gzclose (file based)
    const char* fname = "test_zlib_api_sequence_temp.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return -15;
    int written = gzwrite(gz, input, (unsigned)input_len);
    if (written != (int)input_len) { gzclose(gz); std::remove(fname); return -16; }
    ret = gzclose(gz);
    if (ret != Z_OK) { std::remove(fname); return -17; }

    gz = gzopen(fname, "rb");
    if (!gz) { std::remove(fname); return -18; }
    std::vector<char> readback(input_len);
    int r = gzread(gz, readback.data(), (unsigned)input_len);
    if (r != (int)input_len) { gzclose(gz); std::remove(fname); return -19; }
    ret = gzclose(gz);
    if (ret != Z_OK) { std::remove(fname); return -20; }
    if (std::memcmp(readback.data(), input, input_len) != 0) { std::remove(fname); return -21; }
    // cleanup file
    std::remove(fname);

    // 6) Query zlib version string
    const char* ver = zlibVersion();
    if (!ver || ver[0] == '\0') return -22;

    // If all operations succeeded, return 66 as requested.
    return 66;
}