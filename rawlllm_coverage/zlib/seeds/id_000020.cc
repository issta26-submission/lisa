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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include needed headers here so the function is self-contained when inserted into a file.
    // Including headers inside a function is legal (preprocessor runs before compilation),
    // and ensures this snippet compiles even if the outer translation unit doesn't include them.
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    // Sample input data
    const char* input = "The quick brown fox jumps over the lazy dog";
    const size_t input_len = std::strlen(input);

    // --- 1) Simple convenience API: compress / uncompress ---
    // Get an upper bound for compressed size
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp1(bound);
    uLong comp1_len = bound;

    int ret = compress(comp1.data(), &comp1_len,
                       reinterpret_cast<const Bytef*>(input),
                       static_cast<uLong>(input_len));
    if (ret != Z_OK) {
        return -1; // compress failed
    }
    comp1.resize(comp1_len);

    // Uncompress back into a buffer sized exactly as original
    std::vector<Bytef> decomp1(input_len);
    uLong decomp1_len = static_cast<uLong>(input_len);
    ret = uncompress(decomp1.data(), &decomp1_len,
                     comp1.data(), comp1_len);
    if (ret != Z_OK || decomp1_len != input_len) {
        return -2; // uncompress failed or length mismatch
    }
    if (std::memcmp(decomp1.data(), input, input_len) != 0) {
        return -3; // data mismatch
    }

    // --- 2) Streaming API: deflate / inflate using z_stream ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate with default compression level
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -4;
    }

    // Provide the input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len); // safe for small input

    std::vector<unsigned char> comp2;
    const size_t OUT_CHUNK = 64;
    unsigned char outbuf[OUT_CHUNK];

    // Deflate until all input consumed
    while (defstream.avail_in > 0) {
        defstream.next_out = outbuf;
        defstream.avail_out = static_cast<uInt>(OUT_CHUNK);
        int dret = deflate(&defstream, Z_NO_FLUSH);
        if (dret != Z_OK) {
            deflateEnd(&defstream);
            return -5; // deflate failure while processing input
        }
        size_t have = OUT_CHUNK - defstream.avail_out;
        comp2.insert(comp2.end(), outbuf, outbuf + have);
    }

    // Finish the stream
    for (;;) {
        defstream.next_out = outbuf;
        defstream.avail_out = static_cast<uInt>(OUT_CHUNK);
        int dret = deflate(&defstream, Z_FINISH);
        if (dret != Z_OK && dret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -6; // deflate finish failure
        }
        size_t have = OUT_CHUNK - defstream.avail_out;
        comp2.insert(comp2.end(), outbuf, outbuf + have);
        if (dret == Z_STREAM_END) break;
    }

    // Clean up deflate
    if (deflateEnd(&defstream) != Z_OK) {
        return -7;
    }

    // Now inflate the compressed stream back
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    if (inflateInit(&infstream) != Z_OK) {
        return -8;
    }

    infstream.next_in = comp2.empty() ? Z_NULL : comp2.data();
    infstream.avail_in = static_cast<uInt>(comp2.size());

    std::vector<unsigned char> decomp2;
    for (;;) {
        infstream.next_out = outbuf;
        infstream.avail_out = static_cast<uInt>(OUT_CHUNK);
        int iret = inflate(&infstream, Z_NO_FLUSH);
        if (iret != Z_OK && iret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -9; // inflate failure
        }
        size_t have = OUT_CHUNK - infstream.avail_out;
        decomp2.insert(decomp2.end(), outbuf, outbuf + have);
        if (iret == Z_STREAM_END) break;
        // If avail_in is zero and no output produced, continue or break to avoid infinite loop
        if (infstream.avail_in == 0 && have == 0) {
            // nothing more to do but no stream end: treat as error
            inflateEnd(&infstream);
            return -10;
        }
    }

    if (inflateEnd(&infstream) != Z_OK) {
        return -11;
    }

    // Validate streaming round-trip
    if (decomp2.size() != input_len) {
        return -12;
    }
    if (std::memcmp(decomp2.data(), input, input_len) != 0) {
        return -13;
    }

    // If we've reached this point, both the convenience APIs and streaming APIs worked.
    return 66;
}