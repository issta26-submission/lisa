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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Decompress the compressed buffer using uncompress2
    uLongf outBufSize = (uLongf)(sourceLen + 16);
    Bytef *outbuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outbuf, 0, (size_t)outBufSize);
    uLong inConsumed = (uLong)compLen;
    int rc_uncomp2 = uncompress2(outbuf, &outBufSize, comp, &inConsumed);

    // step 3: Initialize an inflate stream and set a dictionary (demonstrate dictionary API usage)
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    const Bytef dict[] = "preset-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_inf_set_dict = inflateSetDictionary(&strm, dict, dictLen);

    // step 4: Validate/cleanup resources and inflate state
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)source;
    (void)sourceLen;
    (void)bound;
    (void)compLen;
    (void)rc_comp;
    (void)inConsumed;
    (void)rc_uncomp2;
    (void)version;
    (void)rc_inf_init;
    (void)dictLen;
    (void)rc_inf_set_dict;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}