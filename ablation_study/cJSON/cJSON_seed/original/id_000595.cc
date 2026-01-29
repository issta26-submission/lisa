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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"name\": \"device\", \"scalar\": 3.14 }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *scalar_item = cJSON_GetObjectItem(root, "scalar");
    double original_value = cJSON_GetNumberValue(scalar_item);
    double samples[3];
    samples[0] = 1.0;
    samples[1] = 2.5;
    samples[2] = 3.75;
    cJSON *double_array = cJSON_CreateDoubleArray(samples, 3);
    cJSON_AddItemToObject(root, "measurements", double_array);

    // step 3: Operate
    cJSON *replacement_scalar = cJSON_CreateNumber(original_value + 10.0);
    cJSON_ReplaceItemViaPointer(root, scalar_item, replacement_scalar);

    // step 4: Validate & Cleanup
    cJSON *after_item = cJSON_GetObjectItem(root, "scalar");
    double after_value = cJSON_GetNumberValue(after_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)original_value;
    (void)after_value;
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}