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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises several zlib APIs:
    // - compress / uncompress (one-shot)
    // - deflateInit / deflate / deflateEnd (streaming compress)
    // - inflateInit / inflate / inflateEnd (streaming decompress)
    // - gzopen / gzwrite / gzread / gzclose (gzip file API)
    //
    // On success returns 66. On failure returns a non-66 error code.

    const char *input = "The quick brown fox jumps over the lazy dog. "
                        "This is a test string to validate zlib compression and decompression.";
    size_t input_len = strlen(input);

    // 1) One-shot compress/uncompress
    uLong bound = compressBound((uLong)input_len);
    Bytef *comp1 = new Bytef[(size_t)bound];
    uLong comp1_len = bound;
    int ret = compress(comp1, &comp1_len, (const Bytef *)input, (uLong)input_len);
    if (ret != Z_OK) {
        delete[] comp1;
        return 1;
    }

    Bytef *decomp1 = new Bytef[input_len + 1];
    uLong decomp1_len = (uLong)(input_len + 1);
    ret = uncompress(decomp1, &decomp1_len, comp1, comp1_len);
    if (ret != Z_OK) {
        delete[] comp1;
        delete[] decomp1;
        return 2;
    }
    if (decomp1_len != (uLong)input_len || memcmp(decomp1, input, input_len) != 0) {
        delete[] comp1;
        delete[] decomp1;
        return 3;
    }
    delete[] comp1;
    delete[] decomp1;

    // 2) Streaming deflate (compress)
    z_stream dstream;
    memset(&dstream, 0, sizeof(dstream));
    ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 4;

    // allocate output buffer with compressBound estimate
    Bytef *comp2 = new Bytef[(size_t)bound];
    dstream.next_in = (Bytef *)input;
    dstream.avail_in = (uInt)input_len;
    dstream.next_out = comp2;
    dstream.avail_out = (uInt)bound;

    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstream);
        delete[] comp2;
        return 5;
    }
    uLong comp2_len = (uLong)((size_t)bound - dstream.avail_out);
    deflateEnd(&dstream);

    // 3) Streaming inflate (decompress)
    z_stream istream;
    memset(&istream, 0, sizeof(istream));
    ret = inflateInit(&istream);
    if (ret != Z_OK) {
        delete[] comp2;
        return 6;
    }
    Bytef *decomp2 = new Bytef[input_len + 1];
    istream.next_in = comp2;
    istream.avail_in = (uInt)comp2_len;
    istream.next_out = decomp2;
    istream.avail_out = (uInt)(input_len + 1);

    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istream);
        delete[] comp2;
        delete[] decomp2;
        return 7;
    }
    uLong decomp2_len = (uLong)((input_len + 1) - istream.avail_out);
    inflateEnd(&istream);

    if (decomp2_len != (uLong)input_len || memcmp(decomp2, input, input_len) != 0) {
        delete[] comp2;
        delete[] decomp2;
        return 8;
    }
    delete[] comp2;
    delete[] decomp2;

    // 4) gz* API (write and read a small gzip file)
    const char *gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gf = gzopen(gzname, "wb");
    if (!gf) return 9;
    int wrote = gzwrite(gf, input, (unsigned)input_len);
    if (wrote != (int)input_len) {
        gzclose(gf);
        return 10;
    }
    if (gzclose(gf) != Z_OK) return 11;

    gf = gzopen(gzname, "rb");
    if (!gf) return 12;
    char *gzreadbuf = new char[input_len + 1];
    int read = gzread(gf, gzreadbuf, (unsigned)input_len);
    if (read < 0) {
        gzclose(gf);
        delete[] gzreadbuf;
        return 13;
    }
    if (gzclose(gf) != Z_OK) {
        delete[] gzreadbuf;
        return 14;
    }
    if (read != (int)input_len || memcmp(gzreadbuf, input, input_len) != 0) {
        delete[] gzreadbuf;
        return 15;
    }
    delete[] gzreadbuf;

    // Optional: remove the temporary gzip file; ignore remove errors
    (void)remove(gzname);

    // All tests passed
    return 66;
}