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
//<ID> 1412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_literal = "{ \"name\": \"Alice\", \"age\": 30, \"items\": [\"a\", \"b\", \"c\"] }";
    char *json_copy = NULL;
    size_t json_len = 0;
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_min = NULL;
    cJSON *injected = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json_literal);
    json_len = strlen(json_literal);
    json_copy = (char *)cJSON_malloc(json_len + 1);
    memcpy(json_copy, json_literal, json_len + 1);
    cJSON_Minify(json_copy);
    parsed_min = cJSON_ParseWithLength(json_copy, strlen(json_copy));
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "minified_parsed", parsed_min);
    injected = cJSON_CreateString("injected_value");
    cJSON_AddItemToObject(root, "injected", injected);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);
    (void)printed;
    (void)parsed;

    // step 4: Cleanup
    cJSON_free(json_copy);
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}