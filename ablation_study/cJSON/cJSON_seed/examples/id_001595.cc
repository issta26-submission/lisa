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
//<ID> 1595
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, s3);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ready", flag);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "items_copy", dup);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_dup_str = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_dup_str);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(array);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "items_size", size_num);
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON *was_false_node = cJSON_CreateBool(was_false);
    cJSON_AddItemToObject(root, "was_false", was_false_node);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}