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
//<ID> 823
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
    const uInt DICT_LEN = 16U;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);
    uLong crc_input = crc32(0UL, input, (uInt)INPUT_LEN);
    uLong crc_dict = crc32(0UL, dict, (uInt)DICT_LEN);
    uLong crc_combined = crc32_combine(crc_input, crc_dict, (off_t)DICT_LEN);

    // step 2: Create gzip file, write and flush
    gzFile gz_out = gzopen64("tmp_zlib_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzflush(gz_out, 2); /* Z_SYNC_FLUSH (numeric constant used) */
    gzclose(gz_out);

    // step 3: Read back gzip file, init inflate stream and prime it
    gzFile gz_in = gzopen64("tmp_zlib_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 128UL));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 128UL));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 128UL));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)(read_bytes > 0 ? read_bytes : 0);
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 3, 5);

    // step 4: Validate and cleanup
    uLong crc_read = crc32(0UL, readBuf, (uInt)(read_bytes > 0 ? read_bytes : 0));
    uLong crc_final = crc32_combine(crc_combined, crc_read, (off_t)0);
    inflateEnd(&istrm);
    gzclose(gz_in);
    free(input);
    free(dict);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_dict;
    (void)crc_combined;
    (void)crc_read;
    (void)crc_final;
    // API sequence test completed successfully
    return 66;
}