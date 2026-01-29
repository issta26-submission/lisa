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
//<ID> 1295
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *t = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemReferenceToObject(root, "items_ref", dup);
    cJSON_AddItemToObject(root, "flag", t);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    (void)count;
    cJSON_bool flag = cJSON_IsTrue(t);
    (void)flag;
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_s = cJSON_GetStringValue(first);
    size_t len = strlen(first_s);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, first_s, len + 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}