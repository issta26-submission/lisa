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
//<ID> 1211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool added1 = cJSON_AddItemToArray(arr, s1);
    cJSON_bool added2 = cJSON_AddItemToArray(arr, s2);
    cJSON_bool added3 = cJSON_AddItemToArray(arr, null_item);
    cJSON_bool added4 = cJSON_AddItemToArray(arr, true_item);
    cJSON *named_null = cJSON_AddNullToObject(root, "explicit_null");

    // step 3: Operate
    char *set_res = cJSON_SetValuestring(s2, "beta_modified");
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 4: Validate & Cleanup
    int size_orig = cJSON_GetArraySize(arr);
    cJSON *dup_items = cJSON_GetObjectItem(dup, "items");
    int size_dup = cJSON_GetArraySize(dup_items);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    (void)added1; (void)added2; (void)added3; (void)added4;
    (void)named_null; (void)set_res; (void)printed; (void)dup;
    (void)size_orig; (void)size_dup; (void)prealloc_ok;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}