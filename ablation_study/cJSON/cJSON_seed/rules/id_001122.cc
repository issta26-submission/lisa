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
//<ID> 1122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *list = NULL;
    cJSON *first_elem = NULL;
    cJSON *false_item = NULL;
    cJSON *replacement = NULL;
    cJSON *string_array = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    void *buf = NULL;
    const char *json = "{\"ok\":false,\"list\":[\"x\"]}";
    const char *strings[] = {"alpha", "beta", "gamma"};
    size_t json_len = 25;
    int arr_size = 0;
    const char *first_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "created_list", string_array);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    buf = cJSON_malloc(64);
    memset(buf, 0, 64);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_flag");
    cJSON_ReplaceItemViaPointer(root, false_item, replacement);
    list = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(list);
    first_elem = cJSON_GetArrayItem(list, 0);
    first_val = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)parse_end;
    (void)arr_size;
    (void)first_val;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}