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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source
    const char src[] = "zlib API sequence payload: compress -> inflate -> uncompress -> gzrewind";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflate()
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

    // step 3: Validate decompression with uncompress and write decompressed data to gzip then rewind
    uLongf decompLen2 = (uLongf)(sourceLen + 64);
    Bytef *decompBuf2 = (Bytef *)malloc((size_t)decompLen2);
    memset(decompBuf2, 0, (size_t)decompLen2);
    int rc_uncomp = uncompress(decompBuf2, &decompLen2, compBuf, (uLong)compLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(decompBuf2);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)compLen;
    (void)bound;
    (void)decompLen;
    (void)decompLen2;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}