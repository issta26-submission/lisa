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
//<ID> 1511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *nameNode = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(info, "name", nameNode);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *item1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, item1);
    cJSON *item2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, item2);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(nameNode, "beta");

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(nameNode);
    double version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    int arr_size = cJSON_GetArraySize(items);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_val[0];
    scratch[1] = (char)('0' + ((int)version_val % 10));
    scratch[2] = (char)('0' + (int)arr_size);
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(flag));
    scratch[4] = set_ret[0];

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}