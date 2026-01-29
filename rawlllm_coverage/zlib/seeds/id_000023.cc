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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers via preprocessor (allowed anywhere). If your build system already includes
    // these headers globally, you can remove these lines.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample data to compress and then decompress
    const std::string original = 
        "The quick brown fox jumps over the lazy dog. "
        "This string is used to test zlib deflate/inflate API usage. "
        "Repeat it a few times to make the buffer slightly larger. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // Prepare deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // initialization failed
        return -1;
    }

    // Allocate output buffer for compressed data using compressBound to be safe
    uLong sourceLen = static_cast<uLong>(original.size());
    uLong bound = compressBound(sourceLen);
    std::vector<unsigned char> compressed(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(sourceLen);
    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    // Perform compression until finished
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return -2;
        }
        // If we get here and still not finished, but avail_out exhausted,
        // this shouldn't happen because we used compressBound. Still handle defensively.
        if (defstream.avail_out == 0) {
            // enlarge buffer and continue
            std::size_t oldSize = compressed.size();
            compressed.resize(oldSize * 2);
            defstream.next_out = compressed.data() + oldSize;
            defstream.avail_out = static_cast<uInt>(compressed.size() - oldSize);
        }
    }

    // Final compressed size
    std::size_t compressedSize = compressed.size() - defstream.avail_out;

    // Clean up deflate state
    deflateEnd(&defstream);

    // Prepare inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> decompressed(original.size()); // expect to get original size back
    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressedSize);
    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    // Perform decompression
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK) {
            inflateEnd(&infstream);
            return -4;
        }
        // If output buffer too small, grow it and continue
        if (infstream.avail_out == 0) {
            std::size_t oldSize = decompressed.size();
            decompressed.resize(oldSize * 2);
            infstream.next_out = decompressed.data() + oldSize;
            infstream.avail_out = static_cast<uInt>(decompressed.size() - oldSize);
        }
    }

    // Final decompressed size
    std::size_t decompressedSize = decompressed.size() - infstream.avail_out;

    // Clean up inflate state
    inflateEnd(&infstream);

    // Verify decompressed content matches original
    if (decompressedSize != original.size()) {
        return -5;
    }
    if (std::memcmp(decompressed.data(), original.data(), original.size()) != 0) {
        return -6;
    }

    // Success: return 66 as requested
    return 66;
}