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
//<ID> 819
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
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);

    // step 2: Initialize and run deflate on the prepared buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    uLong COMP_LEN = INPUT_LEN + 128UL;
    Bytef *comp = (Bytef *)malloc((size_t)COMP_LEN);
    memset(comp, 0, (size_t)COMP_LEN);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)COMP_LEN;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compressed_bytes = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Read back the gzip file and inspect via gzread and gzgetc_
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 128UL));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 128UL));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 128UL));
    int single_char = gzgetc_(gz_in);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_output = crc32(0UL, readBuf, (uInt)read_bytes);

    // step 4: Cleanup
    gzclose(gz_in);
    free(input);
    free(comp);
    free(readBuf);
    (void)ver;
    (void)compressed_bytes;
    (void)single_char;
    (void)crc_input;
    (void)crc_output;
    // API sequence test completed successfully
    return 66;
}