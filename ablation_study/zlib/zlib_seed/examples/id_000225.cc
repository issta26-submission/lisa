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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress with compress()
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzwrite -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Decompress once using uncompress()
    uLongf decompLen1 = (uLongf)(sourceLen + 32);
    Bytef *decompBuf1 = (Bytef *)malloc((size_t)decompLen1);
    memset(decompBuf1, 0, (size_t)decompLen1);
    int rc_uncomp = uncompress(decompBuf1, &decompLen1, compBuf, compLen);

    // step 3: Initialize inflate stream and decompress using inflate()
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompLen2 = (uLongf)(sourceLen + 128);
    Bytef *decompBuf2 = (Bytef *)malloc((size_t)decompLen2);
    memset(decompBuf2, 0, (size_t)decompLen2);
    istrm.next_out = decompBuf2;
    istrm.avail_out = (uInt)decompLen2;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;

    // step 4: Write inflated output to a gzip file, rewind it, cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf2, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf1);
    free(decompBuf2);
    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}