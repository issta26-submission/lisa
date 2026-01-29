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
//<ID> 825
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);

    // step 2: Write compressed data with gzopen64, flush and close
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzflush(gz_out, 4);
    gzclose(gz_out);

    // step 3: Read back the gzip file and compute combined CRC
    gzFile gz_in = gzopen64("tmp_zlib_api_seq.gz", "rb");
    uLong readBufLen = INPUT_LEN + 128UL;
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)readBufLen);
    uLong crc_output = crc32(0UL, readBuf, (uInt)read_bytes);
    uLong crc_combined = crc32_combine(crc_input, crc_output, (off_t)read_bytes);

    // step 4: Initialize inflate stream, prime it, cleanup everything
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);
    inflateEnd(&istrm);

    gzclose(gz_in);
    free(input);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_output;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}