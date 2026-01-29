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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_name = "pi";
    const char *arr_name = "values";
    double pi_val = 3.141592653589793;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_elem = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    char *printed = (char *)0;
    double extracted_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize / Configure JSON structure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(pi_val);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, num_name, num_item);
    cJSON_AddItemToObject(root, arr_name, arr);

    // step 3: Operate on the structure (retrieve items, test types, extract values, print)
    got_item = cJSON_GetObjectItem(root, arr_name);
    is_array = cJSON_IsArray(got_item);
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, num_name));
    printed = cJSON_Print(root);

    // step 4: Simple validation aggregation (no branching)
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)is_array + (int)(printed != (char *)0) + (int)(extracted_number == pi_val);

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}