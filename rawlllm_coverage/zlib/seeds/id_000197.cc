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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained for the example.
    // (Preprocessor directives are allowed anywhere in the translation unit.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare some input data
    std::string input;
    for (int i = 0; i < 100; ++i) input += "The quick brown fox jumps over the lazy dog. ";
    const uLong srcLen = static_cast<uLong>(input.size());

    // 1) One-shot compress/uncompress using compress() / uncompress()
    uLong bound = compressBound(srcLen);
    std::vector<Bytef> comp(bound);
    uLong compLen = bound;
    int ret = compress(comp.data(), &compLen, reinterpret_cast<const Bytef*>(input.data()), srcLen);
    if (ret != Z_OK) return -1;

    std::vector<Bytef> decomp(srcLen);
    uLong decompLen = srcLen;
    ret = uncompress(decomp.data(), &decompLen, comp.data(), compLen);
    if (ret != Z_OK) return -2;
    if (decompLen != srcLen) return -3;
    if (std::memcmp(decomp.data(), input.data(), srcLen) != 0) return -4;

    // 2) Streaming compress using deflateInit / deflate / deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    // Provide input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = srcLen;

    // Provide output buffer large enough
    std::vector<Bytef> comp2(compressBound(srcLen));
    defstream.next_out = comp2.data();
    defstream.avail_out = static_cast<uInt>(comp2.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&defstream); return -6; }
    // total_out holds number of output bytes produced
    uLong comp2Len = defstream.total_out;
    deflateEnd(&defstream);

    // 3) Streaming inflate using inflateInit / inflate / inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    // Input is comp2.data() with length comp2Len
    infstream.next_in = comp2.data();
    infstream.avail_in = static_cast<uInt>(comp2Len);

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -7;

    std::vector<Bytef> decomp2(srcLen);
    infstream.next_out = decomp2.data();
    infstream.avail_out = static_cast<uInt>(decomp2.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&infstream); return -8; }
    uLong decomp2Len = infstream.total_out;
    inflateEnd(&infstream);

    if (decomp2Len != srcLen) return -9;
    if (std::memcmp(decomp2.data(), input.data(), srcLen) != 0) return -10;

    // All checks passed
    return 66;
}