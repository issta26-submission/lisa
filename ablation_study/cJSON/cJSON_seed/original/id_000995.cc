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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"name\":\"example\",\"meta\":{\"lang\":\"C\"}},\"version\":1}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *name_item = cJSON_GetObjectItem(root_obj, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *meta = cJSON_GetObjectItem(root_obj, "meta");
    cJSON *lang_item = cJSON_GetObjectItem(meta, "lang");
    const char *lang_str = cJSON_GetStringValue(lang_item);
    const char *strings_for_array[2];
    strings_for_array[0] = name_str;
    strings_for_array[1] = lang_str;
    cJSON *string_array = cJSON_CreateStringArray(strings_for_array, 2);
    cJSON_AddItemToObject(root, "summary", string_array);
    cJSON_bool has_version = cJSON_HasObjectItem(root, "version");
    (void)has_version;

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buf, 0, buf_len);
    cJSON_PrintPreallocated(root, print_buf, buf_len, 0);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}