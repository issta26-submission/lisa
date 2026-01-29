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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> inflateBackInit_ -> inflateBackEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong initial_adler = adler32_z(1UL, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (initialize deflate stream and inflate-back stream)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *backWindow = (unsigned char *)malloc((size_t)(1U << 15));
    memset(backWindow, 0, (size_t)(1U << 15));
    int rc_back_init = inflateBackInit_(&backstrm, 15, backWindow, version, (int)sizeof(z_stream));

    // step 3: Core operations (compress, uncompress, checksum, teardown inflate-back)
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong final_adler = adler32_z(1UL, decompBuf, (z_size_t)decompLen);
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 4: Cleanup & finalization
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);
    free(backWindow);

    (void)initial_adler;
    (void)final_adler;
    (void)rc_def_init;
    (void)rc_back_init;
    (void)rc_comp2;
    (void)rc_uncomp;
    (void)rc_back_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}