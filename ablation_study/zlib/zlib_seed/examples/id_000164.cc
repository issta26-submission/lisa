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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib API sequence payload: compute crc -> compress -> uncompress -> inflateBackInit_ -> gzputs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 1));
    memset(decompBuf, 0, (size_t)(sourceLen + 1));

    // step 2: Configure & operate: compute CRC, compress then uncompress
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLongf decompLen = (uLongf)(sourceLen + 1);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, compLen);
    decompBuf[decompLen] = 0; // ensure null-terminated for gzputs
    uLong crc_after = crc32_z(0UL, decompBuf, (z_size_t)decompLen);

    // step 3: Initialize inflateBack, write output to gz file
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, (const char *)decompBuf);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_infback_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_infback_init;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)rc_infback_end;
    // API sequence test completed successfully
    return 66;
}