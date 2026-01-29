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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Required headers for types and functions used from the C++ standard library
    // and zlib. Including headers inside a function is valid C/C++ — it ensures the
    // function is self-contained for the purpose of this example.
    #include <zlib.h>
    #include <cstring>
    #include <cstdlib>

    // Source data to compress/decompress
    const char source[] = "The quick brown fox jumps over the lazy dog. "
                          "Repeat to make it somewhat larger: The quick brown fox "
                          "jumps over the lazy dog. The quick brown fox jumps over the lazy dog.";
    const size_t source_len = sizeof(source) - 1; // exclude terminating NUL

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(source), static_cast<uInt>(source_len));
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(source), static_cast<uInt>(source_len));

    // 2) One-shot compress() and uncompress()
    uLongf comp_bound = compressBound(static_cast<uLong>(source_len));
    Bytef* comp_buf = static_cast<Bytef*>(std::malloc(comp_bound));
    if (!comp_buf) return 1;

    uLongf comp_size = comp_bound;
    int ret = compress(comp_buf, &comp_size,
                       reinterpret_cast<const Bytef*>(source),
                       static_cast<uLong>(source_len));
    if (ret != Z_OK) {
        std::free(comp_buf);
        return 2;
    }

    // Uncompress into buffer of known original size
    Bytef* uncomp_buf = static_cast<Bytef*>(std::malloc(source_len));
    if (!uncomp_buf) {
        std::free(comp_buf);
        return 3;
    }
    uLongf uncompsize = static_cast<uLongf>(source_len);
    ret = uncompress(uncomp_buf, &uncompsize, comp_buf, comp_size);
    if (ret != Z_OK || uncompsize != source_len ||
        std::memcmp(uncomp_buf, source, source_len) != 0) {
        std::free(comp_buf);
        std::free(uncomp_buf);
        return 4;
    }

    // Free one-shot buffers (we will also do streaming below)
    std::free(uncomp_buf);

    // 3) Streaming deflate (z_stream + deflateInit/deflate/deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return 5;
    }

    // Prepare output buffer for streaming deflate
    uLongf stream_out_buf_size = compressBound(static_cast<uLong>(source_len));
    Bytef* stream_comp = static_cast<Bytef*>(std::malloc(stream_out_buf_size));
    if (!stream_comp) {
        deflateEnd(&defstream);
        std::free(comp_buf);
        return 6;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(source));
    defstream.avail_in = static_cast<uInt>(source_len);
    defstream.next_out = stream_comp;
    defstream.avail_out = static_cast<uInt>(stream_out_buf_size);

    // Perform deflate in one step (buffer large enough due to compressBound)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        std::free(comp_buf);
        std::free(stream_comp);
        return 7;
    }

    // Compressed size from streaming deflate
    uLongf stream_comp_size = defstream.total_out;

    // Clean up deflate stream
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        std::free(stream_comp);
        return 8;
    }

    // 4) Streaming inflate (z_stream + inflateInit/inflate/inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        std::free(stream_comp);
        return 9;
    }

    Bytef* stream_uncomp = static_cast<Bytef*>(std::malloc(source_len));
    if (!stream_uncomp) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        std::free(stream_comp);
        return 10;
    }

    infstream.next_in = stream_comp;
    infstream.avail_in = static_cast<uInt>(stream_comp_size);
    infstream.next_out = stream_uncomp;
    infstream.avail_out = static_cast<uInt>(source_len);

    // Perform inflate in one step (we know output size)
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        std::free(stream_comp);
        std::free(stream_uncomp);
        return 11;
    }

    // Verify uncompressed streaming output matches original
    if (infstream.total_out != source_len ||
        std::memcmp(stream_uncomp, source, source_len) != 0) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        std::free(stream_comp);
        std::free(stream_uncomp);
        return 12;
    }

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        std::free(stream_comp);
        std::free(stream_uncomp);
        return 13;
    }

    // Clean up all allocated buffers
    std::free(comp_buf);
    std::free(stream_comp);
    std::free(stream_uncomp);

    // If we reached here, all zlib API calls succeeded and round-trip checks passed.
    // Return 66 as requested on success.
    (void)crc;   // silence unused-variable warnings in some build configs
    (void)adler; // silence unused-variable warnings in some build configs
    return 66;
}