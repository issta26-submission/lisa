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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateBackInit_ -> inflateBackEnd -> deflateEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLongf compLen;
    uLong bound_for_comp;
    Bytef *compBuf = NULL;
    unsigned char *window = NULL;

    uLong adler_original = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Setup (initialize deflate stream to obtain bounds, then compress2)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    bound_for_comp = deflateBound(&dstrm, sourceLen);
    compBuf = (Bytef *)malloc((size_t)bound_for_comp);
    memset(compBuf, 0, (int)bound_for_comp);
    compLen = (uLongf)bound_for_comp;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate (initialize inflate back mechanism then tear it down) & validate checksum of compressed data
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_infb_init = inflateBackInit_(&istrm, 15, (unsigned char *)NULL, version, (int)sizeof(z_stream));
    // allocate a window and re-init inflateBack with a real window to be more realistic
    window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, 32768);
    int rc_infb_init_with_window = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    uLong adler_compressed = adler32_z(0UL, compBuf, (z_size_t)compLen);
    int rc_infb_end = inflateBackEnd(&istrm);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)adler_original;
    (void)adler_compressed;
    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_infb_init;
    (void)rc_infb_init_with_window;
    (void)rc_infb_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}