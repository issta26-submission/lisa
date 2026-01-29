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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\": [1.5]}";
    const char *numbers_key = "numbers";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double fetched_value = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse initial JSON into root and obtain the array
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, numbers_key);

    // step 3: Configure - create a new number and add it to the parsed array
    new_num = cJSON_CreateNumber(2.75);
    cJSON_AddItemToArray(arr, new_num);

    // step 4: Operate - verify the node is an array and retrieve the newly added item
    is_array = cJSON_IsArray(arr);
    fetched_item = cJSON_GetArrayItem(arr, 1);
    fetched_value = cJSON_GetNumberValue(fetched_item);

    // step 5: Validate - compute a small validation score exercising pointers and values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(new_num != (cJSON *)0) + (int)(is_array) + (int)(fetched_item != (cJSON *)0) + (int)(fetched_value > 2.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}