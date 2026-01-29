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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary, and compress the source into a buffer
    const char src[] = "zlib API sequence payload: compress -> uncompress2 -> inflateInit_ -> inflateSetDictionary -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char dict[] = "preset-dictionary";
    const Bytef *dictionary = (const Bytef *)dict;
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: One-shot decompress the compressed buffer with uncompress2 to validate content
    uLongf dest1Len = (uLongf)sourceLen;
    Bytef *dest1 = (Bytef *)malloc((size_t)dest1Len);
    memset(dest1, 0, (size_t)dest1Len);
    uLong comp_remaining = (uLong)compLen;
    int rc_uncomp2 = uncompress2(dest1, &dest1Len, comp, &comp_remaining);

    // step 3: Initialize an inflate stream, optionally set a dictionary, then perform inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen;
    Bytef *dest2 = (Bytef *)malloc((size_t)outBound);
    memset(dest2, 0, (size_t)outBound);
    strm.next_out = dest2;
    strm.avail_out = (uInt)outBound;
    int rc_setdict = inflateSetDictionary(&strm, dictionary, dictLen);
    int rc_inflate = inflate(&strm, 0);
    uLong inflated_total = strm.total_out;
    uLong sum1 = adler32(0L, dest1, (uInt)dest1Len);
    uLong sum2 = adler32(0L, dest2, (uInt)inflated_total);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Cleanup allocated memory and finalize
    free(comp);
    free(dest1);
    free(dest2);
    (void)version;
    (void)rc_comp;
    (void)rc_uncomp2;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)compLen;
    (void)comp_remaining;
    (void)sum1;
    (void)sum2;
    // API sequence test completed successfully
    return 66;
}