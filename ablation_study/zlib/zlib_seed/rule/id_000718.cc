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
//<ID> 718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input buffers and dictionary
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    Bytef *dict = (Bytef *)malloc((size_t)64);
    memset(dict, 'D', (size_t)64);
    uInt dict_len = (uInt)64;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = 0;
    uLong OUT_LEN = src_len * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize, configure and run deflate to produce compressed data using a preset dictionary
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dict, dict_len);
    deflate(&def, 4);
    compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, copy & reset a backup, set dictionary and inflate the data
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    inflateReset(&inf_copy);
    inflateSetDictionary(&inf, dict, dict_len);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    uLong outLen = inf.total_out;
    inflateEnd(&inf);
    inflateEnd(&inf_copy);

    // step 4: Finish, write one byte to a gz file and cleanup buffers
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outBuf[0]);
    gzclose(gz);
    free(input);
    free(dict);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    (void)ver;
    (void)compLen;
    (void)outLen;
    return 66;
}