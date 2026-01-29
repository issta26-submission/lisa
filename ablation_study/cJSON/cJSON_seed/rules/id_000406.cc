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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateNumber(42.0);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *flag_item = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *second_item = NULL;
    char *second_str = NULL;
    cJSON_bool flag_val = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate / Validate
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    printed = cJSON_PrintBuffered(root, 128, 1);
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);
    retrieved_arr = cJSON_GetObjectItem(parsed, "list");
    second_item = cJSON_GetArrayItem(retrieved_arr, 1);
    second_str = cJSON_GetStringValue(second_item);
    flag_val = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "flag"));
    buffer[0] = second_str ? second_str[0] : '\0';
    buffer[1] = (char)('0' + (int)flag_val);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}