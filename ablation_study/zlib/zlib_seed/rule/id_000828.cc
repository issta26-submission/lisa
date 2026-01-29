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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256UL;
    const uLong EXTRA_LEN = 64UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    Bytef *extra = (Bytef *)malloc((size_t)EXTRA_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    memset(extra, 'B', (size_t)EXTRA_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_extra = crc32(0UL, extra, (uInt)EXTRA_LEN);
    uLong crc_combined = crc32_combine(crc_input, crc_extra, (off_t)EXTRA_LEN);

    // step 2: Compress input to memory and write gzip file
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    uLong comp_cap = compressBound(INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_cap);
    memset(comp, 0, (size_t)comp_cap);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_cap;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compressed_bytes = dstrm.total_out;
    deflateEnd(&dstrm);
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)comp, (unsigned int)compressed_bytes);
    gzflush(gz_out, 2); /* Z_SYNC_FLUSH */
    gzclose(gz_out);

    // step 3: Initialize inflate, prime bit buffer, perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_bytes;
    Bytef *outbuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(outbuf, 0, (size_t)INPUT_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)INPUT_LEN;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 0);
    inflate(&istrm, 4); /* Z_FINISH */
    uLong decompressed_bytes = istrm.total_out;
    inflateEnd(&istrm);

    // step 4: Cleanup
    free(input);
    free(extra);
    free(comp);
    free(outbuf);
    (void)ver;
    (void)crc_input;
    (void)crc_extra;
    (void)crc_combined;
    (void)compressed_bytes;
    (void)decompressed_bytes;
    // API sequence test completed successfully
    return 66;
}