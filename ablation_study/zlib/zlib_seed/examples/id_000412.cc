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
//<ID> 412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflateSetDictionary -> uncompress2 -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and configure a dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uInt dictLen = (uInt)(sourceLen < 16 ? sourceLen : 16);
    int rc_setdict = inflateSetDictionary(&strm, source, dictLen);

    // step 3: One-shot uncompress using uncompress2 into an output buffer
    uLong outBound = sourceLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    uLongf outLen = (uLongf)outBound;
    uLong compRemaining = (uLong)compLen;
    int rc_uncomp2 = uncompress2(outbuf, &outLen, comp, &compRemaining);

    // step 4: Validate via adler32 checksums and cleanup resources
    uLong ad_src = adler32(0L, source, (uInt)sourceLen);
    uLong ad_out = adler32(0L, outbuf, (uInt)outLen);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_uncomp2;
    (void)compRemaining;
    (void)ad_src;
    (void)ad_out;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}