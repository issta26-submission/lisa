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
//<ID> 1418
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
    cJSON *picked = (cJSON *)0;
    char *rendered = (char *)0;
    int arr_size = 0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.72);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - query array size, record it into the root as a number, and pick an element
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    picked = cJSON_GetArrayItem(arr, 0);
    is_array_flag = (int)cJSON_IsArray(arr);

    // step 4: Configure - render the root to a string
    rendered = cJSON_Print(root);

    // step 5: Validate - compute a simple validation score from the operations
    validation_score = arr_size + (int)(picked != (cJSON *)0) + is_array_flag + (int)(rendered != (char *)0);

    // step 6: Cleanup - free printed string and delete the main tree
    if (rendered) {
        cJSON_free(rendered);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)picked;
    return 66;
}