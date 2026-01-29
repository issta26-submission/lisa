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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateBackInit_ -> inflateBackEnd -> deflateEnd -> adler32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup & Compress
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: InflateBack init & End
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    int rc_infb_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    bstrm.next_in = compBuf;
    bstrm.avail_in = (uInt)compLen;
    int rc_infb_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup & finalization
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)checksum;
    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_infb_init;
    (void)rc_infb_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}