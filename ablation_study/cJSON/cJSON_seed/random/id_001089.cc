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
//<ID> 1089
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
    cJSON *dup_root = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    char *printed = (char *)0;
    const float numbers[3] = { 1.1f, 2.2f, 3.3f };
    int arr_count = 3;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool is_null_arr = (cJSON_bool)0;
    cJSON_bool is_null_label = (cJSON_bool)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;

    // step 2: Setup - create root object, a float array and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, arr_count);
    label = cJSON_CreateString("sensor_label");

    // step 3: Configure - attach array and string to the root object
    add_ok_arr = cJSON_AddItemToObject(root, "readings", arr);
    add_ok_label = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate - duplicate the root and fetch items from the duplicate
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);
    fetched_arr = cJSON_GetObjectItem(dup_root, "readings");
    fetched_label = cJSON_GetObjectItem(dup_root, "label");
    is_null_arr = cJSON_IsNull(fetched_arr);
    is_null_label = cJSON_IsNull(fetched_label);

    // step 5: Validate - inspect array size, serialize and compute a small score
    arr_size = cJSON_GetArraySize(fetched_arr);
    printed = cJSON_PrintUnformatted(dup_root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(arr_size == arr_count) + (int)(add_ok_arr) + (int)(add_ok_label) + (int)(is_null_arr == (cJSON_bool)0) + (int)(is_null_label == (cJSON_bool)0);
    (void)validation_score;

    // step 6: Cleanup - delete duplicates and original structures
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}