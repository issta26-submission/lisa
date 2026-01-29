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
//<ID> 1299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s = cJSON_CreateString("hello");
    cJSON *t = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToObject(container, "list", arr);
    cJSON_AddItemToObject(container, "flag", t);
    cJSON_AddItemReferenceToObject(root, "s_ref", s);

    // step 3: Operate & Validate
    cJSON_bool truth = cJSON_IsTrue(t);
    cJSON *dup = cJSON_Duplicate(container, 1);
    cJSON *dup_list = cJSON_GetObjectItem(dup, "list");
    cJSON *dup_first = cJSON_GetArrayItem(dup_list, 0);
    const char *val = cJSON_GetStringValue(dup_first);
    size_t len = strlen(val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, val, len + 1);
    cJSON *num = cJSON_CreateNumber((double)truth);
    cJSON_AddItemToObject(dup, "truth_num", num);

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_Delete(container);

    // API sequence test completed successfully
    return 66;
}