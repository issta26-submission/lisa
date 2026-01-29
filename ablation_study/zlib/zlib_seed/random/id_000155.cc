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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> write gz -> deflate -> inflateBackInit_ -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Write compressed data to a gz file (use gzprintf and gzwrite)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "OriginalSize=%lu\n", (unsigned long)sourceLen);
    int rc_gzwrite = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gz);

    // step 3: Stream deflate of original source and initialize inflate-back with a window
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    Bytef *defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&defstrm, 0);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflateBackInit = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Cleanup & Validate
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inflateBackEnd = inflateBackEnd(&istrm);
    free(compBuf);
    free(defOut);
    free(window);
    (void)rc_comp; (void)rc_gzprintf; (void)rc_gzwrite; (void)rc_gzclose;
    (void)rc_def_init; (void)rc_deflate; (void)rc_inflateBackInit;
    (void)rc_def_end; (void)rc_inflateBackEnd;
    // API sequence test completed successfully
    return 66;
}