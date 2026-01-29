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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_name = cJSON_CreateString("cJSON-Test");
    cJSON_AddItemToObject(meta, "name", meta_name);
    cJSON *meta_ver = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(meta, "ver", meta_ver);
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *data_value = cJSON_CreateString("initial");
    cJSON_AddItemToObject(data, "value", data_value);

    // step 2: Configure
    cJSON *replacement_value = cJSON_CreateString("replaced");
    cJSON *old_value_ptr = cJSON_GetObjectItem(data, "value");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(data, old_value_ptr, replacement_value);
    (void)replaced;

    // step 3: Operate & Validate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_name = cJSON_GetObjectItem(retrieved_meta, "name");
    const char *name_str = cJSON_GetStringValue(retrieved_name);
    cJSON *retrieved_data_value = cJSON_GetObjectItem(data, "value");
    const char *value_str = cJSON_GetStringValue(retrieved_data_value);
    double ver = cJSON_GetNumberValue(cJSON_GetObjectItem(retrieved_meta, "ver"));
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|value=%s|ver=%.2f", name_str ? name_str : "null", value_str ? value_str : "null", ver);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}