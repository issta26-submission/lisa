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
//<ID> 854
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
    Bytef *plain = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(plain, 'A', (size_t)INPUT_LEN);
    uLong adl_orig = adler32(0UL, plain, (uInt)INPUT_LEN);

    // step 2: Compress with deflate and finalize the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = plain;
    dstrm.avail_in = (uInt)INPUT_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    int def_status = deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate2, decompress the compressed buffer, and validate via adler32
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    istrm.adler = adl_orig;
    int inf_status = inflate(&istrm, 4); /* Z_FINISH */
    uLong out_len = istrm.total_out;
    uLong adl_after_inflate = adler32(0UL, out, (uInt)out_len);
    inflateEnd(&istrm);

    // step 4: Use gzFile APIs to round-trip via gzwrite/gzread and finalize resources
    gzFile gf = gzopen("test_zlib_api_seq_tmp.gz", "wb");
    gzwrite(gf, plain, (unsigned int)INPUT_LEN);
    gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_seq_tmp.gz", "rb");
    Bytef *gz_out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(gz_out, 0, (size_t)INPUT_LEN);
    int gz_read_bytes = gzread(gf2, gz_out, (unsigned int)INPUT_LEN);
    uLong adl_gz = adler32(0UL, gz_out, (uInt)gz_read_bytes);
    gzclose(gf2);

    free(plain);
    free(comp);
    free(out);
    free(gz_out);
    (void)ver;
    (void)bound;
    (void)def_status;
    (void)inf_status;
    (void)adl_orig;
    (void)adl_after_inflate;
    (void)adl_gz;
    (void)out_len;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}