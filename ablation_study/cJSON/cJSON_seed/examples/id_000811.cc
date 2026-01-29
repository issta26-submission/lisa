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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *languages[] = { "C", "C++", "Python" };
    cJSON *lang_array = cJSON_CreateStringArray(languages, 3);
    cJSON_AddItemToObject(root, "languages", lang_array);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *debug_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "debug", debug_flag);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_langs = cJSON_GetObjectItem(parsed, "languages");
    cJSON *second_item = cJSON_GetArrayItem(parsed_langs, 1);
    const char *second_val = cJSON_GetStringValue(second_item);
    cJSON *fav = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(parsed, "favorite", fav);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}