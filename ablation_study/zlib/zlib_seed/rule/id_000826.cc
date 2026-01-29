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
//<ID> 826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong DATA_LEN = 1024UL;
    Bytef *data = (Bytef *)malloc((size_t)DATA_LEN);
    memset(data, 'A', (size_t)DATA_LEN);
    uLong crc_orig = crc32(0UL, data, (uInt)DATA_LEN);

    // step 2: Write data to a gzip file and flush
    gzFile gz_out = gzopen64("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)data, (unsigned int)DATA_LEN);
    gzflush(gz_out, 2);
    gzclose(gz_out);

    // step 3: Read back, initialize inflate, prime and compute combined CRC
    gzFile gz_in = gzopen64("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(DATA_LEN + 128UL));
    memset(readBuf, 0, (size_t)(DATA_LEN + 128UL));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(DATA_LEN + 128UL));
    uLong crc_read = crc32(0UL, readBuf, (uInt)read_bytes);
    uLong crc_combined = crc32_combine(crc_orig, crc_read, (off_t)DATA_LEN);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)read_bytes;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);
    inflateEnd(&istrm);

    // step 4: Cleanup
    gzclose(gz_in);
    free(data);
    free(readBuf);
    (void)ver;
    (void)crc_orig;
    (void)crc_read;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}