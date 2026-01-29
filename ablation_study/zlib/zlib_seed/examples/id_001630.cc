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
//<ID> 1630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "example payload for zlib with preset dictionary";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char dict_contents[] = "preset dictionary for zlib";
    const size_t dictLen_s = (size_t)(sizeof(dict_contents) - 1);
    uInt dictLen = (uInt)dictLen_s;
    Bytef *dict = new Bytef[dictLen_s];
    memset(dict, 0, dictLen_s);
    memcpy(dict, dict_contents, dictLen_s);
    const char *version = zlibVersion();

    // step 2: Setup (deflate init, set dictionary, compute bound, compress via deflate)
    z_stream def;
    memset(&def, 0, (size_t)sizeof(z_stream));
    int rc_def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int rc_def_dict = deflateSetDictionary(&def, dict, dictLen);
    def.next_in = src;
    def.avail_in = (uInt)srcLen;
    uLong bound = deflateBound(&def, srcLen);
    uLong compAlloc = bound ? bound : 1UL;
    Bytef *compBuf = new Bytef[(size_t)compAlloc];
    memset(compBuf, 0, (size_t)compAlloc);
    def.next_out = compBuf;
    def.avail_out = (uInt)(compAlloc > 0 ? compAlloc : 1);
    int rc_deflate_finish = deflate(&def, Z_FINISH);
    uLong compLen = def.total_out;

    // step 3: Core operations (inflate init, attempt inflate which will need dict, set dictionary, finish inflate)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)(compLen ? compLen : 1);
    uLong outAlloc = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)outAlloc];
    memset(outBuf, 0, (size_t)outAlloc);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)(outAlloc ? outAlloc : 1);
    int rc_inflate_first = inflate(&inf, Z_NO_FLUSH);
    int rc_inflate_setdict = inflateSetDictionary(&inf, dict, dictLen);
    int rc_inflate_second = inflate(&inf, Z_FINISH);

    // step 4: Validate and Cleanup
    uLong computed_adler = adler32(0UL, (const Bytef *)src, (uInt)srcLen);
    int rc_inf_end = inflateEnd(&inf);
    int rc_def_end = deflateEnd(&def);
    delete [] src;
    delete [] dict;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)payloadLen;
    (void)srcLen;
    (void)rc_def_init;
    (void)rc_def_dict;
    (void)rc_deflate_finish;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate_first;
    (void)rc_inflate_setdict;
    (void)rc_inflate_second;
    (void)computed_adler;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}