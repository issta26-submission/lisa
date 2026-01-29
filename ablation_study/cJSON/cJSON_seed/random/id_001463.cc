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
//<ID> 1463
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
    cJSON *n2 = (cJSON *)0;
    cJSON *label_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON *picked2 = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    double avg_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create object, array and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.0);
    n1 = cJSON_CreateNumber(4.5);
    n2 = cJSON_CreateNumber(2.5);

    // step 3: Configure - populate array and attach to root, add a descriptive string
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    added = cJSON_AddItemToObject(root, "values", arr);
    label_node = cJSON_AddStringToObject(root, "label", "three_numbers");

    // step 4: Operate - read array size and compute sum/average using number accessors, then print
    arr_size = cJSON_GetArraySize(arr);
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    picked2 = cJSON_GetArrayItem(arr, 2);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1) + cJSON_GetNumberValue(picked2);
    avg_val = sum_val / (double)arr_size;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum_val));
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(avg_val));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - derive a small validation score and cleanup resources
    validation_score = (int)(printed != (char *)0) + (int)(arr_size == 3) + (int)(label_node != (cJSON *)0) + (int)(added != 0);
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)picked0;
    (void)picked1;
    (void)picked2;
    (void)sum_val;
    (void)avg_val;
    (void)arr_size;
    (void)added;

    // API sequence test completed successfully
    return 66;
}