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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and provide a preset dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    const Bytef dict[] = "preset-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_setdict = inflateSetDictionary(&strm, dict, dictLen);

    // step 3: One-shot uncompress using uncompress2 with source length pointer
    uLong compSrcLen = (uLong)compLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outbuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress2(outbuf, &outLen, comp, &compSrcLen);

    // step 4: Cleanup inflate state and allocated memory
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_uncomp;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}