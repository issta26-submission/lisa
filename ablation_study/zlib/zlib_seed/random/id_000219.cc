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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize deflate and compute bounds
    const char src_c[] = "zlib API sequence payload: compress2 -> inflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and decompress into a buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;

    // step 3: Write decompressed data to a gzip file and close it
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actual_out);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup deflate & inflate streams and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)actual_out;
    (void)bound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}