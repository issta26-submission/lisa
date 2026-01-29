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
//<ID> 1664
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
    cJSON *n3 = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    cJSON *tmp_item = (cJSON *)0;
    char *json_unformatted = (char *)0;
    int arr_size = 0;
    double extracted_value = 0.0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    n3 = cJSON_CreateNumber(3.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "replace_me", 7.0);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    newnum = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInObject(root, "replace_me", newnum);
    json_unformatted = cJSON_PrintUnformatted(root);
    tmp_item = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(tmp_item);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)replaced + (int)(json_unformatted != (char *)0) + (int)(extracted_value == 2.5);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)newnum;
    (void)tmp_item;
    (void)arr_size;
    (void)extracted_value;
    (void)replaced;
    // API sequence test completed successfully
    return 66;
}