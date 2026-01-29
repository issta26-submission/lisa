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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char rawData[128];
    memset(rawData, 'A', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);
    Bytef compBuf[512];
    memset(compBuf, 0, sizeof(compBuf));
    uLongf compLen = (uLongf)sizeof(compBuf);
    compress2(compBuf, &compLen, (const Bytef *)rawData, rawLen, 6);
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&infstrm, &head);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sizeof(outBuf);

    // step 3: Operate
    inflate(&infstrm, Z_FINISH);
    inflateSync(&infstrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)infstrm.total_out);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&infstrm);

    // API sequence test completed successfully
    return 66;
}