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
//<ID> 767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    const char json[] = "{\"value\": 123.45, \"flag\": false}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *new_root = NULL;
    cJSON *false_item = NULL;
    cJSON *got = NULL;
    double value = 0.0;
    double copied_val = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    num_item = cJSON_GetObjectItem(parsed, "value");
    value = cJSON_GetNumberValue(num_item);
    new_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_root, "copied_value", value);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(new_root, "copied_flag", false_item);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(new_root, "copied_value");
    copied_val = cJSON_GetNumberValue(got);
    scratch[0] = (char)copied_val;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(new_root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}