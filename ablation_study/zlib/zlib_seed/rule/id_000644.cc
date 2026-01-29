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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate and decompress
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflateReset2(&inf, 15);
    inflateUndermine(&inf, 1);
    inflate(&inf, 0);

    // step 3: Initialize deflate, recompress decompressed data, and end deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    uLong decompressed_len = inf.total_out;
    uLong compBound2 = deflateBound(&def, decompressed_len);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);
    def.next_in = outBuf;
    def.avail_in = (uInt)decompressed_len;
    def.next_out = compBuf2;
    def.avail_out = (uInt)compBound2;
    deflate(&def, 4);
    deflateEnd(&def);

    // step 4: Write decompressed data to gz file, cleanup, and finalize
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)decompressed_len);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(compBuf2);
    free(outBuf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}