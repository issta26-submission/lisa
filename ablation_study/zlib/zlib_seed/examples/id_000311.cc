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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source compression
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate copy -> gz I/O -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and create a copy with inflateCopy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: gzFile operations: open with gzopen and with gzdopen (via open fd), write compressed data, query positions
    gzFile gzf1 = gzopen("zlib_seq1.gz", "wb");
    int rc_gzwrite1 = 0;
    off_t tell1 = -1;
    off64_t off64_1 = (off64_t)-1;
    if (gzf1) {
        rc_gzwrite1 = gzwrite(gzf1, compBuf, (unsigned int)compLen);
        tell1 = gztell(gzf1);
        off64_1 = gzoffset64(gzf1);
    }

    int fd2 = open("zlib_seq2.gz", O_RDWR | O_CREAT | O_TRUNC, 0600);
    gzFile gzf2 = gzdopen(fd2, "wb");
    int rc_gzwrite2 = 0;
    off_t tell2 = -1;
    off64_t off64_2 = (off64_t)-1;
    if (gzf2) {
        rc_gzwrite2 = gzwrite(gzf2, compBuf, (unsigned int)compLen);
        tell2 = gztell(gzf2);
        off64_2 = gzoffset64(gzf2);
    }

    // step 4: Validation and cleanup
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose1 = gzclose(gzf1);
    int rc_gzclose2 = gzclose(gzf2);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_gzwrite1;
    (void)tell1;
    (void)off64_1;
    (void)rc_gzwrite2;
    (void)tell2;
    (void)off64_2;
    (void)rc_sync;
    (void)rc_inf_end_copy;
    (void)rc_inf_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;

    // API sequence test completed successfully
    return 66;
}