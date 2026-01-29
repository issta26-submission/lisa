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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    unsigned char input1[128];
    memset(input1, 'A', sizeof(input1));
    uLong input1Len = (uLong)sizeof(input1);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong compBound = deflateBound(&def_strm, input1Len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input1;
    def_strm.avail_in = (uInt)input1Len;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    unsigned char input2[32];
    memset(input2, 'B', sizeof(input2));
    uLong input2Len = (uLong)sizeof(input2);
    def_strm.next_in = (Bytef *)input2;
    def_strm.avail_in = (uInt)input2Len;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 4);
    uInt compLen = (uInt)def_strm.total_out;

    // step 3: Operate & Validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)input2[0]);
    gzputc(gz, (int)input2[1]);
    gzputc(gz, (int)'\n');

    // step 4: Cleanup
    deflateEnd(&def_strm);
    gzclose(gz);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}