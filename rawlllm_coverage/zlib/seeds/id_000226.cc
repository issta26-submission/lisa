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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress/decompress
    const std::string original = std::string(
        "The quick brown fox jumps over the lazy dog. "
        "Pack my box with five dozen liquor jugs. "
        "Sphinx of black quartz, judge my vow. "
        "This is some sample text to test zlib APIs. "
    ) + std::string(256, 'A'); // make it a bit larger

    const size_t orig_size = original.size();

    // 1) Test compress2 + uncompress (simple single-call APIs)
    {
        // allocate buffer using compressBound
        uLong src_len = static_cast<uLong>(orig_size);
        uLong bound = compressBound(src_len);
        std::vector<Bytef> comp_buf(bound);

        uLongf comp_len = bound;
        int ret = compress2(comp_buf.data(), &comp_len,
                            reinterpret_cast<const Bytef*>(original.data()), src_len,
                            Z_BEST_COMPRESSION);
        if (ret != Z_OK) return 1;

        // uncompress back
        std::vector<Bytef> uncmp_buf(src_len);
        uLongf uncmp_len = src_len;
        ret = uncompress(uncmp_buf.data(), &uncmp_len, comp_buf.data(), comp_len);
        if (ret != Z_OK) return 2;

        if (uncmp_len != src_len) return 3;
        if (std::memcmp(uncmp_buf.data(), original.data(), src_len) != 0) return 4;
    }

    // 2) Test streaming deflate + inflate (stateful APIs)
    std::vector<unsigned char> deflated_stream;
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 5;

        defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original.data()));
        defstream.avail_in = static_cast<uInt>(orig_size);

        const size_t CHUNK = 128;
        unsigned char out[CHUNK];

        // First, feed input and finish
        int flush = defstream.avail_in ? Z_NO_FLUSH : Z_FINISH;
        while (true) {
            defstream.next_out = out;
            defstream.avail_out = CHUNK;
            // if there is input left, Z_NO_FLUSH; else request finish
            ret = deflate(&defstream, defstream.avail_in ? Z_NO_FLUSH : Z_FINISH);
            if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return 6; }

            size_t have = CHUNK - defstream.avail_out;
            deflated_stream.insert(deflated_stream.end(), out, out + have);

            if (ret == Z_STREAM_END) break;
            // If no more input, loop will call with Z_FINISH until Z_STREAM_END
        }

        deflateEnd(&defstream);
    }

    // Inflate the stream
    std::vector<unsigned char> inflated_stream;
    {
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        int ret = inflateInit(&infstream);
        if (ret != Z_OK) return 7;

        infstream.next_in = deflated_stream.empty() ? nullptr : deflated_stream.data();
        infstream.avail_in = static_cast<uInt>(deflated_stream.size());

        const size_t CHUNK = 128;
        unsigned char out[CHUNK];

        while (true) {
            infstream.next_out = out;
            infstream.avail_out = CHUNK;
            ret = inflate(&infstream, Z_NO_FLUSH);
            if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&infstream);
                return 8;
            }

            size_t have = CHUNK - infstream.avail_out;
            inflated_stream.insert(inflated_stream.end(), out, out + have);

            if (ret == Z_STREAM_END) break;
            if (infstream.avail_in == 0 && have == 0) break; // nothing more to do
        }

        inflateEnd(&infstream);
    }

    if (inflated_stream.size() != orig_size) return 9;
    if (std::memcmp(inflated_stream.data(), original.data(), orig_size) != 0) return 10;

    // 3) crc32 and adler32 computations
    {
        uLong crc_initial = crc32(0L, Z_NULL, 0);
        uLong crc_val = crc32(crc_initial,
                              reinterpret_cast<const Bytef*>(original.data()),
                              static_cast<uInt>(orig_size));
        if (crc_val == 0) {
            // improbable but check API call succeeded
            return 11;
        }

        uLong adl_initial = adler32(0L, Z_NULL, 0);
        uLong adl_val = adler32(adl_initial,
                                reinterpret_cast<const Bytef*>(original.data()),
                                static_cast<uInt>(orig_size));
        if (adl_val == 0) return 12;
    }

    // 4) gz* file APIs (write then read)
    const char* gz_fname = "test_zlib_api_sequence_temp.gz";
    {
        gzFile gz = gzopen(gz_fname, "wb");
        if (!gz) return 13;
        int wrote = gzwrite(gz, original.data(), static_cast<unsigned int>(orig_size));
        if (wrote == 0) { gzclose(gz); return 14; }
        gzclose(gz);
    }
    {
        gzFile gz = gzopen(gz_fname, "rb");
        if (!gz) { std::remove(gz_fname); return 15; }

        std::vector<char> readbuf;
        const size_t CHUNK = 128;
        char tmp[CHUNK];
        int r = 0;
        while ((r = gzread(gz, tmp, CHUNK)) > 0) {
            readbuf.insert(readbuf.end(), tmp, tmp + r);
        }
        if (r < 0) { gzclose(gz); std::remove(gz_fname); return 16; }
        gzclose(gz);

        if (readbuf.size() != orig_size) { std::remove(gz_fname); return 17; }
        if (std::memcmp(readbuf.data(), original.data(), orig_size) != 0) { std::remove(gz_fname); return 18; }
    }
    // remove temporary file
    std::remove(gz_fname);

    // All checks passed
    return 66;
}