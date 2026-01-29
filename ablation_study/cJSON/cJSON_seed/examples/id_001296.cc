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
//<ID> 1296
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
    cJSON *str_ref = cJSON_CreateString("shared");
    cJSON *str_elem = cJSON_CreateString("element");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "shared_str", str_ref);
    cJSON_AddItemToArray(arr, cJSON_Duplicate(str_elem, 0));
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate & Validate
    cJSON_bool is_enabled = cJSON_IsTrue(flag);
    (void)is_enabled;
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(dup);
    size_t len = printed ? strlen(printed) : 0;
    char *copy = (char *)cJSON_malloc(len + 1);
    if (printed && copy) memcpy(copy, printed, len + 1);

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    if (copy) cJSON_free(copy);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_Delete(str_elem);

    // API sequence test completed successfully
    return 66;
}