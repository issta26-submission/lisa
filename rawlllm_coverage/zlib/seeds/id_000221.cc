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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here to make the function self-contained when pasted alone
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Prepare input data (make it sufficiently large to exercise compression)
    std::string input;
    for (int i = 0; i < 200; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }

    // Basic bounds and types
    uLong sourceLen = static_cast<uLong>(input.size());

    // 1) Use compress/uncompress (simple single-call API)
    uLongf compBoundVal = compressBound(sourceLen);
    std::vector<unsigned char> compBuf(compBoundVal);
    uLongf compSize = compBoundVal;

    int cres = compress(compBuf.data(), &compSize,
                        reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    if (cres != Z_OK) {
        return 1;
    }

    std::vector<unsigned char> uncompBuf(sourceLen);
    uLongf uncompSize = sourceLen;
    int ucres = uncompress(uncompBuf.data(), &uncompSize, compBuf.data(), compSize);
    if (ucres != Z_OK || uncompSize != sourceLen) {
        return 2;
    }
    if (std::memcmp(uncompBuf.data(), input.data(), sourceLen) != 0) {
        return 3;
    }

    // 2) Compute and compare checksums (crc32 and adler32)
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    uLong crc_uncomp = crc32(0L, Z_NULL, 0);
    crc_uncomp = crc32(crc_uncomp, uncompBuf.data(), uncompSize);
    if (crc_orig != crc_uncomp) {
        return 4;
    }

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    uLong adler_uncomp = adler32(0L, Z_NULL, 0);
    adler_uncomp = adler32(adler_uncomp, uncompBuf.data(), uncompSize);
    if (adler_orig != adler_uncomp) {
        return 5;
    }

    // 3) Streaming compression with deflate / inflate
    // deflate (streaming)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int dinit = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (dinit != Z_OK) {
        return 6;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(sourceLen); // safe for our size

    // allocate output buffer using compressBound to ensure it's large enough
    std::vector<unsigned char> scompBuf(static_cast<size_t>(compressBound(sourceLen)));
    defstream.next_out = scompBuf.data();
    defstream.avail_out = static_cast<uInt>(scompBuf.size());

    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 7;
    }
    uLongf scompSize = static_cast<uLongf>(scompBuf.size() - defstream.avail_out);
    deflateEnd(&defstream);

    // inflate (streaming)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    int iinit = inflateInit(&infstream);
    if (iinit != Z_OK) {
        return 8;
    }

    infstream.next_in = scompBuf.data();
    infstream.avail_in = static_cast<uInt>(scompSize);
    std::vector<unsigned char> sdecompBuf(sourceLen);
    infstream.next_out = sdecompBuf.data();
    infstream.avail_out = static_cast<uInt>(sdecompBuf.size());

    int iret = inflate(&infstream, Z_FINISH);
    if (iret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 9;
    }
    uLongf sdecompSize = static_cast<uLongf>(sdecompBuf.size() - infstream.avail_out);
    inflateEnd(&infstream);

    if (sdecompSize != sourceLen) {
        return 10;
    }
    if (std::memcmp(sdecompBuf.data(), input.data(), sourceLen) != 0) {
        return 11;
    }

    // All tests passed; return the required success value
    return 66;
}