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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_version[] = "v1.0";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *ver = cJSON_CreateString(initial_version);
    cJSON_AddItemToObject(root, "version", ver);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *ok_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "ok", ok_flag);

    // step 3: Operate
    int orig_count = cJSON_GetArraySize(values);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON_DeleteItemFromArray(values, 1);
    int new_count = cJSON_GetArraySize(values);
    size_t buf_len = 64 + (size_t)new_count;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *ver_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "version"));

    // step 4: Validate & Cleanup
    (void)orig_count;
    (void)first_val;
    (void)new_count;
    (void)ver_str;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}