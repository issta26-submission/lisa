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
//<ID> 1474
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *initial_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    cJSON_bool added_arr = 0;

    // step 2: Initialize - create root object, an array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(2.0);
    n1 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach array to root and add an initial numeric property
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    initial_node = cJSON_AddNumberToObject(root, "initial", 10.0);

    // step 4: Operate - inspect array elements and compute sum, then add sum to root
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1);
    sum_node = cJSON_AddNumberToObject(root, "sum", sum_val);

    // step 5: Validate - produce a printed representation to exercise print path
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)added_arr;
    (void)initial_node;
    (void)sum_node;
    (void)printed;

    // step 6: Cleanup - remove the array property explicitly and delete the remaining tree
    cJSON_DeleteItemFromObject(root, "numbers");
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)elem0;
    (void)elem1;
    (void)n0;
    (void)n1;
    (void)sum_val;
    // API sequence test completed successfully
    return 66;
}