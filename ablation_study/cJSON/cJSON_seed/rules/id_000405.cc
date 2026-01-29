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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateString("second");
    cJSON *replacement = cJSON_CreateString("second_replaced");
    cJSON *enabled_item = NULL;
    cJSON *parsed = NULL;
    cJSON *arr_from_parsed = NULL;
    cJSON *arr_item = NULL;
    const char *parse_end = NULL;
    cJSON_bool replaced = 0;
    const char *retrieved_str = NULL;

    // step 2: Setup / Configure
    enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item1);
    replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    printed = cJSON_PrintBuffered(root, 256, 1);
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);

    // step 3: Operate / Validate
    arr_from_parsed = cJSON_GetObjectItem(parsed, "list");
    arr_item = cJSON_GetArrayItem(arr_from_parsed, 1);
    retrieved_str = cJSON_GetStringValue(arr_item);
    buffer[0] = retrieved_str ? retrieved_str[0] : 'X';
    buffer[1] = (char)('0' + (int)replaced);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}