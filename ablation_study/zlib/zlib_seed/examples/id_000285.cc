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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into buffer
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> write-gz -> error-check";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;

    // step 3: Operate: inflate compressed data and record any textual error
    int rc_inflate = inflate(&istrm, 0);
    const char *inflate_err_text = zError(rc_inflate);

    // step 4: Write decompressed data to a gzip file and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf = gzprintf(gz, (const char *)decompBuf);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gz);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)decompAlloc;
    (void)inflate_err_text;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)rc_inf_end;
    (void)rc_gzclose;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}