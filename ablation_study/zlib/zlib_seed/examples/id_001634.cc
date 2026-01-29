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
//<ID> 1634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api payload for deflateBound, inflateSetDictionary, adler32 and cleanup";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong adler_src = adler32(0UL, src, (uInt)srcLen);

    // step 2: Setup (initialize deflate and determine bound, produce compressed data)
    z_stream def;
    memset(&def, 0, (size_t)sizeof(z_stream));
    def.next_in = src;
    def.avail_in = (uInt)srcLen;
    int rc_def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def, srcLen);
    uLong compAlloc = bound ? bound : 1UL;
    Bytef *compBuf = new Bytef[(size_t)compAlloc];
    memset(compBuf, 0, (size_t)compAlloc);
    def.next_out = compBuf;
    def.avail_out = (uInt)(compAlloc ? compAlloc : 1UL);
    int rc_deflate = deflate(&def, 4); /* 4 used as Z_FINISH */
    uLong compLen = def.total_out;
    int rc_def_end = deflateEnd(&def);

    // step 3: Core operations (initialize inflate, set dictionary, inflate, compute checksum)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)(compLen ? compLen : 1UL);
    uLong outAlloc = srcLen + 64UL;
    Bytef *outBuf = new Bytef[(size_t)outAlloc];
    memset(outBuf, 0, (size_t)outAlloc);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)(outAlloc ? outAlloc : 1UL);
    int rc_inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int rc_inf_setdict = inflateSetDictionary(&inf, src, (uInt)srcLen);
    int rc_inflate = inflate(&inf, 0);
    uLong outProduced = inf.total_out;
    uLong adler_out = adler32(0UL, outBuf, (uInt)(outProduced ? outProduced : 0UL));
    int rc_inf_end = inflateEnd(&inf);

    // step 4: Validate and Cleanup
    int rc_inflate_end_extra = inflateEnd(&inf); /* safe repeat call */
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)adler_src;
    (void)rc_def_init;
    (void)bound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_inflate;
    (void)outProduced;
    (void)adler_out;
    (void)rc_inf_end;
    (void)rc_inflate_end_extra;
    // API sequence test completed successfully
    return 66;
}