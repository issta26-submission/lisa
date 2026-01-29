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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *answer_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_num);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_valid", flag_false);
    cJSON_AddNullToObject(root, "empty");

    // step 2: Configure
    double extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    cJSON_bool original_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "is_valid"));
    cJSON_AddNumberToObject(root, "answer_copy", extracted_value);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    double parsed_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "answer"));
    cJSON_bool parsed_is_false = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "is_valid"));
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig=%f;parsed=%f;orig_false=%d;parsed_false=%d",
            version ? version : "null",
            extracted_value,
            parsed_value,
            (int)original_is_false,
            (int)parsed_is_false);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}