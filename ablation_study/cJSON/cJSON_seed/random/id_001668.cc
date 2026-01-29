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
//<ID> 1668
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
    cJSON *replacement = (cJSON *)0;
    char *json_unformatted = (char *)0;
    int size_before = 0;
    double replaced_value = 0.0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    replacement = cJSON_CreateNumber(9.0);

    // step 3: Configure
    added_n1 = cJSON_AddItemToArray(arr, n1);
    added_n2 = cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInObject(root, "values", replacement);
    replaced_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "values"));
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(json_unformatted != (char *)0)
               + (int)added_n1
               + (int)added_n2
               + (int)added_arr
               + (int)replaced
               + (int)(size_before == 2)
               + (int)(replaced_value == 9.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)replacement;
    (void)added_n1;
    (void)added_n2;
    (void)added_arr;
    (void)replaced;
    (void)size_before;
    (void)replaced_value;
    // API sequence test completed successfully
    return 66;
}