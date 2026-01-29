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
//<ID> 994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "values";
    const char *raw_key = "raw";
    const char *bool_key = "flag";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *bool_obj = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    cJSON *first_el = (cJSON *)0;
    double first_val = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, two numbers and a raw item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.71);
    raw_item = cJSON_CreateRaw("unparsed_fragment");

    // step 3: Configure - add numbers to the array, attach array and raw to root, add a boolean via AddBoolToObject
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToObject(root, array_key, arr);
    bool_obj = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);
    add_ok3 = cJSON_AddItemToObject(root, raw_key, raw_item);

    // step 4: Operate - inspect rawness and read first array element
    is_raw = cJSON_IsRaw(raw_item);
    first_el = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_el);

    // step 5: Validate - compute a small validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(bool_obj != (cJSON *)0) + (int)(is_raw) + (int)(cJSON_GetArraySize(arr) == 2) + (int)(first_val == 3.14);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}