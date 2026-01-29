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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "tags", tags);

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "name", name_item);
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));

    // step 3: Operate and Validate
    cJSON_bool has_info = cJSON_HasObjectItem(root, "info");
    cJSON *got_info = cJSON_GetObjectItemCaseSensitive(root, "info");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(got_info, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_tags = cJSON_GetObjectItemCaseSensitive(root, "tags");
    cJSON_bool tags_are_array = cJSON_IsArray(got_tags);
    cJSON *second_tag = cJSON_GetArrayItem(got_tags, 1);
    const char *second_tag_str = cJSON_GetStringValue(second_tag);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + (int)(has_info ? 1 : 0));
    scratch[2] = (char)(tags_are_array ? '1' : '0');
    scratch[3] = second_tag_str ? second_tag_str[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}