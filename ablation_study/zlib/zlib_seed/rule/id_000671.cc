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
//<ID> 671
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf outCapacity = (uLongf)(src_len * 2);
    Bytef *outBuf = (Bytef *)malloc((size_t)outCapacity);
    memset(outBuf, 0, (size_t)outCapacity);

    // step 2: Initialize, configure and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong comp_len = def.total_out;
    deflateEnd(&def);

    // step 3: Decompress with uncompress2, initialize an inflate stream and call inflateBackEnd, exercise gzputc
    uLong comp_len_copy = comp_len;
    int unres = uncompress2(outBuf, &outCapacity, compBuf, &comp_len_copy);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateBackEnd(&inf);
    inflateEnd(&inf);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outBuf[0]);
    gzclose(gz);

    // step 4: Cleanup & finalization
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)unres;
    (void)comp_len;
    (void)comp_len_copy;
    // API sequence test completed successfully
    return 66;
}