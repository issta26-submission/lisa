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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source/compression buffers
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateCopy -> write gz -> query positions";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate source stream and set input/output pointers
    z_stream istrm_src;
    memset(&istrm_src, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm_src, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm_src.next_in = compBuf;
    istrm_src.avail_in = (uInt)compLen;
    istrm_src.next_out = decompBuf;
    istrm_src.avail_out = (uInt)decompAlloc;

    // step 3: Copy inflate stream state and perform parallel inflate, then write compressed data to gz files and query positions
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&istrm_copy, &istrm_src);
    int rc_inf1 = inflate(&istrm_src, 0);
    int rc_inf2 = inflate(&istrm_copy, 0);

    gzFile gzf_file = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf_file, compBuf, (unsigned int)compLen);
    off_t tell_pos = gztell(gzf_file);
    gzFile gzf_stdout = gzdopen(1, "wb");
    int rc_gzwrite2 = gzwrite(gzf_stdout, compBuf, (unsigned int)(compLen ? 1U : 0U));
    off64_t offset64_file = gzoffset64(gzf_file);
    off64_t offset64_stdout = gzoffset64(gzf_stdout);

    // step 4: Cleanup streams, gz files and buffers
    int rc_inf_end_src = inflateEnd(&istrm_src);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_gzclose_file = gzclose(gzf_file);
    int rc_gzclose_stdout = gzclose(gzf_stdout);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)decompAlloc;
    (void)rc_copy;
    (void)rc_inf1;
    (void)rc_inf2;
    (void)rc_gzwrite1;
    (void)tell_pos;
    (void)rc_gzwrite2;
    (void)offset64_file;
    (void)offset64_stdout;
    (void)rc_inf_end_src;
    (void)rc_inf_end_copy;
    (void)rc_gzclose_file;
    (void)rc_gzclose_stdout;

    // API sequence test completed successfully
    return 66;
}