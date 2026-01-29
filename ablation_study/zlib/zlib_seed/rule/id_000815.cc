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
//<ID> 815
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);

    // step 2: Initialize deflate stream and compress input in-memory
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* flush = Z_FINISH */
    uLong compLen = dstrm.total_out;
    uLong crc_comp = crc32(0UL, compBuf, (uInt)compLen);
    deflateEnd(&dstrm);

    // step 3: Write original data to a gzip file and read it back using gzgetc_ and gzread
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 2));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 2));
    int first_char = gzgetc_(gz_in);
    readBuf[0] = (Bytef)first_char;
    int read_rest = gzread(gz_in, (voidp)(readBuf + 1), (unsigned int)INPUT_LEN);
    uInt total_read = (uInt)read_rest + 1U;
    uLong crc_output = crc32(0UL, readBuf, total_read);
    gzclose(gz_in);

    // step 4: Cleanup
    free(input);
    free(compBuf);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_comp;
    (void)crc_output;
    // API sequence test completed successfully
    return 66;
}