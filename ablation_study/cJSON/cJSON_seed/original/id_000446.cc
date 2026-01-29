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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"demo\",\"value\":42}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "original", cJSON_CreateString(name_str));
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON *scaled = cJSON_CreateNumber(value_num * 2.0);
    cJSON_AddItemToObject(root, "scaled", scaled);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}