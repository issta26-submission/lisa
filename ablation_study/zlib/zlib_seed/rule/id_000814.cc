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
//<ID> 814
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
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong crc_orig = crc32(0UL, input, (uInt)INPUT_LEN);

    // step 2: Initialize deflate stream and compress into a buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Create a gzip file with gz* APIs, then read it back and compute CRCs
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 128));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 128));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 128));
    int next_char = gzgetc_(gz_in); // call gzgetc_ on the gzFile
    (void)next_char;
    uLong crc_read = crc32(0UL, readBuf, (uInt)(read_bytes > 0 ? read_bytes : 0));
    gzclose(gz_in);

    // step 4: Cleanup resources
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}