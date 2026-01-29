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
//<ID> 764
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
    const char *json_text = "{\"parsed\": 3.14}";
    size_t json_len = strlen(json_text);
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *false_item = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_num = NULL;
    char *printed = NULL;
    double num_value = 0.0;
    double parsed_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "from_parse", parsed);

    // step 3: Operate / Validate
    num_value = cJSON_GetNumberValue(num_item);
    parsed_num = cJSON_GetObjectItem(parsed, "parsed");
    parsed_value = cJSON_GetNumberValue(parsed_num);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}