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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here to keep the function self-contained when pasted into a file.
    // It's legal to place preprocessor directives anywhere; the includes will be expanded here.
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare sample data to compress
    const std::string original = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string that will be compressed by zlib. "
        "Repeating content to make compression worthwhile. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";
    const Bytef* input_ptr = reinterpret_cast<const Bytef*>(original.data());
    uLong input_len = static_cast<uLong>(original.size());

    // 1) Stream-based compression with deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    // Get an upper bound for compressed size and allocate buffer
    uLong bound = deflateBound(&defstream, input_len);
    std::vector<unsigned char> compBuf(bound);

    defstream.next_in = const_cast<Bytef*>(input_ptr);
    defstream.avail_in = input_len;
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size()); // avail_out is uInt

    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END) {
        // Clean up and return error
        deflateEnd(&defstream);
        return -2;
    }
    // total_out holds compressed size
    uLong compSize = defstream.total_out;
    deflateEnd(&defstream);

    // 2) Stream-based decompression with inflate
    z_stream instream;
    std::memset(&instream, 0, sizeof(instream));
    instream.zalloc = Z_NULL;
    instream.zfree = Z_NULL;
    instream.opaque = Z_NULL;

    if (inflateInit(&instream) != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> outBuf(input_len);
    instream.next_in = compBuf.data();
    instream.avail_in = static_cast<uInt>(compSize);
    instream.next_out = outBuf.data();
    instream.avail_out = static_cast<uInt>(outBuf.size());

    int iret = Z_OK;
    while (iret == Z_OK) {
        iret = inflate(&instream, Z_NO_FLUSH);
        if (iret == Z_NEED_DICT || iret == Z_DATA_ERROR || iret == Z_MEM_ERROR) {
            inflateEnd(&instream);
            return -4;
        }
        // If output buffer was not large enough, in general we would expand it here.
        // But we used the original size, so we expect to finish with Z_STREAM_END.
    }
    if (iret != Z_STREAM_END) {
        inflateEnd(&instream);
        return -5;
    }
    uLong decompressedSize = instream.total_out;
    inflateEnd(&instream);

    if (decompressedSize != input_len) {
        return -6;
    }
    if (std::memcmp(outBuf.data(), input_ptr, input_len) != 0) {
        return -7;
    }

    // 3) Quick compression APIs: compress / uncompress
    uLongf comp2Bound = compressBound(input_len);
    std::vector<unsigned char> comp2Buf(comp2Bound);
    uLongf comp2Size = comp2Buf.size();

    int rc = compress(comp2Buf.data(), &comp2Size, input_ptr, input_len);
    if (rc != Z_OK) {
        return -8;
    }

    std::vector<unsigned char> decomp2Buf(input_len);
    uLongf decomp2Size = decomp2Buf.size();
    rc = uncompress(decomp2Buf.data(), &decomp2Size, comp2Buf.data(), comp2Size);
    if (rc != Z_OK || decomp2Size != input_len) {
        return -9;
    }
    if (std::memcmp(decomp2Buf.data(), input_ptr, input_len) != 0) {
        return -10;
    }

    // 4) CRC checks to ensure identical data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, input_ptr, input_len);
    uLong crc_inflated = crc32(0L, Z_NULL, 0);
    crc_inflated = crc32(crc_inflated, outBuf.data(), decompressedSize);
    if (crc_orig != crc_inflated) {
        return -11;
    }

    // All tests passed
    return 66;
}