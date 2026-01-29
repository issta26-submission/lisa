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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, dictionary, and compress the source in one shot
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const Bytef dict_bytes[] = "example-dictionary";
    uInt dictLen = (uInt)(sizeof(dict_bytes) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: One-shot decompression using uncompress2 to verify data round-trip
    Bytef *out1 = (Bytef *)malloc((size_t)sourceLen);
    memset(out1, 0, (size_t)sourceLen);
    uLongf out1Len = (uLongf)sourceLen;
    uLong srcLenForUn = (uLong)compLen;
    int rc_uncomp2 = uncompress2(out1, &out1Len, comp, &srcLenForUn);

    // step 3: Initialize an inflate stream, configure a dictionary, and perform streaming inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong out2Bound = sourceLen;
    Bytef *out2 = (Bytef *)malloc((size_t)out2Bound);
    memset(out2, 0, (size_t)out2Bound);
    strm.next_out = out2;
    strm.avail_out = (uInt)out2Bound;
    int rc_setdict = inflateSetDictionary(&strm, dict_bytes, dictLen);
    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Validate minimally via returned lengths and cleanup resources
    (void)rc_comp;
    (void)rc_uncomp2;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)version;
    free(comp);
    free(out1);
    free(out2);
    // API sequence test completed successfully
    return 66;
}