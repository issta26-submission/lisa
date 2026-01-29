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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary slice, and compress the source data
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // derive a small dictionary from the source to use with inflateSetDictionary
    const uInt dictLen = (uInt) (sourceLen > 16 ? 16u : (uInt)sourceLen);
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, source, (size_t)dictLen);

    // step 2: Initialize an inflate stream and configure it with the derived dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    int rc_setdict = inflateSetDictionary(&strm, dict, dictLen);

    // step 3: Uncompress the compressed buffer into a destination buffer using uncompress2
    uLong srcLenForUncompress = (uLong)compLen;
    uLongf destLen = (uLongf)sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp2 = uncompress2(dest, &destLen, comp, &srcLenForUncompress);

    // perform a lightweight use of the stream fields to keep data flow meaningful
    strm.next_out = dest;
    strm.avail_out = (uInt)destLen;

    // step 4: Cleanup inflate stream and allocated resources
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    free(dict);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_uncomp2;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}