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
//<ID> 765
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
    const char *parse_end = NULL;
    const char *json_text = "{\"parsed_number\":3.14}";
    size_t json_len = strlen(json_text);
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *parsed_num_item = NULL;
    char *printed = NULL;
    double read_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "sub", parsed);
    num_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "original", num_item);

    // step 3: Operate / Validate
    cJSON_AddNumberToObject(root, "added_via_api", 7.5);
    bool_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", bool_item);
    parsed_num_item = cJSON_GetObjectItem(parsed, "parsed_number");
    read_value = cJSON_GetNumberValue(parsed_num_item);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}