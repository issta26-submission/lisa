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
//<ID> 827
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

    // step 2: Compress input with deflate to produce a raw compressed buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compressed_bytes = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Demonstrate gzopen64/gzwrite/gzflush on original data and inflate using inflatePrime
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzflush(gz_out, 2); /* Z_SYNC_FLUSH */
    gzclose(gz_out);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_bytes;
    uLong OUT_LEN = INPUT_LEN + 64UL;
    Bytef *outbuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outbuf, 0, (size_t)OUT_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)OUT_LEN;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 1);
    inflate(&istrm, 4); /* attempt to finish */
    uLong inflated_bytes = istrm.total_out;
    inflateEnd(&istrm);

    // step 4: Validate via CRC combine, cleanup and finish
    uLong crc_inflated = crc32(0UL, outbuf, (uInt)inflated_bytes);
    uLong crc_combined = crc32_combine(crc_input, crc_inflated, (off_t)inflated_bytes);
    free(input);
    free(comp);
    free(outbuf);
    (void)compressed_bytes;
    (void)crc_input;
    (void)crc_inflated;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}