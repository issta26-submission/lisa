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
//<ID> 1470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("test-name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 42.5);
    cJSON_AddItemToObject(meta, "value", num);
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON_AddItemToArray(items, cJSON_CreateString("leaf"));

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int size = cJSON_GetArraySize(got_items);
    cJSON *third = cJSON_GetArrayItem(got_items, 2);
    char *third_str = cJSON_GetStringValue(third);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = name_str ? name_str[0] : 'n';
    scratch[1] = third_str ? third_str[0] : 'n';
    scratch[2] = (char)('0' + (size % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}