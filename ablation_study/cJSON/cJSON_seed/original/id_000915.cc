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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":10,\"b\":20,\"c\":\"hello\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON *extracted = cJSON_AddArrayToObject(root, "extracted");
    cJSON *item_a = cJSON_GetObjectItemCaseSensitive(root, "a");
    double a_val = cJSON_GetNumberValue(item_a);
    cJSON *double_a = cJSON_CreateNumber(a_val * 2.0);

    // step 3: Operate
    cJSON *item_b = cJSON_DetachItemFromObject(root, "b");
    cJSON_AddItemToArray(extracted, item_b);
    cJSON_AddItemToArray(extracted, double_a);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(128);
    memset(buf, 0, 128);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 128, 1);
    (void)pre_ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}