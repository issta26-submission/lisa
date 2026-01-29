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
//<ID> 899
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *num_key = "num";
    double number_value = 123.5;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double extracted_number = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize / Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 3: Configure - attach array and a number to the root object
    cJSON_AddItemToObject(root, arr_key, arr);
    num_item = cJSON_CreateNumber(number_value);
    cJSON_AddItemToObject(root, num_key, num_item);

    // step 4: Operate / Validate - retrieve items, check types, extract value, and print
    arr_item = cJSON_GetObjectItem(root, arr_key);
    is_array = cJSON_IsArray(arr_item);
    retrieved_num = cJSON_GetObjectItem(root, num_key);
    extracted_number = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_Print(root);
    validation_score = (int)(root != (cJSON *)0) + (int)is_array + (int)(retrieved_num != (cJSON *)0) + (int)(printed != (char *)0) + (int)(extracted_number == number_value);

    // step 5: Cleanup
    if (printed) { cJSON_free((void *)printed); }  // no branching on logic flow, safe cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}