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
//<ID> 663
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
    int numbers_static[4] = {1, 2, 3, 4};
    cJSON *numbers = cJSON_CreateIntArray(numbers_static, 4);
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObjectCS(root, "active", flag);

    // step 3: Operate & Validate
    cJSON *numbers_dup = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", numbers_dup);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *parsed_numbers_copy = cJSON_GetObjectItem(parsed, "numbers_copy");
    int original_size = cJSON_GetArraySize(parsed_numbers);
    int copy_size = cJSON_GetArraySize(parsed_numbers_copy);
    (void)version;
    (void)original_size;
    (void)copy_size;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}