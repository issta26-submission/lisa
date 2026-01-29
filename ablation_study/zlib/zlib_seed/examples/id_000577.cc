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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer (one-shot compress)
    const char src[] = "zlib API payload: initialize inflateInit_ -> inflateBackInit_ -> inflate -> gzwrite -> gztell -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate streams (inflateInit_ for standard inflate, inflateBackInit_ for back inflater)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = (unsigned char *)malloc((size_t)windowSize);
    memset(window, 0, (size_t)windowSize);
    int rc_inflate_back_init = inflateBackInit_(&ibstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Operate: set up input/output for inflate, perform inflate once, and write decompressed output to a gzFile then query position
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&istrm, 0); /* Z_NO_FLUSH == 0 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)istrm.total_out);
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup: end inflate streams, cleanup back-inflate, free buffers and window
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&ibstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}