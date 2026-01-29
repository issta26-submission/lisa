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
//<ID> 1585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *dup_str = (cJSON *)0;
    cJSON *nested_ptr = (cJSON *)0;
    cJSON *list_ptr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_strval = (const char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_obj_to_root = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, str1);
    dup_str = cJSON_Duplicate(str2, 1);
    added2 = cJSON_AddItemToArray(arr, dup_str);
    added_arr_to_obj = cJSON_AddItemToObjectCS(obj, "list", arr);
    added_obj_to_root = cJSON_AddItemToObjectCS(root, "nested", obj);

    // step 4: Operate
    nested_ptr = cJSON_GetObjectItem(root, "nested");
    list_ptr = cJSON_GetObjectItem(nested_ptr, "list");
    array_size = cJSON_GetArraySize(list_ptr);
    first_elem = cJSON_GetArrayItem(list_ptr, 0);
    first_strval = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)added_arr_to_obj
        + (int)added_obj_to_root
        + (int)(nested_ptr != (cJSON *)0)
        + (int)(list_ptr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_strval != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(str2);
    (void)validation_score;
    (void)first_strval;
    (void)first_elem;
    (void)list_ptr;
    (void)nested_ptr;
    (void)dup_str;
    (void)arr;
    (void)obj;
    (void)str1;
    (void)str2;
    // API sequence test completed successfully
    return 66;
}