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
//<ID> 1588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *dup_str2 = (cJSON *)0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON *retrieved_arr = (cJSON *)0;
    int array_size = 0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    dup_str2 = cJSON_Duplicate(str2, 1);
    added_dup = cJSON_AddItemToArray(arr, dup_str2);
    added_arr_to_root = cJSON_AddItemToObjectCS(root, "list", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_str1
        + (int)added_dup
        + (int)added_arr_to_root
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)str2;
    (void)first_str;
    (void)first_elem;
    (void)retrieved_arr;
    (void)arr;
    (void)str1;
    (void)dup_str2;
    // API sequence test completed successfully
    return 66;
}