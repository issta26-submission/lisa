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
//<ID> 1564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload for deflateBound, inflateUndermine, inflateResetKeep and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream deflateStrm;
    z_stream inflateStrm;
    memset(&deflateStrm, 0, (size_t)sizeof(z_stream));
    memset(&inflateStrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Configure (initialize streams and compute bounds)
    int rc_deflate_init = deflateInit_(&deflateStrm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&deflateStrm, srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    deflateStrm.next_in = src;
    deflateStrm.avail_in = (uInt)srcLen;
    deflateStrm.next_out = comp;
    deflateStrm.avail_out = (uInt)compBound;
    int rc_inflate_init = inflateInit_(&inflateStrm, version, (int)sizeof(z_stream));
    inflateStrm.next_out = comp;
    inflateStrm.avail_out = (uInt)compBound;

    // step 3: Operate (invoke undermine/reset and use gz file/tell)
    int rc_undermine = inflateUndermine(&inflateStrm, 1);
    int rc_reset_keep = inflateResetKeep(&inflateStrm);
    const char *fname = "test_zlib_api_sequence_stream.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    off_t pos = gztell(gzf);
    int rc_close = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&deflateStrm);
    int rc_inflate_end = inflateEnd(&inflateStrm);
    delete [] src;
    delete [] comp;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)compBound;
    (void)rc_undermine;
    (void)rc_reset_keep;
    (void)written;
    (void)pos;
    (void)rc_close;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}