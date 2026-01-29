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
//<ID> 1304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"ver\":\"1.0\"},\"arr\":[\"one\",\"two\"]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    const char *lib_ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(lib_ver);
    cJSON_AddItemToObject(root, "lib_version", ver_item);
    cJSON *data_array = cJSON_CreateArray();
    cJSON *str_alpha = cJSON_CreateString("alpha");
    cJSON *strref_beta = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(data_array, str_alpha);
    cJSON_AddItemToArray(data_array, strref_beta);
    cJSON_AddItemToObject(root, "data", data_array);

    // step 3: Operate
    cJSON *parsed_arr = cJSON_GetObjectItemCaseSensitive(parsed, "arr");
    cJSON *second_elem = cJSON_GetArrayItem(parsed_arr, 1);
    const char *second_value = cJSON_GetStringValue(second_elem);
    cJSON *found_copy = cJSON_CreateString(second_value);
    cJSON_AddItemToObject(root, "found", found_copy);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}