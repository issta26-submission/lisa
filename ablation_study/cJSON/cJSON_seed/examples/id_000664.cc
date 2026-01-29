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
//<ID> 664
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
    int numbers_static[5] = {1, 2, 3, 4, 5};
    cJSON *numbers = cJSON_CreateIntArray(numbers_static, 5);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *numbers_dup = cJSON_Duplicate(numbers, 1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObjectCS(root, "numbers_dup", numbers_dup);
    cJSON_AddItemToObjectCS(root, "enabled", flag);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON *parsed_numbers_dup = cJSON_GetObjectItem(parsed, "numbers_dup");
    int parsed_dup_count = cJSON_GetArraySize(parsed_numbers_dup);
    (void)version;
    (void)parsed_count;
    (void)parsed_dup_count;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}