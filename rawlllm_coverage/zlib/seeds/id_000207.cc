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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of the zlib API sequence. "
        "We repeat this sentence to have a reasonable data size. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";
    const uLong input_len = static_cast<uLong>(input.size());

    // 1) Use deflate (z_stream) to compress in-memory
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = input_len;

    // allocate output buffer using compressBound
    uLong bound = compressBound(input_len);
    std::vector<unsigned char> compressed(bound);
    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }

    uLong compressed_size = static_cast<uLong>(compressed.size()) - defstream.avail_out;

    if (deflateEnd(&defstream) != Z_OK) {
        return -3;
    }

    // 2) Use inflate (z_stream) to decompress in-memory
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    if (inflateInit(&infstream) != Z_OK) {
        return -4;
    }

    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);

    std::vector<unsigned char> uncompressed(input_len);
    infstream.next_out = uncompressed.data();
    infstream.avail_out = static_cast<uInt>(uncompressed.size());

    int iret = inflate(&infstream, Z_FINISH);
    if (iret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }

    uLong uncompressed_size = static_cast<uLong>(uncompressed.size()) - infstream.avail_out;

    if (inflateEnd(&infstream) != Z_OK) {
        return -6;
    }

    // Verify decompressed data matches original
    if (uncompressed_size != input_len) {
        return -7;
    }
    if (std::memcmp(uncompressed.data(), input.data(), input_len) != 0) {
        return -8;
    }

    // 3) Use convenience APIs compress2 and uncompress
    uLong bound2 = compressBound(input_len);
    std::vector<unsigned char> comp2(bound2);
    uLong comp2_size = bound2;

    int cret = compress2(comp2.data(), &comp2_size,
                         reinterpret_cast<const Bytef*>(input.data()), input_len,
                         Z_BEST_COMPRESSION);
    if (cret != Z_OK) {
        return -9;
    }

    std::vector<unsigned char> uncomp2(input_len);
    uLong uncomp2_size = input_len;
    int uret = uncompress(uncomp2.data(), &uncomp2_size, comp2.data(), comp2_size);
    if (uret != Z_OK) {
        return -10;
    }
    if (uncomp2_size != input_len) {
        return -11;
    }
    if (std::memcmp(uncomp2.data(), input.data(), input_len) != 0) {
        return -12;
    }

    // 4) Compute checksums using crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), input_len);

    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input.data()), input_len);

    // Basic sanity checks on checksums (they should be non-zero for this data)
    if (crc == 0 || adler == 0) {
        return -13;
    }

    // If we've reached this point, the API sequence succeeded
    return 66;
}