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
//<ID> 1011
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"metrics\": [10, 20, 30], \"meta\": {\"name\":\"sensor-x\",\"active\":true}, \"value\": 42.5}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *detached_metric = cJSON_DetachItemFromArray(metrics, 1);
    cJSON *extracted = cJSON_CreateObject();
    cJSON_AddItemToObject(extracted, "detached_metric", detached_metric);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate & Validate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON *computed = cJSON_CreateNumber(value_num * 2.0);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON_AddItemToObject(root, "extracted", extracted);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}