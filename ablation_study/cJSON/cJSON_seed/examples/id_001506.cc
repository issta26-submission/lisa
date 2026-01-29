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
//<ID> 1506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[4];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    cJSON *arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObjectCS(root, "numbers", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "numbers_ref", arr);
    (void)ref_added;
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    const char *parse_end = NULL;
    size_t printed_len = 0;
    if (printed) {
        printed_len = strlen(printed);
    }
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed ? printed : "", printed_len, &parse_end, 1);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    cJSON *first_item = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}