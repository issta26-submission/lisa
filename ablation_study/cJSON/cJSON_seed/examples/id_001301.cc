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
//<ID> 1301
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
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddFalseToObject(meta, "valid");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *newstr = cJSON_CreateString("gamma");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, newstr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool invalid = cJSON_IsInvalid(first);
    cJSON *status = cJSON_CreateNumber((double)(replaced + invalid));
    cJSON_AddItemToObject(root, "status", status);
    char *printed = cJSON_PrintUnformatted(root);
    size_t len = strlen(printed);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, printed, len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}