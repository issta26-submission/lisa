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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    (void)compress2(comp, &compLen, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and a deflate stream, then exercise deflateResetKeep
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int rc_inf_init = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    (void)rc_inf_init;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out, 0, (size_t)ORIG_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)ORIG_LEN;
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    int rc_def_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    (void)rc_def_init;
    deflateResetKeep(&dstrm);

    // step 3: Operate: inflate compressed data, compute CRC, write decompressed data via gzdopen
    inflate(&istrm, 4);
    uInt outLen = (uInt)istrm.total_out;
    uLong crc_out = crc32(0UL, out, outLen);
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, out, (unsigned int)outLen);
    gzclose(gz);

    // step 4: Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(out);
    (void)ver;
    (void)crc_orig;
    (void)crc_out;
    (void)bound;
    (void)compLen;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}