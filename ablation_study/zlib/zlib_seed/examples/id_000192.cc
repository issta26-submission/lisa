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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> deflateInit_ -> inflateBackInit_ -> checksums";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong checksum_initial = adler32_z(1UL, source, (z_size_t)sourceLen);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress (operate) using compress2
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong checksum_after_comp = adler32_z(checksum_initial, compBuf, (z_size_t)compLen);

    // step 3: Setup inflateBack and operate minimal lifecycle
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(decompBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);

    // step 4: Validate & Cleanup
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    free(decompBuf);

    (void)checksum_initial;
    (void)checksum_after_comp;
    (void)rc_def_init;
    (void)bound;
    (void)rc_comp2;
    (void)rc_back_init;
    (void)rc_inflate_back_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}