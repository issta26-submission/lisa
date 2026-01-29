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
//<ID> 1271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON *str0 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, num0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, str0);
    cJSON_AddStringToObject(root, "label", "example");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);
    (void)first_invalid;
    cJSON *replacement = cJSON_CreateNumber(99.9);
    cJSON_ReplaceItemInArray(items, 1, replacement);
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);

    // step 4: Validate & Cleanup
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);
    (void)second_invalid;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}