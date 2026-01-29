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
//<ID> 944
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "child", child);

    // step 2: Configure
    cJSON_AddStringToObject(child, "name", "alpha");
    cJSON_AddNumberToObject(child, "value", 10.5);
    cJSON_AddTrueToObject(config, "enabled");
    cJSON_AddStringToObject(root, "version", "2.0");

    // step 3: Operate and Validate
    cJSON_InsertItemInArray(items, 0, cJSON_CreateString("first"));
    cJSON_InsertItemInArray(items, 1, cJSON_CreateNumber(42.0));
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_bool is_obj = cJSON_IsObject(child);
    cJSON_bool eq = cJSON_Compare(child, dup_child, 1);
    cJSON *is_obj_num = cJSON_CreateNumber((double)is_obj);
    cJSON_AddItemToObject(root, "child_is_object", is_obj_num);
    cJSON *eq_num = cJSON_CreateNumber((double)eq);
    cJSON_AddItemToObject(root, "child_equals_dup", eq_num);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(dup_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}