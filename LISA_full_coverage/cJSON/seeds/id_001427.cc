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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[10,20,30],\"meta\":null}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *parsed_first = cJSON_GetArrayItem(parsed_items, 0);
    cJSON_bool parsed_first_invalid = cJSON_IsInvalid(parsed_first);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);

    // step 2: Configure
    cJSON *arr_item2 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool arr_item2_invalid = cJSON_IsInvalid(arr_item2);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate & Validate
    cJSON *replaced = cJSON_GetArrayItem(arr, 1);
    cJSON_bool replaced_invalid = cJSON_IsInvalid(replaced);
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_items, 1);
    double val_parsed_second = cJSON_GetNumberValue(parsed_second);
    double val_replaced = cJSON_GetNumberValue(replaced);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    (void)parsed_first_invalid;
    (void)arr_item2_invalid;
    (void)replaced_invalid;
    (void)val_parsed_second;
    (void)val_replaced;
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}