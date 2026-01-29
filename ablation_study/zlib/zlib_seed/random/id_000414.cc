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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: One-shot compress the source into comp
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 3: One-shot uncompress the compressed data into a destination buffer using uncompress2
    uLongf destLen = sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    uLong compSrcLen = (uLong)compLen;
    int rc_uncomp2 = uncompress2(dest, &destLen, comp, &compSrcLen);

    // step 4: Initialize an inflate stream, set a dictionary, perform an inflate operation, then cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = dest;
    strm.avail_out = (uInt)destLen;
    int rc_setdict = inflateSetDictionary(&strm, source, (uInt)sourceLen);
    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);

    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_uncomp2;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}