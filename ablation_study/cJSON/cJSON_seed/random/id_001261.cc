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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    float numbers[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    int size_before = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a float array from C floats
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray((const float *)numbers, 4);

    // step 3: Configure - create an array reference to the first element and a false item, then attach items to root
    first_elem = (float_arr != (cJSON *)0) ? float_arr->child : (cJSON *)0;
    ref_arr = cJSON_CreateArrayReference((const cJSON *)first_elem);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(ref_arr, false_item);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "floats_ref", ref_arr);
    cJSON_AddBoolToObject(root, "ok", (cJSON_bool)1);

    // step 4: Operate - measure referenced array size and read a numeric value from the original array
    size_before = cJSON_GetArraySize(ref_arr);
    double first_value = (first_elem != (cJSON *)0) ? cJSON_GetNumberValue(first_elem) : 0.0;

    // step 5: Validate - compute a simple validation score to ensure meaningful data flow
    validation_score = size_before + (int)(root != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(ref_arr != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(first_value > 0.0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}