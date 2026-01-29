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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed payload with compress2
    const char src[] = "zlib test payload: prepare -> inflateInit_ -> inflateBackInit_ -> gztell usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize a standard inflate stream and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 16;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 3: Initialize an inflateBack stream (alternate mode) with a window buffer, then tear it down
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&bstrm);

    // step 4: Write decompressed output to a gzFile, query position with gztell, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)istrm.total_out);
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}