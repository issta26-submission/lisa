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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into buffer
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> report via gz";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Inflate compressed data into a buffer and obtain a textual error if any
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    const char *inflate_msg = zError(rc_inflate);

    // step 4: Report results to a gzip file and cleanup
    gzFile gzf = gzopen("/dev/null", "wb");
    int rc_gzprintf = gzprintf(gzf, "comp_rc=%d comp_len=%lu inflate_rc=%d msg=%s\n",
                               rc_comp, (unsigned long)compLen, rc_inflate, inflate_msg);
    int rc_gzclose = gzclose(gzf);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflate_msg;
    (void)gzf;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;
    (void)decompAlloc;

    // API sequence test completed successfully
    return 66;
}