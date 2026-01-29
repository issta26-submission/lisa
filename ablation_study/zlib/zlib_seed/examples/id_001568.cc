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
//<ID> 1568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload to exercise deflateBound, inflateResetKeep, inflateUndermine and gztell";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, payloadLen);
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 2: Configure (query deflateBound and prepare gz file)
    uLong bound = deflateBound(&defstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);

    // step 3: Operate (query gz position and manipulate inflate stream)
    off_t position = gztell(gzf);
    int rc_inf_reset_keep = inflateResetKeep(&infstrm);
    int rc_inf_undermine = inflateUndermine(&infstrm, 1);

    // step 4: Validate and Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)bound;
    (void)written;
    (void)position;
    (void)rc_inf_reset_keep;
    (void)rc_inf_undermine;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}