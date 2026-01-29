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
//<ID> 410
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
    cJSON *replacement_holder = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    n2 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "note", "before-replace");

    // step 4: Operate
    cJSON_ReplaceItemInArray(arr, 1, n2);
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(got_item);

    // step 5: Validate
    replacement_holder = cJSON_CreateNumber(extracted_value);
    cJSON_AddItemToObject(root, "replaced_value", replacement_holder);
    cJSON_AddStringToObject(root, "note_after", "replace-done");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}