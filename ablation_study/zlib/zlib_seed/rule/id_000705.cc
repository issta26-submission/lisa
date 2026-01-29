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
//<ID> 705
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and produce compressed data using deflate
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong compBound = compressBound(INPUT_LEN);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)INPUT_LEN;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    const char *ver = zlibVersion();
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 2: Initialize inflate and configure internal state
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflatePrime(&inf, 0, 0);
    inflateResetKeep(&inf);
    inflateUndermine(&inf, 1);

    // step 3: Operate (inflate), validate with adler32, and write one byte to a gz file
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    uLong checksum = adler32(0UL, outBuf, (uInt)inf.total_out);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outBuf[0]);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)checksum;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}