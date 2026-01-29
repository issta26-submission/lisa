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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    const char *langs[] = { "C", "C++", "JSON" };
    cJSON *lang_array = cJSON_CreateStringArray(langs, 3);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON *old_ver = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObject(meta, "languages", lang_array);
    cJSON_AddItemToObject(root, "count", count);
    cJSON_AddItemToObject(meta, "version", old_ver);

    // step 3: Operate & Validate
    cJSON *new_ver = cJSON_CreateNumber(2.0);
    cJSON_ReplaceItemViaPointer(meta, old_ver, new_ver);
    cJSON *ver_ptr = cJSON_GetObjectItem(meta, "version");
    double ver_value = cJSON_GetNumberValue(ver_ptr);
    cJSON *ver_copy = cJSON_CreateNumber(ver_value);
    cJSON_AddItemToObject(root, "version_copy", ver_copy);
    cJSON_bool has_lang = cJSON_HasObjectItem(meta, "languages");
    cJSON *has_lang_num = cJSON_CreateNumber((double)has_lang);
    cJSON_AddItemToObject(root, "has_languages", has_lang_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}