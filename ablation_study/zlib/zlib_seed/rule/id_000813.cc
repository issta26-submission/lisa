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
//<ID> 813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uInt compAlloc = (uInt)(INPUT_LEN + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Initialize deflate stream and compress
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compAlloc;
    deflate(&dstrm, 4);
    uLong comp_used = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write original data to gzip file, read it back, and use gzgetc_
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    uInt readBufLen = (uInt)(INPUT_LEN + 64);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int bytes_read = gzread(gz_in, (voidp)readBuf, (unsigned int)readBufLen);
    int next_char = gzgetc_(gz_in);
    gzclose(gz_in);

    // step 4: Compute CRCs, cleanup and finalize
    uLong crc_orig = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_read = crc32(0UL, readBuf, (uInt)bytes_read);
    free(input);
    free(compBuf);
    free(readBuf);
    (void)ver;
    (void)comp_used;
    (void)next_char;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}