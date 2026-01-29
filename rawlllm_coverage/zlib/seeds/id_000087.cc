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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // required headers (legal to include inside a function; placed here to keep this snippet self-contained)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>

    // Prepare sample input data
    std::string input;
    for (int i = 0; i < 100; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong sourceLen = static_cast<uLong>(input.size());

    // --- High-level API: compress2 / uncompress sequence ---
    // Allocate compressed buffer using compressBound
    uLong bound = compressBound(sourceLen);
    std::vector<Bytef> compBuf(bound);
    uLongf compLen = static_cast<uLongf>(bound);

    // Compress with compress2
    int ret = compress2(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(input.data()),
                        sourceLen, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // Decompress with uncompress
    std::vector<Bytef> decompBuf(static_cast<size_t>(sourceLen));
    uLongf decompLen = static_cast<uLongf>(decompBuf.size());
    ret = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (ret != Z_OK) {
        return -2;
    }
    if (decompLen != sourceLen) {
        return -3;
    }
    if (std::memcmp(decompBuf.data(), input.data(), sourceLen) != 0) {
        return -4;
    }

    // --- Stream API: deflate / inflate sequence ---
    // Compress using z_stream + deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -10;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = sourceLen;

    std::vector<Bytef> streamOutBuf(static_cast<size_t>(compressBound(sourceLen)));
    defstream.next_out = streamOutBuf.data();
    defstream.avail_out = static_cast<uInt>(streamOutBuf.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&defstream); return -11; }
    size_t streamOutSize = streamOutBuf.size() - defstream.avail_out;
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return -12;

    // Decompress using z_stream + inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -13;

    infstream.next_in = streamOutBuf.data();
    infstream.avail_in = static_cast<uInt>(streamOutSize);

    std::vector<Bytef> streamDecompBuf(static_cast<size_t>(sourceLen));
    infstream.next_out = streamDecompBuf.data();
    infstream.avail_out = static_cast<uInt>(streamDecompBuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&infstream); return -14; }
    size_t streamDecompSize = streamDecompBuf.size() - infstream.avail_out;
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) return -15;

    if (streamDecompSize != sourceLen) return -16;
    if (std::memcmp(streamDecompBuf.data(), input.data(), sourceLen) != 0) return -17;

    // If all sequences succeeded, return the required success code
    return 66;
}