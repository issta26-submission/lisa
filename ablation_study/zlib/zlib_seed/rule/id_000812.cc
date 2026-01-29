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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);

    // step 2: Initialize deflate stream and prepare compression buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)(compBound));
    memset(comp, 0, (size_t)compBound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Operate: compress once, write/read gzip file, compute CRCs
    deflate(&dstrm, 0);
    deflateEnd(&dstrm);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_byte = gzgetc_(gz_in);
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 16));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 16));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 16));
    uLong crc_read = crc32(0UL, (const Bytef *)&first_byte, 1U);
    crc_read = crc32(crc_read, readBuf, (uInt)read_bytes);
    gzclose(gz_in);

    // step 4: Cleanup
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}