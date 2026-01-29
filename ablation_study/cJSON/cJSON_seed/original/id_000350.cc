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
//<ID> 350
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_bool add_ok1 = cJSON_AddItemToArray(values, n1);
    cJSON_bool add_ok2 = cJSON_AddItemToArray(values, n2);
    cJSON_bool add_ok3 = cJSON_AddItemToArray(values, btrue);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *total_node = cJSON_AddNumberToObject(root, "total", 4.0);
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(values));
    cJSON_AddNumberToObject(meta, "add_ok1", (double)add_ok1);
    cJSON_AddNumberToObject(meta, "add_ok2", (double)add_ok2);
    cJSON_AddNumberToObject(meta, "add_ok3", (double)add_ok3);

    // step 3: Operate
    cJSON_bool is_bool = cJSON_IsBool(btrue);
    cJSON_bool is_true = cJSON_IsTrue(btrue);
    cJSON_AddNumberToObject(meta, "is_bool", (double)is_bool);
    cJSON_AddNumberToObject(meta, "is_true", (double)is_true);
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