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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib usage";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char dict_data[] = "preset_dictionary_for_inflate";
    const uInt dictLen = (uInt)(sizeof(dict_data) - 1);
    const char *version = zlibVersion();
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, (size_t)sizeof(z_stream));
    memset(&infStrm, 0, (size_t)sizeof(z_stream));
    Bytef *srcBuf = new Bytef[(size_t)srcLen];
    memset(srcBuf, 0, (size_t)srcLen);
    memcpy(srcBuf, payload, (size_t)srcLen);
    Bytef *dictBuf = new Bytef[(size_t)dictLen];
    memset(dictBuf, 0, (size_t)dictLen);
    memcpy(dictBuf, dict_data, (size_t)dictLen);

    // step 2: Setup (initialize deflate stream and compute bound)
    int rc_deflate_init = deflateInit_(&defStrm, 6, version, (int)sizeof(z_stream));
    uLong worst_bound = deflateBound(&defStrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)worst_bound];
    memset(compBuf, 0, (size_t)worst_bound);
    defStrm.next_in = srcBuf;
    defStrm.avail_in = (uInt)srcLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)worst_bound;

    // step 3: Core operations (initialize inflate stream and set dictionary)
    int rc_inflate_init = inflateInit_(&infStrm, version, (int)sizeof(z_stream));
    int rc_inflate_setdict = inflateSetDictionary(&infStrm, dictBuf, dictLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)worst_bound;

    // step 4: Cleanup and finalization
    defStrm.adler = 0UL;
    defStrm.total_in = srcLen;
    int rc_deflate_end = deflateEnd(&defStrm);
    int rc_inflate_end = inflateEnd(&infStrm);
    delete [] srcBuf;
    delete [] dictBuf;
    delete [] compBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)worst_bound;
    (void)rc_inflate_init;
    (void)rc_inflate_setdict;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}