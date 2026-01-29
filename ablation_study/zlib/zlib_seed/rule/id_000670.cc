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
//<ID> 670
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf outBufLen = (uLongf)(INPUT_LEN * 2);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufLen);
    memset(outBuf, 0, (size_t)outBufLen);

    // step 2: Initialize, configure and run deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compSize = def.total_out;
    deflateEnd(&def);

    // step 3: Decompress with uncompress2 and initialize inflate
    uLong sourceLen = compSize;
    uLongf destLen = outBufLen;
    uncompress2(outBuf, &destLen, compBuf, &sourceLen);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)sourceLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)outBufLen;
    inflate(&inf, 0);
    inflateEnd(&inf);

    // step 4: Inflate-back init/end, gz operations and Cleanup
    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char window[1];
    inflateBackInit_(&infb, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&infb);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'X');
    gzputc(gz, '\n');
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)destLen;
    (void)compSize;
    // API sequence test completed successfully
    return 66;
}