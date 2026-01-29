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
//<ID> 768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *json = "{\"value\":123.5}";
    size_t json_len = strlen(json);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *false_item = NULL;
    cJSON *parsed_value = NULL;
    cJSON *got_value = NULL;
    char *printed = NULL;
    double parsed_num = 0.0;
    double retrieved_num = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", false_item);
    parsed_value = cJSON_GetObjectItem(parsed, "value");
    parsed_num = cJSON_GetNumberValue(parsed_value);
    cJSON_AddNumberToObject(root, "value", parsed_num);
    cJSON_AddNumberToObject(root, "extra", 3.1415);

    // step 3: Operate / Validate
    got_value = cJSON_GetObjectItem(root, "value");
    retrieved_num = cJSON_GetNumberValue(got_value);
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