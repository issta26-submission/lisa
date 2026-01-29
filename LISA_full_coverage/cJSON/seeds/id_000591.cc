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
//<ID> 591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"value\": 10.5, \"name\": \"sample\" }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig_value = cJSON_GetNumberValue(value_item);
    double numbers[3];
    numbers[0] = orig_value;
    numbers[1] = orig_value + 1.25;
    numbers[2] = orig_value + 2.5;
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);

    // step 3: Operate & Validate
    cJSON *replacement_num = cJSON_CreateNumber(orig_value * 2.0);
    cJSON_ReplaceItemViaPointer(root, value_item, replacement_num);
    cJSON *new_value_item = cJSON_GetObjectItem(root, "value");
    double new_value = cJSON_GetNumberValue(new_value_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)new_value;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}