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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Including inside a function is valid though unusual; it ensures required declarations
    // are available for this standalone snippet.)
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Example input to compress and decompress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This string will be compressed using zlib and then "
                              "decompressed to verify the API sequence.";

    // ---------- Compression (deflate) ----------
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return -1;
    }

    uLong src_len = static_cast<uLong>(input.size());
    // Reserve an output buffer large enough for compressed data
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = src_len;
    defstream.next_out = comp_buf.data();
    defstream.avail_out = bound;

    // Call deflate until stream ends
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            deflateEnd(&defstream);
            return -2; // unexpected error during deflate
        }
        // If Z_BUF_ERROR or Z_OK and not finished, loop (though compressBound should avoid needing more)
    }

    size_t comp_size = bound - defstream.avail_out;
    deflateEnd(&defstream);

    // ---------- Decompression (inflate) ----------
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    // Allocate output buffer sized to original input length (we expect exact match)
    std::vector<unsigned char> decomp_buf(src_len);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    // Inflate until stream end
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            inflateEnd(&infstream);
            return -4; // unexpected error during inflate
        }
        // In normal conditions with correct buffer sizes this loop will end quickly
    }

    size_t decomp_size = decomp_buf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    // ---------- Verify ----------
    if (decomp_size != input.size()) {
        return -5; // size mismatch
    }
    if (std::memcmp(decomp_buf.data(), input.data(), input.size()) != 0) {
        return -6; // content mismatch
    }

    // If we reached here, the zlib API sequence was used correctly and round-trip succeeded
    return 66;
}