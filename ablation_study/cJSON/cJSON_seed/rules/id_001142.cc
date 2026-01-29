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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_name = NULL;
    cJSON *replacement_name = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *first_arr_item = NULL;
    cJSON *first_from_parsed = NULL;
    cJSON *is_string_flag = NULL;
    char *dynamic_name = NULL;
    char *printed = NULL;
    const char *first_value = NULL;
    const char json[] = "{\"name\":\"orig\",\"arr\":[\"one\",\"two\"]}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_name = cJSON_CreateString("orig");
    cJSON_AddItemToObject(root, "name", old_name);
    parsed = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(parsed, "arr");
    first_arr_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first_arr_item);
    first_from_parsed = cJSON_CreateString(first_value);
    cJSON_AddItemToObject(root, "first_from_parsed", first_from_parsed);
    dynamic_name = (char *)cJSON_malloc(6);
    memset(dynamic_name, 0, 6);
    dynamic_name[0] = 'r';
    dynamic_name[1] = 'e';
    dynamic_name[2] = 'p';
    dynamic_name[3] = 'l';
    dynamic_name[4] = '\0';
    replacement_name = cJSON_CreateString(dynamic_name);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_name, replacement_name);
    cJSON_bool name_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "name"));
    is_string_flag = cJSON_CreateBool(name_is_string);
    cJSON_AddItemToObject(root, "name_is_string", is_string_flag);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(dynamic_name);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}