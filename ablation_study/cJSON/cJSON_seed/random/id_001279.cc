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
//<ID> 1279
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
    cJSON *parsed_obj = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_result = 0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string containing an array
    root = cJSON_Parse("{\"numbers\": [10, 20, 30]}");

    // step 3: Configure - retrieve the array from the parsed root
    arr = cJSON_GetObjectItem(root, "numbers");

    // step 4: Operate - measure array size, add a boolean to the root, parse an extra object and attach it
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddBoolToObject(root, "active", (cJSON_bool)1);
    parsed_obj = cJSON_Parse("{\"extra\": 99}");
    add_result = cJSON_AddItemToObject(root, "metadata", parsed_obj);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = arr_size + (int)add_result + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(parsed_obj != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}