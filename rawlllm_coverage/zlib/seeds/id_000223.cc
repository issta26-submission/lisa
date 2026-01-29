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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers (preprocessor directives are allowed here).
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Sample input data (repeated to make it compressible)
    const std::string input = std::string("Hello zlib! ") + std::string(50, 'A') + " End.";
    const uLong input_len = static_cast<uLong>(input.size());

    // 1) Simple one-shot compress() / uncompress() using compressBound()
    {
        uLong bound = compressBound(input_len);
        std::vector<Bytef> comp(bound);
        uLong comp_len = bound;

        int ret = compress(comp.data(), &comp_len, reinterpret_cast<const Bytef*>(input.data()), input_len);
        if (ret != Z_OK) return 1; // fail

        std::vector<Bytef> decomp(input_len + 10); // small margin
        uLong decomp_len = static_cast<uLong>(decomp.size());
        ret = uncompress(decomp.data(), &decomp_len, comp.data(), comp_len);
        if (ret != Z_OK) return 2; // fail

        if (decomp_len != input_len) return 3;
        if (std::memcmp(decomp.data(), input.data(), input_len) != 0) return 4;
    }

    // 2) compress2 with explicit compression level
    {
        uLong bound = compressBound(input_len);
        std::vector<Bytef> comp(bound);
        uLong comp_len = bound;

        int ret = compress2(comp.data(), &comp_len, reinterpret_cast<const Bytef*>(input.data()), input_len, Z_BEST_COMPRESSION);
        if (ret != Z_OK) return 5;

        std::vector<Bytef> decomp(input_len);
        uLong decomp_len = static_cast<uLong>(decomp.size());
        ret = uncompress(decomp.data(), &decomp_len, comp.data(), comp_len);
        if (ret != Z_OK) return 6;

        if (decomp_len != input_len) return 7;
        if (std::memcmp(decomp.data(), input.data(), input_len) != 0) return 8;
    }

    // 3) Checksums: crc32 and adler32
    {
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), input_len);
        if (crc == 0) {
            // crc could be zero for some inputs but unlikely here; just keep it as a used API.
        }

        uLong adl = adler32(0L, Z_NULL, 0);
        adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), input_len);
        (void)adl; // used to demonstrate adler32
    }

    // 4) Streaming deflate / inflate (no dictionary)
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 9;

        // Provide input
        defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
        defstream.avail_in = static_cast<uInt>(input_len);

        // Prepare output buffer (use compressBound as a safe maximum)
        uLong out_bound = compressBound(input_len);
        std::vector<Bytef> outbuf(out_bound);
        defstream.next_out = outbuf.data();
        defstream.avail_out = static_cast<uInt>(out_bound);

        ret = deflate(&defstream, Z_FINISH);
        if (!(ret == Z_STREAM_END || ret == Z_OK)) {
            deflateEnd(&defstream);
            return 10;
        }
        uLong compressed_size = out_bound - defstream.avail_out;
        deflateEnd(&defstream);

        // Inflate
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        ret = inflateInit(&infstream);
        if (ret != Z_OK) return 11;

        infstream.next_in = outbuf.data();
        infstream.avail_in = static_cast<uInt>(compressed_size);

        std::vector<Bytef> recovered(input_len + 10);
        infstream.next_out = recovered.data();
        infstream.avail_out = static_cast<uInt>(recovered.size());

        ret = inflate(&infstream, Z_FINISH);
        if (!(ret == Z_STREAM_END || ret == Z_OK)) {
            inflateEnd(&infstream);
            return 12;
        }
        uLong recovered_size = recovered.size() - infstream.avail_out;
        inflateEnd(&infstream);

        if (recovered_size != input_len) return 13;
        if (std::memcmp(recovered.data(), input.data(), input_len) != 0) return 14;
    }

    // 5) Dictionary-based deflate / inflate demonstrating deflateSetDictionary and inflateSetDictionary
    {
        const std::string dict = "COMMON_DICTIONARY_DATA_1234567890";
        const uInt dict_len = static_cast<uInt>(dict.size());

        // Compressor side
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 15;

        // Set dictionary before first deflate call
        ret = deflateSetDictionary(&defstream, reinterpret_cast<const Bytef*>(dict.data()), dict_len);
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return 16;
        }

        defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
        defstream.avail_in = static_cast<uInt>(input_len);

        uLong out_bound = compressBound(input_len);
        std::vector<Bytef> outbuf(out_bound);
        defstream.next_out = outbuf.data();
        defstream.avail_out = static_cast<uInt>(out_bound);

        ret = deflate(&defstream, Z_FINISH);
        if (!(ret == Z_STREAM_END || ret == Z_OK)) {
            deflateEnd(&defstream);
            return 17;
        }
        uLong compressed_size = out_bound - defstream.avail_out;
        deflateEnd(&defstream);

        // Decompressor side
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        ret = inflateInit(&infstream);
        if (ret != Z_OK) return 18;

        infstream.next_in = outbuf.data();
        infstream.avail_in = static_cast<uInt>(compressed_size);

        std::vector<Bytef> recovered(input_len + 10);
        infstream.next_out = recovered.data();
        infstream.avail_out = static_cast<uInt>(recovered.size());

        // First inflate call is expected to return Z_NEED_DICT because compressor used a dictionary
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT) {
            // Provide the same dictionary
            ret = inflateSetDictionary(&infstream, reinterpret_cast<const Bytef*>(dict.data()), dict_len);
            if (ret != Z_OK) {
                inflateEnd(&infstream);
                return 19;
            }
            // continue inflation
            ret = inflate(&infstream, Z_FINISH);
        }
        if (!(ret == Z_STREAM_END || ret == Z_OK)) {
            inflateEnd(&infstream);
            return 20;
        }
        uLong recovered_size = recovered.size() - infstream.avail_out;
        inflateEnd(&infstream);

        if (recovered_size != input_len) return 21;
        if (std::memcmp(recovered.data(), input.data(), input_len) != 0) return 22;
    }

    // If all sequences succeeded
    return 66;
}