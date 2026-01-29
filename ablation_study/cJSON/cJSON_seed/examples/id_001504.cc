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
//<ID> 1504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObjectCS(root, "numbers", ints);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(meta, "description", cJSON_CreateString("integer list"));
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "numbers_ref", ints);
    const char sample_json[] = "{\"parsed\":true,\"value\":123}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(sample_json, sizeof(sample_json) - 1, &parse_end, 0);

    // step 3: Operate & Validate
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_parsed = cJSON_PrintUnformatted(parsed);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double extracted_value = cJSON_GetNumberValue(value_item);
    cJSON_AddNumberToObject(root, "extracted_value", extracted_value);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}