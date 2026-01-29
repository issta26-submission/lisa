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
//<ID> 471
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
    cJSON_AddStringToObject(root, "name", "fuzzer-example");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *str_item = cJSON_CreateString("greeting");

    // step 2: Configure
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToArray(items, raw_item);
    cJSON_AddItemToArray(items, str_item);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_bool second_is_false = cJSON_IsFalse(second);
    int flags = (int)first_is_true | ((int)second_is_false << 1);
    (void)flags;
    cJSON_DeleteItemFromArray(items, 2);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}