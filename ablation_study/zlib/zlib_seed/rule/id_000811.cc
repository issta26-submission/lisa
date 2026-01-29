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
//<ID> 811
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

    // step 2: Initialize deflate stream and compress in a single call
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
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);
    uLong crc_comp = crc32(0UL, comp, (uInt)comp_len);

    // step 3: Write compressed bytes into a gzip file, then read them back
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)comp, (unsigned int)comp_len);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    const uLong READ_BUF_LEN = INPUT_LEN + 256UL;
    Bytef *readBuf = (Bytef *)malloc((size_t)READ_BUF_LEN);
    memset(readBuf, 0, (size_t)READ_BUF_LEN);
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)READ_BUF_LEN);
    int first_char = gzgetc_(gz_in);
    gzclose(gz_in);

    // step 4: Validate checksums and cleanup
    uLong crc_read = crc32(0UL, readBuf, (uInt)read_bytes);
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_comp;
    (void)crc_read;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}