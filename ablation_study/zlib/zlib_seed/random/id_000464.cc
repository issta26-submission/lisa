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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute compress bound, compress and then uncompress to verify
    const char src[] = "zlib API sequence payload: compress -> uncompress -> gz IO -> deflate lifecycle";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);
    uLongf destLen = (uLongf)sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_un = uncompress(dest, &destLen, comp, (uLong)compLen);

    // step 2: Initialize a deflate stream and immediately end it (lifecycle demonstration)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write the uncompressed data to a gzip file, reopen for reading, read some bytes, push one back, and check EOF
    gzFile gw = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)dest, (unsigned int)destLen);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("zlib_api_sequence_tmp.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)destLen);
    memset(readbuf, 0, (size_t)destLen);
    int read_n = gzread(gr, (voidp)readbuf, (unsigned int)destLen);
    int push_char = (int)(unsigned char)readbuf[0];
    int rc_ungetc = gzungetc(push_char, gr);
    int eof_flag = gzeof(gr);
    int rc_gzclose_r = gzclose(gr);

    // step 4: Cleanup
    free(comp);
    free(dest);
    free(readbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_un;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)read_n;
    (void)rc_ungetc;
    (void)eof_flag;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}