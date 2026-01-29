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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *shared = NULL;
    cJSON *ref_shared = NULL;
    cJSON *arr_child = NULL;
    cJSON_bool add_res_obj = 0;
    cJSON_bool add_res_ref = 0;
    char *got_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_res_obj = cJSON_AddItemToObjectCS(root, "items", arr);
    shared = cJSON_CreateString("shared_value");
    add_res_obj = cJSON_AddItemToObjectCS(root, "shared", shared);

    // step 3: Operate / Validate
    ref_shared = cJSON_CreateObjectReference(shared);
    add_res_ref = cJSON_AddItemReferenceToArray(arr, ref_shared);
    arr_size = cJSON_GetArraySize(arr);
    arr_child = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(arr_child);

    // step 4: Cleanup
    (void)got_str;
    (void)add_res_obj;
    (void)add_res_ref;
    (void)arr_size;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}