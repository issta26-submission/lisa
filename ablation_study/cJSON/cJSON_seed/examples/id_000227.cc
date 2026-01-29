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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.234);
    cJSON_AddItemToArray(array, n1);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, s1);
    cJSON *nested_val = cJSON_CreateString("child_value");
    cJSON_AddItemToObject(child, "value", nested_val);

    // step 3: Operate & Validate
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *has_child_flag = cJSON_CreateBool(has_child);
    cJSON_AddItemToObject(root, "has_child", has_child_flag);
    int count = cJSON_GetArraySize(array);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "item_count", count_num);
    char *buffer = (char *)cJSON_malloc((size_t)256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON *dump_str = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "dump", dump_str);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}