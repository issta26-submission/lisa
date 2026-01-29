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
//<ID> 769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"value\": 123.5, \"flag\": false}";
    size_t json_len = strlen(json);
    char *buffer = (char *)cJSON_malloc(json_len + 1);
    memset(buffer, 0, json_len + 1);
    memcpy(buffer, json, json_len);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *added_num_item = NULL;
    cJSON *false_item = NULL;
    double extracted_value = 0.0;
    double new_value = 0.0;
    double verify_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(buffer, json_len, &parse_end, 1);
    root = cJSON_CreateObject();

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(parsed, "value");
    extracted_value = cJSON_GetNumberValue(num_item);
    new_value = extracted_value + 10.0;
    added_num_item = cJSON_AddNumberToObject(root, "sum", new_value);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    verify_value = cJSON_GetNumberValue(added_num_item);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}