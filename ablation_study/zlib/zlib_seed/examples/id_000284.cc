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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> write gz";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and decompress into buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Open a gz file and write the decompressed payload using gzprintf, query error text
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "%s", (const char *)decompBuf);
    int gz_errno = 0;
    const char *gz_errstr = gzerror(gz, &gz_errno);
    const char *inflate_errstr = zError(rc_inflate);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup inflate stream and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)decompAlloc;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)gz_errno;
    (void)gz_errstr;
    (void)inflate_errstr;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}