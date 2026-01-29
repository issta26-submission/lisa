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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute CRC and compress it
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ -> gzseek64 -> gzgetc_ -> gzclose";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflateBack with a sliding window (configure)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = (unsigned char *)malloc((size_t)windowSize);
    memset(window, 0, (size_t)windowSize);
    const char *version = zlibVersion();
    int rc_inf_back_init = inflateBackInit_(&istrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Operate on gzFile: write original data, seek with gzseek64, read a byte with gzgetc_, then close
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    off64_t pos = gzseek64(gz, (off64_t)0, 0);
    int first_char = gzgetc_(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate and cleanup: compute CRC over compressed buffer, finalize inflateBack and free resources
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);
    int rc_inf_back_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(window);

    (void)crc_src;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_back_init;
    (void)rc_gz_write;
    (void)pos;
    (void)first_char;
    (void)rc_gz_close;
    (void)crc_comp;
    (void)rc_inf_back_end;
    (void)version;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}