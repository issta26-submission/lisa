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
//<ID> 1156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json[] = "  /* test comment */  { \"alpha\" : 123.0, \"beta\" : null }  ";
    const char *parse_end = NULL;
    size_t json_len = (size_t)(sizeof(json) - 1);
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *alpha_item = NULL;
    double alpha_value = 0.0;
    cJSON *dup_parsed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNullToObject(root, "initial_null");
    cJSON_Minify(json);
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Operate / Validate
    alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    alpha_value = cJSON_GetNumberValue(alpha_item);
    cJSON_AddItemToObject(root, "alpha_value", cJSON_CreateNumber(alpha_value));
    dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_copy", dup_parsed);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}