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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here to make the function self-contained when pasted into a single file.
    // Including inside a function is unusual but valid for this purpose.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>
    #include <cstdio>
    #include <cstdlib>

    const char* filename = "test_zlib_api_sequence_tmp.gz";

    // Sample input data
    const char* input = "The quick brown fox jumps over the lazy dog. "
                        "This is a test string for exercising multiple zlib APIs.";
    size_t input_len = std::strlen(input);

    // 1) Compute adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    // Basic sanity: neither should be zero for non-empty input
    if (input_len > 0 && (adl == 0 || crc == 0)) {
        return -1;
    }

    // 2) Use compress() / uncompress()
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> compbuf(bound);
    uLong comp_size = bound;
    int zret = compress(compbuf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(input),
                        static_cast<uLong>(input_len));
    if (zret != Z_OK) {
        return -2;
    }

    // Uncompress
    std::vector<Bytef> uncombuf(input_len);
    uLong uncom_len = static_cast<uLong>(input_len);
    zret = uncompress(uncombuf.data(), &uncom_len, compbuf.data(), comp_size);
    if (zret != Z_OK || uncom_len != input_len ||
        std::memcmp(uncombuf.data(), input, input_len) != 0) {
        return -3;
    }

    // 3) Use deflateInit / deflate / deflateEnd and inflateInit / inflate / inflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    zret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (zret != Z_OK) {
        return -4;
    }

    // Prepare input for deflate
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);

    // Output for deflate
    uLong def_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> deflated(def_bound);
    defstream.next_out = deflated.data();
    defstream.avail_out = static_cast<uInt>(def_bound);

    // Perform deflate with single Z_FINISH (buffer big enough due to compressBound)
    zret = deflate(&defstream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -5;
    }
    uLong deflated_size = def_bound - defstream.avail_out;
    deflateEnd(&defstream);

    // Now inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    zret = inflateInit(&infstream);
    if (zret != Z_OK) {
        return -6;
    }

    infstream.next_in = deflated.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);

    std::vector<Bytef> inflated(input_len);
    infstream.next_out = inflated.data();
    infstream.avail_out = static_cast<uInt>(input_len);

    zret = inflate(&infstream, Z_FINISH);
    if (zret != Z_STREAM_END || (input_len - infstream.avail_out) != input_len) {
        inflateEnd(&infstream);
        return -7;
    }
    inflateEnd(&infstream);

    if (std::memcmp(inflated.data(), input, input_len) != 0) {
        return -8;
    }

    // 4) Use gzopen / gzwrite / gzclose and gzopen / gzread / gzclose
    gzFile gz = gzopen(filename, "wb");
    if (!gz) {
        return -9;
    }
    int wrote = gzwrite(gz, input, static_cast<unsigned int>(input_len));
    if (wrote != static_cast<int>(input_len)) {
        gzclose(gz);
        std::remove(filename);
        return -10;
    }
    if (gzclose(gz) != Z_OK) {
        std::remove(filename);
        return -11;
    }

    // Read back
    gz = gzopen(filename, "rb");
    if (!gz) {
        std::remove(filename);
        return -12;
    }
    std::vector<char> gzreadbuf(input_len);
    int read = gzread(gz, gzreadbuf.data(), static_cast<int>(input_len));
    if (read < 0 || static_cast<size_t>(read) != input_len) {
        gzclose(gz);
        std::remove(filename);
        return -13;
    }
    if (gzclose(gz) != Z_OK) {
        std::remove(filename);
        return -14;
    }

    if (std::memcmp(gzreadbuf.data(), input, input_len) != 0) {
        std::remove(filename);
        return -15;
    }

    // Clean up the temporary gz file
    std::remove(filename);

    // If we reach here, all API usages succeeded; return 66 as requested.
    return 66;
}