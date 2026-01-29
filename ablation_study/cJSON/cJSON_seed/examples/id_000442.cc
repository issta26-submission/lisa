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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "fuzzer_example";
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, flag_false);
    cJSON_AddItemToObject(root, "app", cJSON_CreateString(app_name));
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "version", ver);
    cJSON *none_item = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "none", none_item);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_none = cJSON_GetObjectItem(retrieved_meta, "none");
    cJSON_bool is_none = cJSON_IsNull(retrieved_none);
    cJSON *elem0 = cJSON_GetArrayItem(retrieved_values, 0);
    cJSON *elem1 = cJSON_GetArrayItem(retrieved_values, 1);
    double v0 = cJSON_GetNumberValue(elem0);
    double v1 = cJSON_GetNumberValue(elem1);
    double total = v0 + v1;
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "total=%.2f|null=%d", total, (int)is_none);
    cJSON_AddStringToObject(root, "status", status_buf);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}