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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = { 1.25, 2.5, 3.75 };
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *initial_raw = cJSON_CreateRaw("/*initial_raw*/");
    cJSON_AddItemToObjectCS(root, "numbers", dbl_array);
    cJSON_AddItemToObjectCS(root, "raw_entry", initial_raw);

    // step 2: Configure
    cJSON *replacement_raw = cJSON_CreateRaw("42");
    cJSON_ReplaceItemInArray(dbl_array, 1, replacement_raw);

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
    const char *second_str = cJSON_GetStringValue(second_item);
    double second_num = cJSON_GetNumberValue(second_item);
    (void)second_str;
    (void)second_num;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}