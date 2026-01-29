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
//<ID> 1412
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
    int is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("one");
    elem1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - inspect the array and pick the first element
    arr_size = cJSON_GetArraySize(arr);
    is_array = (int)cJSON_IsArray((const cJSON *const)arr);
    picked = cJSON_GetArrayItem(arr, 0);

    // step 4: Configure - record the array size as a numeric field in the root object
    num_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 5: Validate - create a printed representation and compute a tiny validation score
    printed = cJSON_Print((const cJSON *)root);
    validation_score = arr_size + is_array + (int)(picked != (cJSON *)0) + (int)(num_node != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the whole JSON tree
    if (printed != (char *)0) { cJSON_free((void *)printed); }
    cJSON_Delete(root);

    (void)validation_score;
    (void)elem0;
    (void)elem1;
    (void)picked;
    // API sequence test completed successfully
    return 66;
}