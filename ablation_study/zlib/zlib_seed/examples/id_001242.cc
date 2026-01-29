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
//<ID> 1242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Test payload for zlib compression and inflate dictionary usage";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate, compute bound, compress)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong compSize = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Core operations (initialize inflate, set dictionary, inflate)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef dict[] = "preset_inflate_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dict, dictLen);
    Bytef *decompBuf = new Bytef[(size_t)srcLen];
    memset(decompBuf, 0, (size_t)srcLen);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 4); /* 4 == Z_FINISH */

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] decompBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_set_dict;
    (void)rc_inflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}