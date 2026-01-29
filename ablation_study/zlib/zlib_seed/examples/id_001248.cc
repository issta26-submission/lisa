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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib usage";
    const char dict[] = "preset_dictionary";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const uInt dictLen = (uInt)(sizeof(dict) - 1);
    const char *version = zlibVersion();
    z_stream deflate_strm;
    z_stream inflate_strm;
    memset(&deflate_strm, 0, (size_t)sizeof(z_stream));
    memset(&inflate_strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream and compute output bound)
    int rc_deflate_init = deflateInit_(&deflate_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&deflate_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    deflate_strm.next_in = (Bytef *)payload;
    deflate_strm.avail_in = (uInt)srcLen;
    deflate_strm.next_out = compBuf;
    deflate_strm.avail_out = (uInt)bound;

    // step 3: Core operations (initialize inflate stream and set dictionary, propagate checksum)
    int rc_inflate_init = inflateInit_(&inflate_strm, version, (int)sizeof(z_stream));
    int rc_inflate_setdict = inflateSetDictionary(&inflate_strm, (const Bytef *)dict, dictLen);
    uLong payload_adler = adler32(0UL, (const Bytef *)payload, (uInt)srcLen);
    deflate_strm.adler = payload_adler;
    inflate_strm.total_in = payload_adler;

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&deflate_strm);
    int rc_inflate_end = inflateEnd(&inflate_strm);
    delete [] compBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_inflate_setdict;
    (void)payload_adler;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}