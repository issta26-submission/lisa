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
//<ID> 1665
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    char *unformatted = (char *)0;
    cJSON_bool replaced = 0;
    int arr_size = 0;
    double first_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "pi", 1.23);

    // step 3: Operate
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first);
    replacement = cJSON_CreateNumber(6.28);
    replaced = cJSON_ReplaceItemInObject(root, "pi", replacement);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation = (int)(unformatted != (char *)0) + (int)(arr_size == 2) + (int)replaced + (int)(first_val == 10.0);
    (void)validation;

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)first;
    (void)replacement;
    (void)replaced;
    (void)arr_size;
    (void)first_val;

    // step 6: Completed
    // API sequence test completed successfully
    return 66;
}