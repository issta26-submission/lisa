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
//<ID> 818
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
    uLong crc_before = crc32(0UL, input, (uInt)INPUT_LEN);

    // step 2: Initialize deflate stream and perform a single-shot deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); // using Z_FINISH (4) to complete compression in one call

    // step 3: Write original data to a gzip file, then read it back and compute CRCs
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_char = gzgetc_(gz_in);
    Bytef first_byte = (Bytef)first_char;
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 16));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 16));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)INPUT_LEN);
    uLong crc_after_first = crc32(0UL, &first_byte, 1U);
    uLong crc_after_rest = crc32(crc_after_first, readBuf, (uInt)read_bytes);

    // step 4: Cleanup streams and free resources
    deflateEnd(&dstrm);
    gzclose(gz_in);
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)crc_before;
    (void)crc_after_rest;
    // API sequence test completed successfully
    return 66;
}