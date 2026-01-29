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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and preparation
    const char src_c[] = "zlib API sequence payload: compress -> init inflate -> inflate -> report via gz";
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

    // step 3: Operate - set inputs, inflate into buffer, and write a report to a gzip file
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);

    const char *err_msg = zError(rc_inflate);
    gzFile gzf = gzopen("zlib_api_sequence_report.gz", "wb");
    int rc_gzprintf = gzprintf(gzf, "gz report: compressed=%lu decompressed=%lu inflate_rc=%d init_rc=%d comp_rc=%d err=%s\n",
                               (unsigned long)compLen, (unsigned long)istrm.total_out, rc_inflate, rc_inf_init, rc_comp, err_msg);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate (by collecting return codes) and cleanup
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
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}