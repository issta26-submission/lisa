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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong INPUT_LEN = 512UL;
    const uLong TAIL_LEN = 64UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    Bytef *tail = (Bytef *)malloc((size_t)TAIL_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    memset(tail, 'B', (size_t)TAIL_LEN);
    const char *ver = zlibVersion();

    // step 2: Write data to a gzip file and flush
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzflush(gz_out, 2); /* Z_SYNC_FLUSH */
    gzclose(gz_out);

    // step 3: Compress the input, initialize inflate, prime and inflate
    uLong bound = compressBound(INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(comp, &compLen, input, INPUT_LEN, 6);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(outBuf, 0, (size_t)INPUT_LEN);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)INPUT_LEN;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);
    inflate(&istrm, 4); /* Z_FINISH */
    uLong decompressed_bytes = istrm.total_out;
    inflateEnd(&istrm);

    // step 4: Read back the gzip file, compute CRCs, combine and cleanup
    gzFile gz_in = gzopen64("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 16));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 16));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 16));
    gzclose(gz_in);

    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_tail = crc32(0UL, tail, (uInt)TAIL_LEN);
    uLong crc_combined = crc32_combine(crc_input, crc_tail, (off_t)TAIL_LEN);
    uLong crc_read = crc32(0UL, readBuf, (uInt)(read_bytes > 0 ? read_bytes : 0));

    free(input);
    free(tail);
    free(comp);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)bound;
    (void)decompressed_bytes;
    (void)crc_input;
    (void)crc_tail;
    (void)crc_combined;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}