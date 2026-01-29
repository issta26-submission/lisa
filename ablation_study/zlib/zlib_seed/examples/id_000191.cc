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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> adler32_z -> inflateBackInit_ -> deflateEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup & Compress
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate & Validate (inflateBack init and end)
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 8));
    memset(window, 0, (size_t)(1 << 8));
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    int rc_ib_init = inflateBackInit_(&ibstrm, 8, window, version, (int)sizeof(z_stream));
    int rc_ib_end = inflateBackEnd(&ibstrm);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)rc_comp2;
    (void)checksum;
    (void)rc_def_init;
    (void)rc_ib_init;
    (void)rc_ib_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}