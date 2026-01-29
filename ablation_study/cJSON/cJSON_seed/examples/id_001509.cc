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
//<ID> 1509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const int nums[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    const char json_text[] = "{\"alpha\":42,\"beta\":[7,8]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON_bool added_numbers = cJSON_AddItemToObjectCS(root, "numbers", int_array);
    cJSON_bool added_parsed_ref = cJSON_AddItemReferenceToObject(root, "parsed_ref", parsed);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    int numbers_size = cJSON_GetArraySize(int_array);
    cJSON *first_num_item = cJSON_GetArrayItem(int_array, 0);
    double first_num = cJSON_GetNumberValue(first_num_item);
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    (void)added_numbers;
    (void)added_parsed_ref;
    (void)numbers_size;
    (void)first_num;
    (void)alpha_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}