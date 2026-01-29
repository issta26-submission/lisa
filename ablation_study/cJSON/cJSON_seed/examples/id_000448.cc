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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *empty = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", empty);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *author = cJSON_CreateString("dev_team");
    cJSON_AddItemToObject(meta, "author", author);
    cJSON *version = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(meta, "version", version);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_null = cJSON_IsNull(retrieved_flag);
    cJSON *first = cJSON_GetArrayItem(retrieved_values, 0);
    cJSON *second = cJSON_GetArrayItem(retrieved_values, 1);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second) + cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version"));
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "flag_is_null=%d|sum=%.2f", (int)flag_is_null, sum);
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