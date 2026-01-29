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
//<ID> 1087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const float numbers[3] = {1.5f, 2.5f, 3.5f};
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *fetched_floats = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *fetched_null = (cJSON *)0;
    cJSON_bool null_check = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items (float array, string, null)
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 3);
    label = cJSON_CreateString("sensor");
    null_item = cJSON_CreateNull();

    // step 3: Configure - attach items to the root and create a duplicate of the array
    cJSON_AddItemToObject(root, "values", float_arr);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "missing", null_item);
    dup_arr = cJSON_Duplicate(float_arr, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);

    // step 4: Operate - fetch items back and check nullness
    fetched_floats = cJSON_GetObjectItem(root, "values");
    fetched_label = cJSON_GetObjectItem(root, "label");
    fetched_null = cJSON_GetObjectItem(root, "missing");
    null_check = cJSON_IsNull(fetched_null);

    // step 5: Validate - derive a small score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(dup_arr != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(fetched_floats != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(null_check);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}