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
//<ID> 712
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
    cJSON *data = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("hello");
    cJSON *item2 = cJSON_CreateString("world");
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(data, item1);
    cJSON_AddItemToArray(data, item2);
    cJSON_AddItemToObjectCS(root, "data", data);
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON_AddItemToObjectCS(root, "label", cJSON_CreateString("example_label"));

    // step 2: Configure
    cJSON *data_copy = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObjectCS(root, "data_copy", data_copy);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_data);
    cJSON *first_item = cJSON_GetArrayItem(parsed_data, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON *parsed_flag = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON_bool parsed_flag_true = cJSON_IsTrue(parsed_flag);
    (void)version;
    (void)parsed_is_array;
    (void)first_value;
    (void)parsed_flag_true;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}