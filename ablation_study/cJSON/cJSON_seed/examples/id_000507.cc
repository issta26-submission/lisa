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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "cjson_demo";
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it0 = cJSON_CreateString("alpha");
    cJSON *it1 = cJSON_CreateString("beta");
    cJSON *it2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, it0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "app", app_name);
    char info_buf[128];
    memset(info_buf, 0, sizeof(info_buf));
    sprintf(info_buf, "first=%s;size=%d", cJSON_GetStringValue(cJSON_GetArrayItem(items, 0)), cJSON_GetArraySize(items));

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool equal = cJSON_Compare(items, parsed_items, 1);
    char result_buf[64];
    memset(result_buf, 0, sizeof(result_buf));
    sprintf(result_buf, "equal=%d", (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}