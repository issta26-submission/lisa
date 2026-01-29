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
//<ID> 1436
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
    cJSON *picked = (cJSON *)0;
    cJSON *summary = (cJSON *)0;
    cJSON *new_summary = (cJSON *)0;
    char *printed = (char *)0;
    double picked_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and populate an array with numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.25);
    n1 = cJSON_CreateNumber(4.75);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 3: Configure - attach the array to the root and add a placeholder summary field
    cJSON_AddItemToObject(root, "numbers", arr);
    summary = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "picked_sum", summary);

    // step 4: Operate - pick the second array item, read its numeric value, and replace the summary with a computed value
    picked = cJSON_GetArrayItem(arr, 1);
    picked_val = cJSON_GetNumberValue((const cJSON *const)picked);
    new_summary = cJSON_CreateNumber(picked_val + 1.0);
    cJSON_ReplaceItemInObject(root, "picked_sum", new_summary);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - derive a small validation score from the picked value and the printed output presence
    validation_score = (int)picked_val + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)n0;
    (void)n1;
    (void)summary;
    (void)new_summary;
    (void)picked;
    // API sequence test completed successfully
    return 66;
}