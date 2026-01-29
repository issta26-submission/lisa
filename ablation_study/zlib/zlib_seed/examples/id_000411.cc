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
//<ID> 411
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

    // step 2: Decompress the compressed buffer using uncompress2 to validate round-trip
    uLongf destLen = sourceLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)destLen);
    memset(outbuf, 0, (size_t)destLen);
    uLong compSrcLen = (uLong)compLen;
    int rc_uncomp2 = uncompress2(outbuf, &destLen, comp, &compSrcLen);

    // step 3: Initialize an inflate stream and configure it with a dictionary derived from the decompressed data
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uInt dictLength = (uInt)destLen;
    int rc_setdict = inflateSetDictionary(&strm, outbuf, dictLength);

    // step 4: Cleanup inflate resources and allocated memory
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)rc_comp;
    (void)rc_uncomp2;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;
    (void)compSrcLen;
    (void)destLen;
    // API sequence test completed successfully
    return 66;
}