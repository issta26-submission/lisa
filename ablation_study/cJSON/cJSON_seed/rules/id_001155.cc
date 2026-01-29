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
//<ID> 1155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *name_item = NULL;
    cJSON *value_item = NULL;
    const char *parse_end = NULL;
    const char *extracted_name = NULL;
    double extracted_value = 0.0;
    char minify_buf[] = "{\n  \"name\": \"example\",\n  \"value\": 42\n}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));
    cJSON_AddNullToObject(root, "nullable");

    // step 3: Operate / Validate
    cJSON_Minify(minify_buf);
    parsed = cJSON_ParseWithLengthOpts(minify_buf, (size_t)(sizeof(minify_buf) - 1), &parse_end, 1);
    name_item = cJSON_GetObjectItem(parsed, "name");
    value_item = cJSON_GetObjectItem(parsed, "value");
    extracted_name = cJSON_GetStringValue(name_item);
    extracted_value = cJSON_GetNumberValue(value_item);
    cJSON_AddItemToObject(root, "from_parsed_name", cJSON_CreateString(extracted_name));
    cJSON_AddItemToObject(root, "from_parsed_value", cJSON_CreateNumber(extracted_value));

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}