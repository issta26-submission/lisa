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
//<ID> 716
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
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *name = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, flag);
    cJSON_AddItemToArray(arr, cJSON_CreateString("member"));
    cJSON_AddItemToObjectCS(root, "list", arr);
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    cJSON *arr_status = cJSON_CreateString(is_arr ? "array:true" : "array:false");
    cJSON_AddItemToObjectCS(root, "arr_status", arr_status);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_status = cJSON_GetObjectItemCaseSensitive(parsed, "arr_status");
    const char *status_str = cJSON_GetStringValue(parsed_status);
    (void)status_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    // API sequence test completed successfully
    return 66;
}