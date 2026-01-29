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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Local includes so the function is self-contained for demonstration purposes.
    #include <zlib.h>
    #include <vector>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    // Sample input data
    const char* input = "The quick brown fox jumps over the lazy dog (zlib test)";
    size_t input_len = std::strlen(input);

    // 1) Version check
    const char* ver = zlibVersion();
    if (!ver) return 1;

    // 2) Compute checksums (adler32 and crc32)
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    // 3) Simple (one-shot) compress/uncompress using compress()/uncompress()
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp_buf(bound);
    uLong comp_len = bound;
    int ret = compress(comp_buf.data(), &comp_len,
                       reinterpret_cast<const Bytef*>(input),
                       static_cast<uLong>(input_len));
    if (ret != Z_OK) return 2;

    std::vector<Bytef> uncomp_buf(input_len);
    uLong uncom_len = static_cast<uLong>(input_len);
    ret = uncompress(uncomp_buf.data(), &uncom_len,
                     comp_buf.data(), comp_len);
    if (ret != Z_OK) return 3;
    if (uncom_len != input_len) return 4;
    if (std::memcmp(uncomp_buf.data(), input, input_len) != 0) return 5;

    // 4) Streaming compression with deflateInit/deflate/deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 6;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);

    std::vector<Bytef> def_out_buf(compressBound(static_cast<uLong>(input_len)));
    defstream.next_out = def_out_buf.data();
    defstream.avail_out = static_cast<uInt>(def_out_buf.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 7;
    }
    uLong deflated_size = defstream.total_out;
    deflateEnd(&defstream);

    // 5) Streaming decompression with inflateInit/inflate/inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 8;

    infstream.next_in = def_out_buf.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);

    std::vector<Bytef> inflate_out_buf(input_len);
    infstream.next_out = inflate_out_buf.data();
    infstream.avail_out = static_cast<uInt>(inflate_out_buf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 9;
    }
    inflateEnd(&infstream);

    if (std::memcmp(inflate_out_buf.data(), input, input_len) != 0) return 10;

    // 6) Use compress2 to show alternate API (different compression level)
    std::vector<Bytef> comp2_buf(compressBound(static_cast<uLong>(input_len)));
    uLong comp2_len = static_cast<uLong>(comp2_buf.size());
    ret = compress2(comp2_buf.data(), &comp2_len,
                    reinterpret_cast<const Bytef*>(input),
                    static_cast<uLong>(input_len),
                    Z_BEST_SPEED);
    if (ret != Z_OK) return 11;

    // 7) Demonstrate deflateParams (init -> change params -> end)
    z_stream def2;
    std::memset(&def2, 0, sizeof(def2));
    ret = deflateInit(&def2, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 12;
    ret = deflateParams(&def2, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        deflateEnd(&def2);
        return 13;
    }
    deflateEnd(&def2);

    // 8) gz* APIs: write and read back using gzopen/gzwrite/gzread/gzclose
    const char* fname = "zlib_test_tmp.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return 14;
    int written = gzwrite(gz, input, static_cast<unsigned int>(input_len));
    if (written != static_cast<int>(input_len)) {
        gzclose(gz);
        std::remove(fname);
        return 15;
    }
    gzclose(gz);

    gz = gzopen(fname, "rb");
    if (!gz) {
        std::remove(fname);
        return 16;
    }
    std::vector<char> gz_read_buf(input_len);
    int readbytes = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(input_len));
    gzclose(gz);
    std::remove(fname);
    if (readbytes != static_cast<int>(input_len)) return 17;
    if (std::memcmp(gz_read_buf.data(), input, input_len) != 0) return 18;

    // 9) CRC of compressed data (example of crc32 usage on compressed bytes)
    uLong crc_comp = crc32(0L, Z_NULL, 0);
    crc_comp = crc32(crc_comp, comp_buf.data(), static_cast<uInt>(comp_len));
    (void)crc_comp; // used to demonstrate API; value not asserted here

    // All API sequences succeeded
    return 66;
}