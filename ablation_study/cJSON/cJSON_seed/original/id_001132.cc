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
//<ID> 1132
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *item0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON *item1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, item1);
    const char *ref_str = "referenced_text";
    cJSON *str_ref = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToObject(root, "ref", str_ref);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", t);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *new_ref = cJSON_CreateStringReference("replaced_value");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, new_ref);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    char *flat = cJSON_PrintUnformatted(dup);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}