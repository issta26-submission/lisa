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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"values\": [1, 2, 3]}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    double got_value = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and obtain the array
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, "values");

    // step 3: Configure - verify it's an array and create a new number to append
    is_arr = cJSON_IsArray(arr);
    new_num = cJSON_CreateNumber(4.5);
    add_ok = cJSON_AddItemToArray(arr, new_num);

    // step 4: Operate - fetch the newly appended item and read its numeric value
    fetched_item = cJSON_GetArrayItem(arr, 3);
    got_value = cJSON_GetNumberValue(fetched_item);

    // step 5: Validate - compute a small score exercising the results
    validation_score = (int)(root != (cJSON *)0) + (int)(is_arr) + (int)(new_num != (cJSON *)0) + (int)(add_ok) + (int)(got_value == 4.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}