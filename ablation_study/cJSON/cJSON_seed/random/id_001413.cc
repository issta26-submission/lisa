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
//<ID> 1413
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *count_node = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    int is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with three numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.1);
    num1 = cJSON_CreateNumber(2.2);
    num2 = cJSON_CreateNumber(3.3);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - inspect the array, pick the second element, record the size as a number in root and print JSON
    picked = cJSON_GetArrayItem(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    count_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    is_array = (int)cJSON_IsArray(arr);
    printed = cJSON_Print(root);

    // step 4: Validate - compute a simple validation score from operations and returned pointers
    validation_score = arr_size + (int)(picked != (cJSON *)0) + is_array + (int)(count_node != (cJSON *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup - free printed buffer and delete the whole tree
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)picked;
    return 66;
}