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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> uncompress -> gz I/O -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and set a gzip header for it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(gz_header));
    Bytef *namebuf = (Bytef *)malloc(7);
    memcpy(namebuf, "sample", 7); // includes terminating NUL
    hdr.name = namebuf;
    Bytef *commbuf = (Bytef *)malloc(9);
    memcpy(commbuf, "comment", 8); // include terminating NUL in allocated space
    hdr.comment = commbuf;
    int rc_def_hdr = deflateSetHeader(&dstrm, &hdr);

    // step 3: Decompress using uncompress and write decompressed data to a gz stream opened via gzdopen
    uLongf destLen = sourceLen + 16;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_un = uncompress(dest, &destLen, comp, compLen);

    FILE *tmpf = tmpfile();
    int fd_orig = fileno(tmpf);
    int fd_dup = dup(fd_orig);
    gzFile gz = gzdopen(fd_dup, "wb");
    int rc_gzwrite = gzwrite(gz, dest, (unsigned int)destLen);
    gzclearerr(gz);
    int rc_gzclose = gzclose(gz);
    int rc_fclose = fclose(tmpf);

    // step 4: Cleanup allocated resources and deflate stream
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    free(namebuf);
    free(commbuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_hdr;
    (void)rc_un;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_fclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}