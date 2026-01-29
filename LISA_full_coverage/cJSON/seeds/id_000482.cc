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
//<ID> 482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    const char json[] = "{\"name\":\"example\",\"flag\":false,\"count\":3}";
    cJSON *root = cJSON_Parse(json);

    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    const char *name_str = cJSON_GetStringValue(name_item);
    size_t name_len = strlen(name_str);
    char *name_copy = (char *)cJSON_malloc(name_len + 1);
    memset(name_copy, 0, name_len + 1);
    memcpy(name_copy, name_str, name_len);
    cJSON *new_flag = cJSON_CreateBool(name_is_string);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "flag", new_flag);

    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_string_after = cJSON_IsString(retrieved_flag);
    (void)flag_is_string_after;
    (void)replaced;

    cJSON_free(snapshot);
    cJSON_free(name_copy);
    cJSON_Delete(root);
    return 66;
}