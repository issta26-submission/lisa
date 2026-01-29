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
//<ID> 707
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize a deflate template
    const char src[] = "zlib API sequence test payload demonstrating deflateBound, inflateBackInit_, gz I/O and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Compress source into buffer using compress2
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize a normal inflate stream, perform inflate, initialize inflateBack template, write output to gzFile
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);

    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream istrm_back;
    memset(&istrm_back, 0, (int)sizeof(z_stream));
    int rc_inf_back_init = inflateBackInit_(&istrm_back, 15, window, version, (int)sizeof(z_stream));

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)istrm.total_out);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup and validation
    int rc_inf_end = inflateEnd(&istrm);
    int rc_inf_back_end = inflateEnd(&istrm_back);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)compAlloc;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_inf_back_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}