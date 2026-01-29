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
//<ID> 737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRC
    const char src[] = "zlib_seq_test_payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Persist a few bytes to a gzFile using gzopen and gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_put0 = gzputc(gf, (int)src[0]);
    int rc_put1 = gzputc(gf, (int)src[1]);
    int rc_put2 = gzputc(gf, (int)src[2]);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize a deflate stream, perform a single deflate call, then end it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Initialize an inflate stream to demonstrate inflateSyncPoint usage and then clean up
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end = inflateEnd(&istrm);

    free(outBuf);
    (void)crc_before;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_gzclose;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_sync;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}