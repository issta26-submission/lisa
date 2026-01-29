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
//<ID> 1416
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("alpha");
    elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - pick the first element from the array
    picked = cJSON_GetArrayItem(arr, 0);

    // step 4: Operate - get array size, note if it's an array, store size as a number in root and print
    arr_size = cJSON_GetArraySize(arr);
    is_array_flag = cJSON_IsArray(arr);
    num_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a simple validation score from produced values
    validation_score = arr_size + (int)is_array_flag + (int)(num_node != (cJSON *)0) + (int)(printed != (char *)0) + (int)(picked != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    if (printed != (char *)0) { cJSON_free((void *)printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem0;
    (void)elem1;
    (void)picked;
    (void)arr_size;
    return 66;
}