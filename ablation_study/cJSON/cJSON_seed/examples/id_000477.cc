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
//<ID> 477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(root, "title", "cJSON_api_sequence_test");
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToArray(flags, false_item);
    cJSON *raw = cJSON_CreateRaw("{\"rawnum\":42,\"note\":\"raw content\"}");
    cJSON_AddItemToObject(root, "raw_payload", raw);

    // step 3: Operate & Validate
    cJSON_bool is_first_true = cJSON_IsTrue(cJSON_GetArrayItem(flags, 0));
    cJSON_bool is_second_false = cJSON_IsFalse(cJSON_GetArrayItem(flags, 1));
    cJSON_DeleteItemFromArray(flags, 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)version;
    (void)is_first_true;
    (void)is_second_false;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}