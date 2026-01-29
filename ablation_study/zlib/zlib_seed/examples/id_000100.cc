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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_src;
    z_stream inf_copy;
    memset(&inf_src, 0, sizeof(z_stream));
    memset(&inf_copy, 0, sizeof(z_stream));
    const char src_c[] = "Hello zlib gzwrite and inflateCopy test payload";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Setup (compress source into buffer and prepare inflate source)
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_inf_init = inflateInit_(&inf_src, version, (int)sizeof(z_stream));
    inf_src.next_in = compBuf;
    inf_src.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    inf_src.next_out = outBuf;
    inf_src.avail_out = (uInt)sourceLen;

    // step 3: Core operations (copy inflate state, write compressed data to gz and put a char)
    int rc_inf_copy = inflateCopy(&inf_copy, &inf_src);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_putc = gzputc(gz, 'X');
    int rc_gz_close = gzclose(gz);
    int rc_inf_proc_src = inflate(&inf_src, 0);
    int rc_inf_proc_copy = inflate(&inf_copy, 0);

    // step 4: Validate & Cleanup
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_gz_write;
    (void)rc_gz_putc;
    (void)rc_gz_close;
    (void)rc_inf_proc_src;
    (void)rc_inf_proc_copy;
    int rc_inf_end_src = inflateEnd(&inf_src);
    int rc_inf_end_copy = inflateEnd(&inf_copy);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    (void)rc_inf_end_src;
    (void)rc_inf_end_copy;
    return 66;
}