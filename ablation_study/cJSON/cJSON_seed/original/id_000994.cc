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
//<ID> 994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\": {\"tags\": [\"alpha\", \"beta\"]}, \"name\": \"example\", \"count\": 3}";
    const char * parse_end = NULL;
    cJSON * root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);
    const char * string_list[] = { "one", "two", "three" };
    cJSON * str_arr = cJSON_CreateStringArray(string_list, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "new_array", str_arr);
    cJSON_AddStringToObject(root, "added", "added_value");
    cJSON * sub = cJSON_CreateObject();
    cJSON_AddNumberToObject(sub, "subnum", 5.5);
    cJSON_AddItemToObject(root, "sub", sub);

    // step 3: Operate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddBoolToObject(root, "meta_present", has_meta);
    cJSON * meta = cJSON_GetObjectItem(root, "meta");
    cJSON * tags = cJSON_GetObjectItem(meta, "tags");
    cJSON * first_tag = cJSON_GetArrayItem(tags, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_tag);
    char * first_tag_str = (char *)cJSON_GetStringValue(first_tag);
    cJSON_AddStringToObject(root, "first_tag_copy", first_tag_str);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    char * out = cJSON_PrintUnformatted(root);
    char * print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}