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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute its CRC using crc32_z
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ + gzfile operations";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong computed_crc = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Open a gz file, write the source, seek back to start and read first byte
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    off64_t pos_begin = gzseek64(gz, (off64_t)0, 0);
    int first_char = gzgetc_(gz);

    // step 3: Initialize an inflate-back stream using inflateBackInit_
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    const char *version = zlibVersion();
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    istrm.data_type = first_char; /* incorporate the gz-read byte into the stream state */

    // step 4: Cleanup inflate-back resources and gz file, free allocations
    int rc_inf_back_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(window);

    (void)computed_crc;
    (void)rc_gz_write;
    (void)pos_begin;
    (void)first_char;
    (void)rc_inf_back_init;
    (void)rc_inf_back_end;
    (void)rc_gz_close;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}