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
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer
    const char src[] = "zlib API sequence payload: generate compressed data for inflate test";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform a one-shot inflate into outBuf
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(outBuf, 0, (size_t)(sourceLen + 16));
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 16);
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Initialize an inflateBack stream (with a window) and write decompressed data to a gzFile, then query position
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)istrm.total_out);
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup: end inflate streams and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_back_end = inflateBackEnd(&bstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_back_init;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}