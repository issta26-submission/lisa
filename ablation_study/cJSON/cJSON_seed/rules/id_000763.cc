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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    size_t bufsize = 256;
    char *scratch = (char *)cJSON_malloc(bufsize);
    memset(scratch, 0, bufsize);
    const char *json = "{\"value\": 3.1415}";
    size_t json_len = sizeof("{\"value\": 3.1415}") - 1;
    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 1;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *value_item = NULL;
    cJSON *false_item = NULL;
    double extracted = 0.0;
    double copied = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, require_null_terminated);
    value_item = cJSON_GetObjectItem(parsed, "value");
    extracted = cJSON_GetNumberValue(value_item);
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "copied_value", extracted);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 3: Operate / Validate
    copied = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "copied_value"));
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}