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
//<ID> 1246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Alpha_data_for_deflate";
    const char payloadB[] = "Dict_bytes_for_inflate";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong srcLen = lenA;
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payloadA, (size_t)lenA);

    z_stream defstream;
    memset(&defstream, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defstream, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&defstream, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    defstream.next_in = src;
    defstream.avail_in = (uInt)srcLen;
    defstream.next_out = compBuf;
    defstream.avail_out = (uInt)bound;
    int rc_deflate_call = deflate(&defstream, 0);

    // step 3: Core operations
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate_setdict = inflateSetDictionary(&instrm, (const Bytef *)payloadB, (uInt)lenB);

    // step 4: Cleanup and validation
    defstream.adler = defstream.total_in;
    int rc_deflate_end = deflateEnd(&defstream);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_call;
    (void)rc_inflate_init;
    (void)rc_inflate_setdict;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}