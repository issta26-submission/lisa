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
//<ID> 1727
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *first_value = (char *)0;
    char *json_unformatted = (char *)0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("one");
    str2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first_item);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(first_value != (char *)0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str1;
    (void)str2;
    (void)first_item;
    (void)first_value;
    (void)arr_size;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}