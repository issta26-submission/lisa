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
//<ID> 662
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *defComp = (Bytef *)malloc((size_t)compBound);
    memset(defComp, 0, (size_t)compBound);
    Bytef *cmp2Buf = (Bytef *)malloc((size_t)compBound);
    memset(cmp2Buf, 0, (size_t)compBound);
    uLongf cmp2Len = (uLongf)compBound;

    // step 2: Initialize and run deflate, and also run compress2
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = defComp;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compLen = def.total_out;
    compress2(cmp2Buf, &cmp2Len, input, src_len, 6);

    // step 3: Write compressed data to a gzip file, reopen via descriptor and use gzungetc
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, defComp, (unsigned int)compLen);
    gzclose(gz);
    FILE *f = fopen("test_zlib_api_sequence.gz", "rb");
    int fd = fileno(f);
    int dupfd = dup(fd);
    gzFile gfd = gzdopen(dupfd, "rb");
    int ch = gzgetc(gfd);
    int unret = gzungetc(ch, gfd);
    gzclose(gfd);
    fclose(f);

    // step 4: Cleanup & Validate-like operations
    uLong crc = crc32(0L, input, (uInt)src_len);
    deflateEnd(&def);
    free(input);
    free(defComp);
    free(cmp2Buf);
    (void)ver;
    (void)cmp2Len;
    (void)compLen;
    (void)crc;
    (void)ch;
    (void)unret;
    // API sequence test completed successfully
    return 66;
}