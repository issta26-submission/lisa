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
//<ID> 353
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
    cJSON *n1 = cJSON_CreateNumber(3.1415);
    cJSON *n2 = cJSON_CreateNumber(2.7182);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    int count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    cJSON_AddNumberToObject(meta, "flag_is_bool", (double)flag_is_bool);
    cJSON_AddNumberToObject(meta, "flag_is_true", (double)flag_is_true);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddNumberToObject(meta, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}