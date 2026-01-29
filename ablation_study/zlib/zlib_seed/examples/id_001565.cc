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
//<ID> 1565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload to exercise deflateBound, inflateUndermine, inflateResetKeep and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream, compute bound, write gz file)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&defstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int wrote = gzwrite(gzf, src, (unsigned int)srcLen);
    off_t pos = gztell(gzf);
    int rc_close = gzclose(gzf);

    // step 3: Operate (initialize inflate stream, apply undermine and reset-keep)
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_undermine = inflateUndermine(&instrm, 1);
    int rc_reset_keep = inflateResetKeep(&instrm);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_deflate_end = deflateEnd(&defstrm);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)rc_def_init;
    (void)compBound;
    (void)wrote;
    (void)pos;
    (void)rc_close;
    (void)rc_inflate_init;
    (void)rc_undermine;
    (void)rc_reset_keep;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}