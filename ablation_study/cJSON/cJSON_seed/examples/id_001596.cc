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
//<ID> 1596
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, s0);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    int initial_size = cJSON_GetArraySize(array);
    cJSON *initial_size_num = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToObject(root, "items_count", initial_size_num);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate & Validate
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON *flag_as_number = cJSON_CreateNumber((double)is_false);
    cJSON_AddItemToObject(root, "active_numeric", flag_as_number);
    cJSON *copy = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "items_copy", copy);
    int copy_size = cJSON_GetArraySize(copy);
    cJSON *copy_size_num = cJSON_CreateNumber((double)copy_size);
    cJSON_AddItemToObject(root, "items_copy_count", copy_size_num);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}