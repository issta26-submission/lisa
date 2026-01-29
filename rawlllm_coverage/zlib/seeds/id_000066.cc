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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    const char* original = "The quick brown fox jumps over the lazy dog. "
                           "Repeat: The quick brown fox jumps over the lazy dog.";
    const size_t original_len = std::strlen(original);

    // 1) Simple one-shot compress / uncompress using compress2 / uncompress
    {
        uLongf bound = compressBound(static_cast<uLong>(original_len));
        std::vector<unsigned char> comp_buf(bound);
        uLongf comp_size = bound;

        int ret = compress2(comp_buf.data(), &comp_size,
                            reinterpret_cast<const Bytef*>(original),
                            static_cast<uLong>(original_len),
                            Z_BEST_COMPRESSION);
        if (ret != Z_OK) return -1;

        std::vector<unsigned char> decomp_buf(original_len + 10); // a little extra
        uLongf decomp_size = static_cast<uLongf>(decomp_buf.size());
        ret = uncompress(decomp_buf.data(), &decomp_size,
                         comp_buf.data(), comp_size);
        if (ret != Z_OK) return -2;

        if (decomp_size != original_len) return -3;
        if (std::memcmp(decomp_buf.data(), original, original_len) != 0) return -4;
    }

    // 2) Streaming deflate with a preset dictionary, then streaming inflate requiring that dictionary
    const char dict[] = "preset_dictionary_v1";
    const uInt dict_len = static_cast<uInt>(sizeof(dict) - 1);

    std::vector<unsigned char> streamed_compressed;
    {
        z_stream dstream;
        std::memset(&dstream, 0, sizeof(dstream));
        int ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -10;

        // Set a preset dictionary for the compressor
        ret = deflateSetDictionary(&dstream,
                                   reinterpret_cast<const Bytef*>(dict),
                                   dict_len);
        if (ret != Z_OK) {
            deflateEnd(&dstream);
            return -11;
        }

        // Tell zlib about the input
        dstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        dstream.avail_in = static_cast<uInt>(original_len);

        // Allocate an output buffer using deflateBound to be safe
        uLong bound = deflateBound(&dstream, static_cast<uLong>(original_len));
        streamed_compressed.resize(bound);

        dstream.next_out = streamed_compressed.data();
        dstream.avail_out = static_cast<uInt>(streamed_compressed.size());

        // Finish compression in one step (input fits), handle return codes
        ret = deflate(&dstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            deflateEnd(&dstream);
            return -12;
        }

        // compressed size is total_out
        streamed_compressed.resize(dstream.total_out);

        deflateEnd(&dstream);
    }

    // 3) Streaming inflate expecting the preset dictionary
    {
        z_stream istream;
        std::memset(&istream, 0, sizeof(istream));
        int ret = inflateInit(&istream);
        if (ret != Z_OK) return -20;

        istream.next_in = streamed_compressed.data();
        istream.avail_in = static_cast<uInt>(streamed_compressed.size());

        std::vector<unsigned char> recovered;
        recovered.resize(original_len + 64); // some slack

        istream.next_out = recovered.data();
        istream.avail_out = static_cast<uInt>(recovered.size());

        ret = inflate(&istream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT) {
            // supply the same dictionary used by the compressor
            ret = inflateSetDictionary(&istream,
                                       reinterpret_cast<const Bytef*>(dict),
                                       dict_len);
            if (ret != Z_OK) {
                inflateEnd(&istream);
                return -21;
            }
            // After setting dictionary, continue inflating
            ret = inflate(&istream, Z_NO_FLUSH);
        }

        if (ret != Z_STREAM_END && ret != Z_OK) {
            inflateEnd(&istream);
            return -22;
        }

        size_t recovered_size = istream.total_out;
        inflateEnd(&istream);

        if (recovered_size != original_len) return -23;
        if (std::memcmp(recovered.data(), original, original_len) != 0) return -24;
    }

    // If we reach here all zlib API sequences used succeeded
    return 66;
}