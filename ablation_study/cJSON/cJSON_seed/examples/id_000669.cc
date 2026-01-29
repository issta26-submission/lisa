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
//<ID> 669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int numbers_static[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers_static, 3);
    cJSON *primary_flag = cJSON_CreateBool(1);
    cJSON *flag_duplicate = cJSON_Duplicate(primary_flag, 0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObjectCS(root, "primary_flag", primary_flag);
    cJSON_AddItemToObject(root, "flag_copy", flag_duplicate);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON *first_item = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)version;
    (void)parsed_count;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}