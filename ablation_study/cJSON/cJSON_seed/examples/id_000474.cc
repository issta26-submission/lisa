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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw\"}");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *t = cJSON_CreateTrue();
    cJSON *f = cJSON_CreateFalse();
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate & Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    int size_after = cJSON_GetArraySize(arr);
    cJSON *maybe_true = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_true = cJSON_IsTrue(maybe_true);
    cJSON *maybe_false = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_false = cJSON_IsFalse(maybe_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)size_before;
    (void)size_after;
    (void)is_true;
    (void)is_false;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}