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
//<ID> 661
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
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef *comp2Buf = (Bytef *)malloc((size_t)compBound);
    memset(comp2Buf, 0, (size_t)compBound);
    uLongf comp2Len = (uLongf)compBound;

    // step 2: Initialize and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong deflatedSize = def.total_out;
    deflateEnd(&def);

    // step 3: Use compress2 and perform gz operations including gzdopen and gzungetc
    compress2(comp2Buf, &comp2Len, input, src_len, 6);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz1, comp2Buf, (unsigned int)comp2Len);
    int push1 = gzungetc('X', gz1);
    gzclose(gz1);
    gzFile gz2 = gzdopen(1, "wb"); // wrap stdout's fd for a transient gzFile
    gzwrite(gz2, input, (unsigned int)src_len);
    int push2 = gzungetc('Z', gz2);
    gzclose(gz2);

    // step 4: Validate & Cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    free(input);
    free(compBuf);
    free(comp2Buf);
    (void)ver;
    (void)deflatedSize;
    (void)comp2Len;
    (void)push1;
    (void)push2;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}