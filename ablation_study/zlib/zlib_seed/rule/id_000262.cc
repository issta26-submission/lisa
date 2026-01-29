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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    gz_header head;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&head, 0, sizeof(head));
    unsigned char rawData[256];
    memset(rawData, 'A', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    deflateInit2_(&def_strm, 6, 8, 31, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inflateInit2_(&inf_strm, 31, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, rawLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)rawData;
    def_strm.avail_in = (uInt)rawLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    deflate(&def_strm, 4); /* Z_FINISH == 4 */
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    Bytef outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);
    inflateGetHeader(&inf_strm, &head);
    inflate(&inf_strm, 4); /* Z_FINISH == 4 */
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)inf_strm.total_out);

    // step 4: Validate & Cleanup
    inflateSync(&inf_strm);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    gzclose(gz);
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}