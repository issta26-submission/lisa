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
//<ID> 662
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
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObjectCS(root, "primary_flag", flag_true);
    cJSON *dup_array = cJSON_Duplicate(int_array, 1);
    cJSON_AddItemToObject(root, "ints_copy", dup_array);
    cJSON_AddItemToObject(root, "secondary_flag", flag_false);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int parsed_count = cJSON_GetArraySize(parsed_ints);
    cJSON *parsed_flag = cJSON_GetObjectItemCaseSensitive(parsed, "primary_flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(parsed_flag);
    (void)version;
    (void)parsed_count;
    (void)flag_is_true;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}