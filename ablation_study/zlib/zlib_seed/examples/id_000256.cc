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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute CRC, allocate window and initialize inflateBack stream
    const char src_c[] = "zlib API sequence payload: inflateBack init -> gz file seek/getc -> crc update";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_sz = (z_size_t)(sizeof(src_c) - 1);
    uLong crc_initial = crc32_z(0UL, source, src_sz);
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << 15));
    memset(window, 0, (size_t)(1u << 15));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 2: Open gz file (64-bit), write source into it
    gzFile gf = gzopen64("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gf, (voidpc)source, (unsigned int)src_sz);

    // step 3: Seek to beginning using gzseek64, read one byte with gzgetc_, update CRC with that byte
    off64_t seek_ret = gzseek64(gf, (off64_t)0, 0);
    int ch = gzgetc_(gf);
    unsigned char uch = (unsigned char)(ch & 0xFF);
    uLong crc_updated = crc32_z(crc_initial, (const Bytef *)&uch, (z_size_t)1);

    // step 4: Cleanup inflateBack stream, close gz file and free resources
    int rc_infback_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gf);
    free(window);

    (void)rc_infback_init;
    (void)rc_gz_write;
    (void)seek_ret;
    (void)ch;
    (void)crc_initial;
    (void)crc_updated;
    (void)rc_infback_end;
    (void)rc_gz_close;
    (void)version;
    (void)source;
    (void)src_sz;
    // API sequence test completed successfully
    return 66;
}