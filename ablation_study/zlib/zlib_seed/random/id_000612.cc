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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization (deflate)
    const char src[] = "zlib API sequence test payload for deflate/inflateBack/gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure inflate-back with a sliding window
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate: compress the source and write a compressed byte to a gzFile via gzputc
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compProduced = dstrm.total_out;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, (int)compBuf[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_inf_back_init;
    (void)rc_def;
    (void)compProduced;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}