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
//<ID> 912
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"config\":{\"values\":[1,2],\"name\":\"original\"}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *cfg = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *new_list = cJSON_AddArrayToObject(cfg, "new_list");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(new_list, n1);
    cJSON_AddItemToArray(new_list, n2);

    // step 3: Operate
    cJSON *detached_name = cJSON_DetachItemFromObject(cfg, "name");
    cJSON *values = cJSON_GetObjectItemCaseSensitive(cfg, "values");
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToArray(new_list, detached_name);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}