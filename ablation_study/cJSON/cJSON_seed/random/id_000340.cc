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
//<ID> 340
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    char *json_unformatted = (char *)0;
    const char *first_str = (const char *)0;
    const char *second_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    arr_ref = cJSON_CreateArrayReference(arr);

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr_ref, 1);
    json_unformatted = cJSON_PrintUnformatted(root);
    first_str = cJSON_GetStringValue(first_elem);
    second_str = cJSON_GetStringValue(second_elem);

    // step 5: Validate
    (void)first_str;
    (void)second_str;
    (void)json_unformatted;

    // step 6: Cleanup
    if (json_unformatted) { cJSON_free((void *)json_unformatted); }
    cJSON_Delete(arr_ref);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}