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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> write gz -> inflateBackInit";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup & Compress
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defBound);
    memset(defOut, 0, (size_t)defBound);
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defBound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong deflated_used = dstrm.total_out;

    // step 3: Operate (write to gz)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, defOut, (unsigned int)deflated_used);
    int rc_gz_printf = gzprintf(gz, "Wrote deflated payload\n");
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&istrm);

    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(defOut);

    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)deflated_used;
    (void)rc_gz_write;
    (void)rc_gz_printf;
    (void)rc_gz_close;
    (void)rc_infback_init;
    (void)rc_infback_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}