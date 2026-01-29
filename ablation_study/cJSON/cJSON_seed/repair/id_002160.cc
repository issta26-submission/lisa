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
//<ID> 2160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag_false);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *value = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "value", value);

    // step 2: Configure
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));
    cJSON *list_copy = cJSON_Duplicate(list, 1);
    cJSON_AddItemToObject(root, "list_copy", list_copy);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool flag_is_bool = cJSON_IsBool(got_flag);
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON *flag_check = cJSON_CreateBool(flag_is_bool);
    cJSON_AddItemToObject(root, "flag_is_bool", flag_check);
    cJSON *name_check = cJSON_CreateBool(name_is_string);
    cJSON_AddItemToObject(root, "name_is_string", name_check);
    char *name_val = cJSON_GetStringValue(got_name);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (name_val && name_val[0]) ? name_val[0] : '\0';
    buffer[1] = (json && json[0]) ? json[0] : '\0';
    buffer[2] = (json && json[1]) ? json[1] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}