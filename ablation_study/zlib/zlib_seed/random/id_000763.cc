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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzseek, gzclose and inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf destLen = (uLongf)compAlloc;
    int rc_comp = compress(compBuf, &destLen, source, sourceLen);
    uLong compProduced = (uLong)destLen;

    // step 2: Persist compressed data to a gzip file and seek within it
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    off_t seek_pos = gzseek(gf, 0, 0); // SEEK_SET == 0
    int rc_gz_close = gzclose(gf);

    // step 3: Initialize inflate stream, point it to the compressed buffer, and validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_validate = inflateValidate(&istrm, 1);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)compProduced;
    (void)rc_gz_write;
    (void)seek_pos;
    (void)rc_gz_close;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}