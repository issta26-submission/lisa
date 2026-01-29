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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *ref_src = NULL;
    cJSON *bool_item = NULL;
    int arr_size = 0;
    cJSON_bool ref_added = 0;
    cJSON_bool bool_added = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    ref_src = cJSON_CreateString("shared");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "source", ref_src);

    // cJSON_AddBoolToObject returns a cJSON*, not a cJSON_bool
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    bool_added = (bool_item != NULL);

    ref_added = cJSON_AddItemReferenceToObject(root, "alias", ref_src);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)arr_size;
    (void)ref_added;
    (void)bool_added;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}