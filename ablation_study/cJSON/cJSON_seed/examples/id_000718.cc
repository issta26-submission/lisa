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
//<ID> 718
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
    cJSON *items = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON_AddItemToObjectCS(root, "name", cJSON_CreateString("example_name"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON_bool is_array = cJSON_IsArray(parsed_items);
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *parsed_flag = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(parsed_flag);
    int combined_usage = (int)is_array + (int)flag_is_true + (int)(first_str ? strlen(first_str) : 0);
    (void)combined_usage;
    (void)version;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}