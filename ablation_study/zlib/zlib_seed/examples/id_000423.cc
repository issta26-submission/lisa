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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> inflateBack test -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and configure input/output buffers
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 16;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;

    // step 3: Operate using inflateBack (with null callbacks for this sequence) and then combine checksums
    int rc_back = inflateBack(&strm, (in_func)NULL, (void *)NULL, (out_func)NULL, (void *)NULL);
    uLong sum_src = adler32(0L, source, (uInt)sourceLen);
    uLong sum_out = adler32(0L, outbuf, (uInt)strm.total_out);
    uLong combined = adler32_combine(sum_src, sum_out, (off_t)strm.total_out);

    // step 4: Validate and cleanup resources (close a gz handle, end inflate, free buffers)
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = gzclose_r(gz);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_back;
    (void)sum_src;
    (void)sum_out;
    (void)combined;
    (void)rc_gzclose_r;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}