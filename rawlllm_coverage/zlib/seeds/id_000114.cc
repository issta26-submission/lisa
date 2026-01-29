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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // The following #includes are placed here so this function can be pasted
    // into a single-file test. They may normally appear at the top of a source file.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Prepare some sample input data (repetitive to allow compression to be effective)
    const std::string original = std::string("The quick brown fox jumps over the lazy dog. ") + 
                                 std::string("The quick brown fox jumps over the lazy dog. ") +
                                 std::string("The quick brown fox jumps over the lazy dog. ") +
                                 std::string("The quick brown fox jumps over the lazy dog. ");

    // 1) One-shot compress / uncompress using compress() and uncompress()
    {
        uLong sourceLen = static_cast<uLong>(original.size());
        uLong bound = compressBound(sourceLen);
        std::vector<unsigned char> compressed(bound);

        uLongf compressedLen = static_cast<uLongf>(bound);
        int ret = compress(compressed.data(), &compressedLen,
                           reinterpret_cast<const Bytef*>(original.data()), sourceLen);
        if (ret != Z_OK) return -1; // compress failed

        // Decompress with uncompress()
        std::vector<unsigned char> decompressed(sourceLen);
        uLongf decompressedLen = sourceLen;
        ret = uncompress(decompressed.data(), &decompressedLen,
                         compressed.data(), compressedLen);
        if (ret != Z_OK) return -2; // uncompress failed

        if (decompressedLen != sourceLen) return -3;
        if (memcmp(decompressed.data(), original.data(), sourceLen) != 0) return -4;
    }

    // 2) Streaming compress using deflateInit / deflate / deflateEnd
    std::vector<unsigned char> stream_compressed;
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        defstream.zalloc = Z_NULL;
        defstream.zfree = Z_NULL;
        defstream.opaque = Z_NULL;

        int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
        if (ret != Z_OK) return -5;

        const Bytef* input_ptr = reinterpret_cast<const Bytef*>(original.data());
        uInt input_left = static_cast<uInt>(original.size());
        defstream.next_in = const_cast<Bytef*>(input_ptr);
        defstream.avail_in = input_left;

        const size_t CHUNK = 128;
        std::vector<unsigned char> outbuf(CHUNK);

        // We want to finish the stream in this call sequence
        do {
            defstream.next_out = outbuf.data();
            defstream.avail_out = static_cast<uInt>(outbuf.size());
            ret = deflate(&defstream, Z_FINISH); // finish the compression
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                deflateEnd(&defstream);
                return -6;
            }
            size_t have = outbuf.size() - defstream.avail_out;
            stream_compressed.insert(stream_compressed.end(), outbuf.data(), outbuf.data() + have);
        } while (defstream.avail_out == 0);

        if (ret != Z_STREAM_END) { deflateEnd(&defstream); return -7; }

        deflateEnd(&defstream);
    }

    // 3) Streaming inflate using inflateInit / inflate / inflateEnd on the streaming output
    {
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        infstream.zalloc = Z_NULL;
        infstream.zfree = Z_NULL;
        infstream.opaque = Z_NULL;

        int ret = inflateInit(&infstream);
        if (ret != Z_OK) return -8;

        infstream.next_in = stream_compressed.empty() ? Z_NULL : stream_compressed.data();
        infstream.avail_in = static_cast<uInt>(stream_compressed.size());

        const size_t CHUNK = 128;
        std::vector<unsigned char> outbuf(CHUNK);
        std::vector<unsigned char> recovered;

        do {
            infstream.next_out = outbuf.data();
            infstream.avail_out = static_cast<uInt>(outbuf.size());
            ret = inflate(&infstream, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&infstream);
                return -9;
            }
            size_t have = outbuf.size() - infstream.avail_out;
            recovered.insert(recovered.end(), outbuf.data(), outbuf.data() + have);
        } while (ret != Z_STREAM_END);

        inflateEnd(&infstream);

        // Verify recovered data matches original
        if (recovered.size() != original.size()) return -10;
        if (memcmp(recovered.data(), original.data(), original.size()) != 0) return -11;
    }

    // If all steps succeeded, return 66 as requested
    return 66;
}