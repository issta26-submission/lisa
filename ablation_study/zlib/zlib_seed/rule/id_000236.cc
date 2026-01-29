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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&def_copy, 0, sizeof(def_copy));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong checksum_crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)compBound);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound);
    memset(compBuf1, 0, (size_t)compBound);
    memset(compBuf2, 0, (size_t)compBound);

    // step 3: Core operations
    deflateCopy(&def_copy, &def_strm);

    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf1;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);

    def_copy.next_in = (Bytef *)input;
    def_copy.avail_in = (uInt)inputLen;
    def_copy.next_out = compBuf2;
    def_copy.avail_out = (uInt)compBound;
    deflate(&def_copy, 0);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf1, (unsigned int)def_strm.total_out);
    gzwrite(gz, compBuf2, (unsigned int)def_copy.total_out);

    uLong combined_adler = adler32(checksum_adler, compBuf1, (uInt)def_strm.total_out);
    uLong combined_crc = crc32(checksum_crc, compBuf2, (uInt)def_copy.total_out);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    deflateEnd(&def_copy);
    gzclose(gz);
    free(compBuf1);
    free(compBuf2);

    // API sequence test completed successfully
    (void)combined_adler;
    (void)combined_crc;
    return 66;
}