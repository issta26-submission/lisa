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
//<ID> 1227
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
    const char *ver_text = cJSON_Version();
    cJSON *version_node = cJSON_CreateString(ver_text);
    cJSON_AddItemToObject(root, "version", version_node);
    const char *raw_json = " { \"a\": 1 , \"b\" : 2 } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *buf = (char *)cJSON_malloc(raw_len);
    memset(buf, 0, raw_len);
    memcpy(buf, raw_json, raw_len);
    cJSON_Minify(buf);
    cJSON *minified_node = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "minified", minified_node);
    cJSON_bool arr_check = cJSON_IsArray(arr);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    const char *retrieved_version = cJSON_GetStringValue(cJSON_GetObjectItem(root, "version"));

    // step 4: Validate & Cleanup
    (void)arr_check;
    (void)arr_size;
    (void)retrieved_version;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}