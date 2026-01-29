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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here so the function is self-contained when copied into a file
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    // sample input data
    const char* input = "The quick brown fox jumps over the lazy dog";
    size_t input_len = std::strlen(input);
    if (input_len == 0) return -1;

    // 1) compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    // 2) deflate (compress) using z_stream APIs
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);

    // allocate output buffer at least compressBound(input_len)
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> compbuf(static_cast<size_t>(bound));
    defstream.next_out = compbuf.data();
    defstream.avail_out = static_cast<uInt>(compbuf.size());

    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        // initialization failed
        return -2;
    }

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -3;
    }

    uLong compressed_size = defstream.total_out;
    // clean up deflate state
    deflateEnd(&defstream);

    // 3) inflate (decompress) the result
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.next_in = compbuf.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);

    // allocate buffer to hold decompressed data (we know original size)
    std::vector<Bytef> decompbuf(input_len + 1); // +1 for safety if treated as C-string
    infstream.next_out = decompbuf.data();
    infstream.avail_out = static_cast<uInt>(decompbuf.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }

    uLong decompressed_size = infstream.total_out;
    // clean up inflate state
    inflateEnd(&infstream);

    // 4) verify sizes and content match original
    if (decompressed_size != input_len) return -6;
    if (std::memcmp(decompbuf.data(), input, input_len) != 0) return -7;

    // Optionally verify checksums round-trip (not strictly necessary here)
    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, decompbuf.data(), static_cast<uInt>(decompressed_size));
    if (crc_check != crc) return -8;

    uLong adl_check = adler32(0L, Z_NULL, 0);
    adl_check = adler32(adl_check, decompbuf.data(), static_cast<uInt>(decompressed_size));
    if (adl_check != adl) return -9;

    // If all operations succeeded, return 66 per requirement
    return 66;
}