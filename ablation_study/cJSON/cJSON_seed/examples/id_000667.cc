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
//<ID> 667
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
    int base_nums[4] = {1, 2, 3, 4};
    cJSON *int_array = cJSON_CreateIntArray(base_nums, 4);
    cJSON *flag = cJSON_CreateBool((cJSON_bool)1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON_AddStringToObject(root, "author", "fuzzer");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int size_before = cJSON_GetArraySize(parsed_numbers);
    cJSON *dup_numbers = cJSON_Duplicate(parsed_numbers, 1);
    cJSON_AddItemToObject(parsed, "dup_numbers", dup_numbers);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON *dup_flag = cJSON_Duplicate(parsed_flag, 0);
    cJSON_AddItemToObjectCS(parsed, "flag_copy", dup_flag);
    int replacement_vals[3] = {10, 20, 30};
    cJSON *replacement_array = cJSON_CreateIntArray(replacement_vals, 3);
    cJSON_AddItemToObject(parsed, "replacement", replacement_array);
    int size_after = cJSON_GetArraySize(cJSON_GetObjectItem(parsed, "replacement"));
    (void)version;
    (void)size_before;
    (void)size_after;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}