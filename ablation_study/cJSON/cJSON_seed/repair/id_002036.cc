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
//<ID> 2036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);
    cJSON_AddStringToObject(root, "replaced", "old_placeholder");

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    const char raw_json[] = "{ \"replacement\": \"new_value\" }";
    const int raw_buflen = (int)sizeof(raw_json);
    char *minify_buf = (char *)cJSON_malloc((size_t)raw_buflen);
    memset(minify_buf, 0, (size_t)raw_buflen);
    memcpy(minify_buf, raw_json, (size_t)raw_buflen);
    cJSON_Minify(minify_buf);
    cJSON *parsed = cJSON_Parse(minify_buf);

    // step 3: Operate and Validate
    cJSON *parsed_item = cJSON_GetObjectItem(parsed, "replacement");
    cJSON *replacement_dup = cJSON_Duplicate(parsed_item, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replaced", replacement_dup);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    const int out_len = 32;
    char *outbuf = (char *)cJSON_malloc((size_t)out_len);
    memset(outbuf, 0, (size_t)out_len);
    outbuf[0] = first_str[0];
    outbuf[1] = '\0';
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(minify_buf);
    cJSON_free(outbuf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}