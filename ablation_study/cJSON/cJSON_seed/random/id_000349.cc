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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr1 = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *arr_dup = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *json_root = (char *)0;
    char *json_elem = (char *)0;
    const char *first_str = (const char *)0;
    double second_num = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr1 = cJSON_CreateArray();
    arr2 = cJSON_CreateArray();

    // step 3: Configure (only add if allocations succeeded)
    if (root && arr1 && arr2) {
        cJSON_AddItemToObject(root, "arrayA", arr1);
        cJSON_AddItemToObject(root, "arrayB", arr2);

        str_item = cJSON_CreateString("hello");
        num_item = cJSON_CreateNumber(123.0);
        if (str_item) cJSON_AddItemToArray(arr1, str_item);
        if (num_item) cJSON_AddItemToArray(arr1, num_item);

        // To avoid creating a circular ownership (which would cause double-free on delete),
        // duplicate arr1 and create a reference to the duplicate. This preserves the
        // logical intent (arrayB contains a reference to an array with the same contents)
        // without introducing double-free on cleanup.
        arr_dup = cJSON_Duplicate(arr1, 1); // recurse = true
        if (arr_dup) {
            arr_ref = cJSON_CreateArrayReference(arr_dup);
            if (arr_ref) {
                cJSON_AddItemToArray(arr2, arr_ref);
            } else {
                // If creating the reference failed, free the duplicate to avoid leak.
                cJSON_Delete(arr_dup);
                arr_dup = (cJSON *)0;
            }
        }
    }

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(arr1, 0);
    json_root = root ? cJSON_PrintUnformatted(root) : (char *)0;
    json_elem = first_elem ? cJSON_PrintUnformatted(first_elem) : (char *)0;

    // step 5: Validate (guarded to avoid dereferencing NULL)
    if (first_elem) {
        first_str = cJSON_GetStringValue(first_elem);
    } else {
        first_str = (const char *)0;
    }
    {
        cJSON *second_elem = cJSON_GetArrayItem(arr1, 1);
        if (second_elem) {
            second_num = cJSON_GetNumberValue(second_elem);
        } else {
            second_num = 0.0;
        }
    }
    (void)first_str;
    (void)second_num;

    // step 6: Cleanup
    if (json_root) {
        cJSON_free(json_root);
        json_root = (char *)0;
    }
    if (json_elem) {
        cJSON_free(json_elem);
        json_elem = (char *)0;
    }
    // Deleting root will also delete arr2 -> arr_ref -> arr_dup (if they were added).
    // arr1 is also a child of root and will be properly freed.
    cJSON_Delete(root);
    root = (cJSON *)0;

    // API sequence test completed successfully
    return 66;
}