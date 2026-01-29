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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzflush(gz_out, 2);
    gzclose(gz_out);

    // step 2: Compress with deflate (Initialize → Configure → Operate)
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
    deflate(&dstrm, 4);
    uLong compLen = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Decompress with inflate and use inflatePrime (Validate)
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    inflatePrime(&istrm, 3, 5);
    inflate(&istrm, 4);
    uLong decompressed = istrm.total_out;
    uLong crc_decomp = crc32(0UL, out, (uInt)decompressed);
    uLong crc_combined = crc32_combine(crc_input, crc_decomp, (off_t)decompressed);
    inflateEnd(&istrm);

    // step 4: Read back gzip file, validate CRC and cleanup
    gzFile gz_in = gzopen64("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readBuf, 0, (size_t)INPUT_LEN);
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)INPUT_LEN);
    uLong crc_gz = crc32(0UL, readBuf, (uInt)read_bytes);
    gzclose(gz_in);
    free(input);
    free(comp);
    free(out);
    free(readBuf);
    (void)ver;
    (void)bound;
    (void)crc_input;
    (void)crc_decomp;
    (void)crc_combined;
    (void)crc_gz;
    // API sequence test completed successfully
    return 66;
}