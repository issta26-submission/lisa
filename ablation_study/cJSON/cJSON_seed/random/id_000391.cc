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
//<ID> 391
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
    cJSON *ref = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool added_ref = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));

    // step 3: Configure
    cJSON_AddBoolToObject(root, "active", 1);
    ref = cJSON_CreateString("shared_value");
    added_ref = cJSON_AddItemReferenceToObject(root, "alias", ref);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 5: Validate
    (void)added_ref;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}