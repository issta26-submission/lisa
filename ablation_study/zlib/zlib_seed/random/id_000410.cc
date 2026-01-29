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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary, and compress the source into a buffer
    const char src[] = "zlib API sequence payload: compress -> inflateInit_ -> inflateSetDictionary -> uncompress2 -> inflate -> cleanup";
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

    // step 2: Initialize an inflate stream and configure a preset dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_setdict = inflateSetDictionary(&strm, dictionary, dictLen);

    // step 3: Operate: use uncompress2 to one-shot decompress, then also run inflate on the compressed buffer
    Bytef *outbuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outbuf, 0, (size_t)sourceLen);
    uLongf outLen = sourceLen;
    uLong compLen_copy = (uLong)compLen;
    int rc_uncomp2 = uncompress2(outbuf, &outLen, comp, &compLen_copy);
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Validate by touching results and cleanup all resources
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_uncomp2;
    (void)rc_inflate;
    (void)rc_inf_end;
    free(comp);
    free(outbuf);
    // API sequence test completed successfully
    return 66;
}