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
//<ID> 665
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
    int initial_numbers[3] = {1, 2, 3};
    cJSON *numbers = cJSON_CreateIntArray(initial_numbers, 3);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObjectCS(root, "duplicate", dup_root);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t snap_len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snap_len + 1);
    memset(buffer, 0, snap_len + 1);
    memcpy(buffer, snapshot, snap_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    int replacement_vals[3] = {10, 20, 30};
    cJSON *replacement_array = cJSON_CreateIntArray(replacement_vals, parsed_size);
    cJSON_ReplaceItemInObject(parsed, "numbers", replacement_array);
    (void)version;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}