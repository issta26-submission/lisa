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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *alpha_item = NULL;
    double alpha_value = 0.0;
    const char *parse_end = NULL;
    char mutable_json[] = " { \"alpha\" : 1 , \"beta\" : \"two\" , \"nested\" : { \"inner\" : 3 } } ";

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_ParseWithLengthOpts(mutable_json, (size_t)(sizeof(mutable_json) - 1), &parse_end, 0);
    root = cJSON_CreateObject();
    cJSON_AddNullToObject(parsed, "optional_null");
    cJSON_AddItemToObject(root, "data", parsed);

    // step 3: Operate / Validate
    alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    alpha_value = cJSON_GetNumberValue(alpha_item);
    cJSON_AddItemToObject(root, "alpha_copy", cJSON_CreateNumber(alpha_value));
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("minified_and_parsed"));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}