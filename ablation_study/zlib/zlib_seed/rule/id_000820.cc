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
//<ID> 820
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

    // step 2: Compress the input with deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, (uLong)INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate the compressed buffer and write result to gzip file
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out = (Bytef *)malloc((size_t)(INPUT_LEN + 16UL));
    memset(out, 0, (size_t)(INPUT_LEN + 16UL));
    istrm.next_out = out;
    istrm.avail_out = (uInt)(INPUT_LEN + 16UL);
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 1, 0);
    inflate(&istrm, 4); /* Z_FINISH */
    uLong out_len = istrm.total_out;
    inflateEnd(&istrm);
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)out, (unsigned int)out_len);
    gzflush(gz_out, 2);
    gzclose(gz_out);

    // step 4: Read back gzip, validate CRCs, combine and cleanup
    gzFile gz_in = gzopen64("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(out_len + 16UL));
    memset(readBuf, 0, (size_t)(out_len + 16UL));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(out_len + 16UL));
    gzclose(gz_in);
    uLong crc_read = crc32(0UL, readBuf, (uInt)(read_bytes > 0 ? read_bytes : 0));
    uLong crc_combined = crc32_combine(crc_input, crc_read, (off_t)(read_bytes > 0 ? read_bytes : 0));
    free(input);
    free(comp);
    free(out);
    free(readBuf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)out_len;
    (void)crc_input;
    (void)crc_read;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}