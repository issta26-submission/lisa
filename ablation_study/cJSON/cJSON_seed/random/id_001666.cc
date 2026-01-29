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
//<ID> 1666
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
    cJSON *replacement_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool added_n3 = 0;
    cJSON_bool replaced = 0;
    int array_size = 0;
    double num_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    n3 = cJSON_CreateNumber(3.25);

    // step 3: Configure
    added_n1 = cJSON_AddItemToArray(arr, n1);
    added_n2 = cJSON_AddItemToArray(arr, n2);
    added_n3 = cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    replacement_num = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInObject(root, "values", replacement_num);
    printed = cJSON_PrintUnformatted(root);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "values"));

    // step 5: Validate
    validation = (int)(array_size == 3) + (int)added_n1 + (int)added_n2 + (int)added_n3 + (int)replaced + (int)(printed != (char *)0) + (int)(num_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)replacement_num;
    (void)added_n1;
    (void)added_n2;
    (void)added_n3;
    (void)replaced;
    (void)array_size;
    (void)num_value;
    // API sequence test completed successfully
    return 66;
}