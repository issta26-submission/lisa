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
//<ID> 1459
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
    cJSON *old_status = (cJSON *)0;
    cJSON *status_ptr = (cJSON *)0;
    cJSON *new_status = (cJSON *)0;
    cJSON *values_ptr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(7.0);
    num1 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach array to root and add a status string that will be replaced
    added = cJSON_AddItemToObject(root, "values", arr);
    old_status = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "status", old_status);
    status_ptr = cJSON_GetObjectItem(root, "status");
    new_status = cJSON_CreateString("updated");
    replaced = cJSON_ReplaceItemViaPointer(root, status_ptr, new_status);

    // step 4: Operate - retrieve the values array via object lookup, compute sum, and add sum to root
    values_ptr = cJSON_GetObjectItem(root, "values");
    first_elem = cJSON_GetArrayItem(values_ptr, 0);
    second_elem = cJSON_GetArrayItem(values_ptr, 1);
    sum_val = cJSON_GetNumberValue(first_elem) + cJSON_GetNumberValue(second_elem);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - print unformatted JSON and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(added != 0) + (int)(replaced != 0) + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)first_elem;
    (void)second_elem;
    (void)values_ptr;
    (void)old_status;
    (void)status_ptr;
    (void)new_status;
    (void)num0;
    (void)num1;
    (void)added;
    (void)replaced;
    // API sequence test completed successfully
    return 66;
}