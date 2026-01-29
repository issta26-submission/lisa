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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "numbers";
    const char *raw_key = "embedded_raw";
    const char *bool_key = "flag";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *fetched_raw = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool is_raw_flag = (cJSON_bool)0;
    double fetched_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and an empty array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach the array, create a number and add it to the array, add a raw item and a bool to the object
    cJSON_AddItemToObject(root, array_key, arr);
    num_item = cJSON_CreateNumber(3.1415);
    add_ok_arr = cJSON_AddItemToArray(arr, num_item);
    raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, raw_key, raw_item);
    bool_item = cJSON_AddBoolToObject(root, bool_key, 1);

    // step 4: Operate - retrieve the first array element, read its numeric value and test raw status of the raw item
    first_elem = cJSON_GetArrayItem(arr, 0);
    fetched_num = cJSON_GetNumberValue(first_elem);
    fetched_raw = cJSON_GetObjectItem(root, raw_key);
    is_raw_flag = cJSON_IsRaw(fetched_raw);

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0)
                     + (int)add_ok_arr
                     + (int)(num_item != (cJSON *)0)
                     + (int)(first_elem != (cJSON *)0 && fetched_num == 3.1415)
                     + (int)is_raw_flag
                     + (int)(bool_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}