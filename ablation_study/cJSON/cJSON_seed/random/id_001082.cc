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
//<ID> 1082
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
    cJSON *label = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    char *printed = (char *)0;
    float numbers[3];
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    cJSON_bool add_ok_null = (cJSON_bool)0;
    cJSON_bool is_null_flag = (cJSON_bool)0;

    // step 2: Setup - prepare numeric data and create JSON items
    numbers[0] = 1.1f;
    numbers[1] = 2.2f;
    numbers[2] = 3.3f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, 3);
    label = cJSON_CreateString("float_values");
    null_item = cJSON_CreateNull();

    // step 3: Configure - attach items to the root object
    add_ok_arr = cJSON_AddItemToObject(root, "data", arr);
    add_ok_label = cJSON_AddItemToObject(root, "label", label);
    add_ok_null = cJSON_AddItemToObject(root, "maybe", null_item);

    // step 4: Operate - fetch items, duplicate array and inspect nullness, serialize
    fetched_arr = cJSON_GetObjectItem(root, "data");
    fetched_label = cJSON_GetObjectItem(root, "label");
    dup_arr = cJSON_Duplicate(fetched_arr, (cJSON_bool)1);
    is_null_flag = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - derive a small validation score using results
    arr_size = cJSON_GetArraySize(fetched_arr);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(null_item != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_label) + (int)(add_ok_null) + (int)(arr_size == 3) + (int)(fetched_label != (cJSON *)0) + (int)(dup_arr != (cJSON *)0) + (int)(is_null_flag);
    (void)validation_score;

    // step 6: Cleanup - delete duplicated and the root which frees attached items
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}