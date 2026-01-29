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
//<ID> 1481
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON *raw_node = (cJSON *)0;
    const char *label = (const char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool raw_check = 0;

    // step 2: Initialize - prepare a C float array and create a cJSON float array from it
    float values[3];
    values[0] = 1.1f;
    values[1] = 2.2f;
    values[2] = 3.3f;
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(values, 3);

    // step 3: Configure - attach the float array and additional descriptive items to the root object
    cJSON_AddItemToObject(root, "floats", float_arr);
    str_node = cJSON_AddStringToObject(root, "label", "float_list");
    raw_node = cJSON_AddRawToObject(root, "raw_info", "{\"type\":\"raw\"}");

    // step 4: Operate - inspect the array size and fetch specific elements; examine raw and string nodes
    arr_size = cJSON_GetArraySize(float_arr);
    elem0 = cJSON_GetArrayItem(float_arr, 0);
    elem2 = cJSON_GetArrayItem(float_arr, 2);
    raw_check = cJSON_IsRaw(raw_node);
    label = cJSON_GetStringValue(str_node);

    // step 5: Validate - compute a small validation score from gathered information
    validation_score = arr_size + (int)raw_check + (int)(label != (const char *)0) + (int)(elem0 != (cJSON *)0) + (int)(elem2 != (cJSON *)0);

    // step 6: Cleanup - delete the constructed JSON tree and silence unused-variable warnings
    cJSON_Delete(root);
    (void)validation_score;
    (void)label;
    (void)elem0;
    (void)elem2;
    (void)arr_size;
    (void)raw_check;
    // API sequence test completed successfully
    return 66;
}