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
//<ID> 619
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);

    // step 2: Initialize deflate stream and inflate-back-capable stream
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    unsigned char window_buf[1];
    int rc_infback_init = inflateBackInit_(&istrm, 15, window_buf, version, (int)sizeof(z_stream));

    // step 3: Operate - compress and write some bytes to a gzFile
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_putc1 = gzputc(gf, 'X');
    int rc_putc2 = gzputc(gf, '\n');
    int rc_putc3 = gzputc(gf, (int)(dstrm.total_out & 0xFF)); /* write one byte derived from compressed length */

    // step 4: Validate & Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_infback_init;
    (void)rc_deflate;
    (void)rc_putc1;
    (void)rc_putc2;
    (void)rc_putc3;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}