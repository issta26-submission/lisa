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
//<ID> 346
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
    cJSON *inner = (cJSON *)0;
    cJSON *inner_str = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *ref_arr = (cJSON *)0;
    cJSON *ref_first = (cJSON *)0;
    char *out_root = (char *)0;
    char *out_arr = (char *)0;
    char *out_ref = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    str_item = cJSON_CreateString("alpha");
    inner = cJSON_CreateArray();
    inner_str = cJSON_CreateString("nested");
    cJSON_AddItemToArray(inner, inner_str);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, inner);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 1);
    ref_arr = cJSON_CreateArrayReference(got_item);
    cJSON_AddItemToObject(root, "ref", ref_arr);
    out_root = cJSON_PrintUnformatted(root);
    out_arr = cJSON_PrintUnformatted(arr);
    out_ref = cJSON_PrintUnformatted(ref_arr);

    // step 5: Validate
    ref_first = cJSON_GetArrayItem(ref_arr, 0);
    (void)got_item;
    (void)ref_first;
    (void)out_root;
    (void)out_arr;
    (void)out_ref;

    // step 6: Cleanup
    cJSON_free(out_ref);
    cJSON_free(out_arr);
    cJSON_free(out_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}