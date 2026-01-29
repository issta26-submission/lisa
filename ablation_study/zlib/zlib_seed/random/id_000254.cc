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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ -> gzseek64 -> gzgetc_ -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate-back state with a sliding window
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_ibinit = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Round-trip compressed data via a gzFile and perform seek + single-byte read
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    off64_t pos0 = gzseek64(gz, (off64_t)0, 0);
    int first_byte = gzgetc_(gz);

    // step 4: Validate via crc32_z, teardown inflate-back, close gz and free buffers
    uLong crc = crc32_z(0UL, compBuf, (z_size_t)compLen);
    int rc_gz_close = gzclose(gz);
    int rc_ibend = inflateBackEnd(&istrm);
    free(compBuf);
    free(window);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_ibinit;
    (void)version;
    (void)rc_gz_write;
    (void)pos0;
    (void)first_byte;
    (void)crc;
    (void)rc_gz_close;
    (void)rc_ibend;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}