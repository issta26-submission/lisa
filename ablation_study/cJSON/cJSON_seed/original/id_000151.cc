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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(child, n1);
    cJSON_AddItemToArray(child, n2);
    cJSON_AddItemToObject(root, "values", child);

    // step 2: Configure (use a duplicate instead of a direct reference to avoid double-free issues)
    cJSON *arr_ref = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "values_ref", arr_ref);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    char *buf = (char *)cJSON_malloc(64);
    memset(buf, 0, 64);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_child = cJSON_GetArraySize(child);
    cJSON *first = cJSON_GetArrayItem(child, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *ref_node = cJSON_GetObjectItem(root, "values_ref");
    int size_ref = cJSON_GetArraySize(ref_node);
    buf[0] = printed ? printed[0] : 0;
    int summary = size_child + size_ref + (int)first_val + (int)buf[0] + root->type + flag->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);

    return 66;
}