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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_name = "items";
    const char *name_key = "name";
    const char *name_val = "example";
    char json_buf[] = " { \"k\" : \"v\" } ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_to_object = (cJSON_bool)0;
    cJSON_bool added_to_array1 = (cJSON_bool)0;
    cJSON_bool added_to_array2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child = cJSON_CreateString("child_value");

    // step 3: Configure
    arr_ref = cJSON_CreateArrayReference(child);
    added_to_array1 = cJSON_AddItemToArray(arr, child);
    added_to_array2 = cJSON_AddItemToArray(arr, arr_ref);
    added_to_object = cJSON_AddItemToObject(root, array_name, arr);

    // step 4: Operate
    str_item = cJSON_AddStringToObject(root, name_key, name_val);
    cJSON_Minify(json_buf);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)added_to_object + (int)added_to_array1 + (int)added_to_array2 + (int)(json_buf[0] == '{');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}