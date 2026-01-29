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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {

    // Sample input
    const char* input = "The quick brown fox jumps over the lazy dog. zlib test data.";
    size_t input_len = strlen(input);

    // 1) Query zlib version (basic usage)
    const char* ver = zlibVersion();
    (void)ver; // silence unused variable warning if not otherwise used

    // 2) Compute checksums
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    (void)crc; (void)adl; // we computed them to demonstrate usage

    // 3) Simple one-shot compress / uncompress
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp(bound);
    uLong comp_size = bound;
    int res = compress2(comp.data(), &comp_size, reinterpret_cast<const Bytef*>(input),
                        static_cast<uLong>(input_len), Z_BEST_COMPRESSION);
    if (res != Z_OK) return -1;

    std::vector<Bytef> uncompressed(input_len + 16);
    uLong un_size = static_cast<uLong>(uncompressed.size());
    res = uncompress(uncompressed.data(), &un_size, comp.data(), comp_size);
    if (res != Z_OK) return -2;
    if (un_size != input_len || memcmp(uncompressed.data(), input, input_len) != 0) return -3;

    // 4) Streaming deflate (z_stream) demonstration
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    res = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return -4;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    std::vector<Bytef> defout(compressBound(static_cast<uLong>(input_len)));
    defstream.next_out = defout.data();
    defstream.avail_out = static_cast<uInt>(defout.size());
    res = deflate(&defstream, Z_FINISH);
    if (res != Z_STREAM_END) { deflateEnd(&defstream); return -5; }
    size_t deflated_size = defout.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // 5) Streaming inflate (z_stream) demonstration
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    res = inflateInit(&infstream);
    if (res != Z_OK) return -6;
    infstream.next_in = defout.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);
    std::vector<Bytef> infout(input_len + 16);
    infstream.next_out = infout.data();
    infstream.avail_out = static_cast<uInt>(infout.size());
    res = inflate(&infstream, Z_FINISH);
    if (res != Z_STREAM_END) { inflateEnd(&infstream); return -7; }
    size_t inflated_size = infout.size() - infstream.avail_out;
    inflateEnd(&infstream);
    if (inflated_size != input_len || memcmp(infout.data(), input, input_len) != 0) return -8;

    // 6) gz* API: write to a .gz file then read back
    const char* gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) return -9;
    int written = gzwrite(gz, input, static_cast<unsigned int>(input_len));
    if (written == 0) { gzclose(gz); return -10; }
    gzclose(gz);

    gz = gzopen(gzname, "rb");
    if (!gz) return -11;
    std::vector<char> gzin(input_len + 16);
    int readbytes = gzread(gz, gzin.data(), static_cast<unsigned int>(gzin.size()));
    gzclose(gz);
    if (readbytes <= 0) return -12;
    if (static_cast<size_t>(readbytes) != input_len || memcmp(gzin.data(), input, input_len) != 0) return -13;

    // Clean up temporary file (ignore failure to remove)
    std::remove(gzname);

    // 7) Demonstrate deflateReset / inflateReset usage (init/reset/end)
    memset(&defstream, 0, sizeof(defstream));
    res = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return -14;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = defout.data();
    defstream.avail_out = static_cast<uInt>(defout.size());
    res = deflate(&defstream, Z_SYNC_FLUSH);
    if (res != Z_OK && res != Z_BUF_ERROR) { deflateEnd(&defstream); return -15; }
    res = deflateReset(&defstream);
    if (res != Z_OK) { deflateEnd(&defstream); return -16; }
    deflateEnd(&defstream);

    memset(&infstream, 0, sizeof(infstream));
    res = inflateInit(&infstream);
    if (res != Z_OK) return -17;
    res = inflateReset(&infstream);
    if (res != Z_OK) { inflateEnd(&infstream); return -18; }
    inflateEnd(&infstream);

    // If we reached this point, all tested API sequences succeeded
    return 66;
}