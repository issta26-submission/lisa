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
//<ID> 830
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_numbers[3] = {3.14, 2.71, 1.41};
    cJSON *numbers = cJSON_CreateDoubleArray(initial_numbers, 3);
    cJSON *raw_meta = cJSON_CreateRaw("raw_info:true");
    cJSON_AddItemToObjectCS(root, "numbers", numbers);
    cJSON_AddItemToObjectCS(root, "meta", raw_meta);

    // step 2: Configure
    cJSON *replacement_number = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(numbers, 1, replacement_number);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *second_item = cJSON_GetArrayItem(parsed_numbers, 1);
    double extracted_value = cJSON_GetNumberValue(second_item);
    (void)extracted_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}