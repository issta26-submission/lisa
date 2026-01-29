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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *strref = cJSON_CreateStringReference("initial_ref");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, strref);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *name_ref = cJSON_CreateStringReference("root_name");
    cJSON_AddItemToObject(root, "name", name_ref);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *arr_from_root = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(arr_from_root, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr_from_root, 0, replacement_true);

    // step 3: Operate
    cJSON *new_first = cJSON_GetArrayItem(arr_from_root, 0);
    cJSON_bool is_true = cJSON_IsTrue(new_first);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    char *flat = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);

    // step 4: Validate & Cleanup
    (void)first_val;
    (void)is_true;
    (void)name_val;
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}