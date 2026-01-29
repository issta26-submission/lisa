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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare input data (repeated to make compression meaningful)
    const std::string phrase = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 50; ++i) input += phrase; // ~2250 bytes

    uLong srcLen = static_cast<uLong>(input.size());

    // 1) Use compress2 / uncompress (simple one-shot API)
    uLong bound = compressBound(srcLen);
    std::vector<Bytef> compBuf(bound);
    uLongf compLen = bound;

    int res = compress2(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(input.data()), srcLen,
                        Z_BEST_COMPRESSION);
    if (res != Z_OK) return -1;

    std::vector<Bytef> decompBuf(srcLen);
    uLongf decompLen = srcLen;
    res = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (res != Z_OK) return -2;

    if (decompLen != srcLen || std::memcmp(decompBuf.data(), input.data(), srcLen) != 0) {
        return -3;
    }

    // 2) Use streaming API: deflate / inflate with z_stream
    std::vector<Bytef> defStreamBuf(bound);
    z_stream defStrm;
    std::memset(&defStrm, 0, sizeof(defStrm));
    res = deflateInit(&defStrm, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return -4;

    defStrm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defStrm.avail_in = srcLen;
    defStrm.next_out = defStreamBuf.data();
    defStrm.avail_out = static_cast<uInt>(defStreamBuf.size());

    // Finish in one call (input fits in memory); expect Z_STREAM_END
    res = deflate(&defStrm, Z_FINISH);
    if (res != Z_STREAM_END) {
        deflateEnd(&defStrm);
        return -5;
    }
    uLong deflatedSize = static_cast<uLong>(defStreamBuf.size() - defStrm.avail_out);

    res = deflateEnd(&defStrm);
    if (res != Z_OK) return -6;

    // Inflate the deflated stream
    z_stream infStrm;
    std::memset(&infStrm, 0, sizeof(infStrm));
    res = inflateInit(&infStrm);
    if (res != Z_OK) return -7;

    std::vector<Bytef> outStreamBuf(srcLen);
    infStrm.next_in = defStreamBuf.data();
    infStrm.avail_in = static_cast<uInt>(deflatedSize);
    infStrm.next_out = outStreamBuf.data();
    infStrm.avail_out = static_cast<uInt>(outStreamBuf.size());

    res = inflate(&infStrm, Z_FINISH);
    if (res != Z_STREAM_END) {
        inflateEnd(&infStrm);
        return -8;
    }
    uLong inflatedSize = static_cast<uLong>(outStreamBuf.size() - infStrm.avail_out);

    res = inflateEnd(&infStrm);
    if (res != Z_OK) return -9;

    if (inflatedSize != srcLen || std::memcmp(outStreamBuf.data(), input.data(), srcLen) != 0) {
        return -10;
    }

    // If we reach here, both API sequences worked and data matches
    return 66;
}