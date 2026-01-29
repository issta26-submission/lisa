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
//<ID> 1632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "example payload for zlib usage sequence with preset dictionary";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong checksum_initial = adler32(0UL, src, (uInt)payloadLen);

    // step 2: Setup (deflate init and allocate based on bound)
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
    int rc_deflate = deflate(&def, 0);
    uLong compLen = def.total_out;

    // step 3: Core operations (inflate init, supply dictionary, inflate)
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
    int rc_inflate_first = inflate(&inf, 0);

    // prepare a small preset dictionary (first 8 bytes of the original payload)
    Bytef dict[8];
    memset(dict, 0, 8);
    memcpy(dict, src, 8);
    uInt dictLen = (uInt)8;
    int rc_set_dict = inflateSetDictionary(&inf, dict, dictLen);
    int rc_inflate_second = inflate(&inf, 0);

    // step 4: Validate and Cleanup
    int rc_def_end = deflateEnd(&def);
    int rc_inf_end = inflateEnd(&inf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)checksum_initial;
    (void)rc_def_init;
    (void)bound;
    (void)compAlloc;
    (void)rc_deflate;
    (void)compLen;
    (void)outAlloc;
    (void)rc_inf_init;
    (void)rc_inflate_first;
    (void)rc_set_dict;
    (void)rc_inflate_second;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}