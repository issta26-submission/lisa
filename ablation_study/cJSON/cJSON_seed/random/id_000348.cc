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
//<ID> 348
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
    cJSON *str_item = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    char *printed = (char *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    cJSON_bool added = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "main_array", arr);
    str_item = cJSON_CreateString("hello_reference");
    added = cJSON_AddItemToArray(arr, str_item);
    ref_arr = cJSON_CreateArrayReference(str_item);
    cJSON_AddItemToObject(root, "ref_array", ref_arr);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    (void)added;
    (void)first_str;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}