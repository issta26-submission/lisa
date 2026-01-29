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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);

    // step 2: Initialize deflate and compress
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;

    // step 3: Write original data to gzip file, read back and inspect
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (const void *)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 16));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 16));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 16));
    int next_char = gzgetc_(gz_in);
    uLong crc_read = crc32(0UL, readBuf, (uInt)read_bytes);
    gzclose(gz_in);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_read;
    (void)comp_size;
    (void)next_char;
    // API sequence test completed successfully
    return 66;
}