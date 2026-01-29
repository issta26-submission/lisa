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
//<ID> 994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40, 50};
    cJSON *num_array = cJSON_CreateIntArray(numbers, 5);
    cJSON *label = cJSON_CreateString("device-42");
    cJSON *meta = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(5.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "numbers", num_array);
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "count", count);

    // step 3: Operate & Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    int size = cJSON_GetArraySize(num_array);
    cJSON_AddBoolToObject(root, "has_numbers", has_numbers);
    cJSON_AddNumberToObject(root, "numbers_count", (double)size);
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    char *label_text = cJSON_GetStringValue(got_label);
    cJSON *label_copy = cJSON_CreateString(label_text);
    cJSON_AddItemToObjectCS(root, "label_copy", label_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}