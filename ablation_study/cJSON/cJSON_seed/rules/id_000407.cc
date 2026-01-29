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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    char buffer[4];
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateNumber(2.0);
    cJSON *new_item = cJSON_CreateString("replaced");
    char *printed = NULL;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *flag_item = NULL;

    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);

    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithOpts(printed, &parse_end, 1);
    retrieved_arr = cJSON_GetObjectItem(parsed, "items");
    cJSON_ReplaceItemInArray(retrieved_arr, 1, new_item);
    flag_item = cJSON_AddBoolToObject(parsed, "flag", 1);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)flag_item->type);
    buffer[2] = '\0';

    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    return 66;
}