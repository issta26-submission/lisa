#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 900
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw = "{\"flags\": [true, false, true], \"count\": 3}";
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    cJSON_bool is2 = 0;
    int size = 0;
    size_t buf_len = 8;
    char *buf = NULL;
    memset((void *)&buf_len, 0, sizeof(buf_len)); /* ensure buf_len variable is touched (will be overwritten next) */

    // step 2: Setup / Configure
    root = cJSON_Parse(raw);
    flags = cJSON_GetObjectItem(root, "flags");
    item0 = cJSON_GetArrayItem(flags, 0);
    item1 = cJSON_GetArrayItem(flags, 1);
    item2 = cJSON_GetArrayItem(flags, 2);

    // step 3: Operate / Validate
    is0 = cJSON_IsTrue(item0);
    is1 = cJSON_IsTrue(item1);
    is2 = cJSON_IsTrue(item2);
    size = cJSON_GetArraySize(flags);
    buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    buf[0] = (char)('0' + (is0 ? 1 : 0));
    buf[1] = (char)('0' + (is1 ? 1 : 0));
    buf[2] = (char)('0' + (is2 ? 1 : 0));
    buf[3] = (char)('0' + (size % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}