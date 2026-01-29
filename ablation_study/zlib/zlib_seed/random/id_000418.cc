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
//<ID> 418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it one-shot using compress()
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflateSetDictionary -> uncompress2 -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and configure a dictionary derived from the source
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uInt dictLen = (uInt)((sourceLen < 32u) ? sourceLen : 32u);
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, source, (size_t)dictLen);
    int rc_set_dict = inflateSetDictionary(&strm, (const Bytef *)dict, dictLen);

    // step 3: Operate: uncompress the previously produced compressed buffer using uncompress2()
    uLong outBound = sourceLen + 16u;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    uLongf destLen = (uLongf)outBound;
    uLong srcConsumed = (uLong)compLen;
    int rc_uncomp2 = uncompress2(outbuf, &destLen, comp, &srcConsumed);

    // step 4: Cleanup and validation placeholders (no branches). Release resources and finalize inflate stream.
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dict);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_set_dict;
    (void)rc_uncomp2;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}