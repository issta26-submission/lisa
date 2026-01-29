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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and dictionary
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    const uInt DICT_LEN = 32;
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);

    // step 2: Initialize and run deflate with a preset dictionary to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dict, DICT_LEN);
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, copy its state, reset the copy, set the dictionary and inflate to decompress
    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = compBuf;
    inf_src.avail_in = (uInt)compLen;
    inf_src.next_out = outBuf;
    inf_src.avail_out = (uInt)OUT_LEN;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf_src);
    inflateReset(&inf_copy);
    inflateSetDictionary(&inf_copy, dict, DICT_LEN);
    inflate(&inf_copy, 0);
    uLong checksum = adler32(0UL, outBuf, (uInt)inf_copy.total_out);

    // step 4: Cleanup resources
    inflateEnd(&inf_copy);
    inflateEnd(&inf_src);
    free(input);
    free(compBuf);
    free(outBuf);
    free(dict);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}