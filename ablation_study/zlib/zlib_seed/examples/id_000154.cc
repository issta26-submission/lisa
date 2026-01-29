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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream ibackstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&ibackstrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> write gz -> inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *defOutBuf = (Bytef *)malloc((size_t)bound);
    memset(defOutBuf, 0, (size_t)bound);
    uLongf defOutLen = (uLongf)bound;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Setup & Compress / Deflate
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = defOutBuf;
    defstrm.avail_out = (uInt)defOutLen;
    int rc_deflate = deflate(&defstrm, 0);

    // step 3: Operate (write compressed data and header to gz file)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "zlib-seq: size=%lu", (unsigned long)compLen);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup (inflateBack init/end, deflate end, free buffers)
    int rc_infb_init = inflateBackInit_(&ibackstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infb_end = inflateBackEnd(&ibackstrm);
    int rc_def_end = deflateEnd(&defstrm);
    free(compBuf);
    free(defOutBuf);
    free(window);
    (void)rc_comp; (void)rc_def_init; (void)rc_deflate;
    (void)rc_gz_printf; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_infb_init; (void)rc_infb_end; (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}