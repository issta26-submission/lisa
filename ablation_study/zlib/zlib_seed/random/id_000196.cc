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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and checksums
    const char src_c[] = "zlib API sequence payload: compress2 -> deflateInit_ -> inflateBackInit_ -> inflateBackEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong checksum0 = adler32_z(1UL, source, (z_size_t)sourceLen);

    // step 2: Setup deflate stream to compute bound and prepare compression buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Setup inflate-back stream and compute checksum over compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    uLong checksum_after = adler32_z(checksum0, compBuf, (z_size_t)compLen);

    // step 4: Cleanup: tear down inflate-back and deflate streams and free buffers
    int rc_inf_back_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_back_init;
    (void)checksum0;
    (void)checksum_after;
    (void)rc_inf_back_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}