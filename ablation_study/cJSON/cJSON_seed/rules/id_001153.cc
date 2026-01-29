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
//<ID> 1153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *wrapper = NULL;
    cJSON *name_item = NULL;
    const char *name_value = NULL;
    cJSON *greeting = NULL;
    cJSON *dup_parsed = NULL;
    cJSON *null_item = NULL;
    const char *parse_end = NULL;
    size_t parsed_len = 0;
    char mutable_json[] = " {\n    \"name\" : \"original name\",\n    \"value\" : 42\n}   ";
    size_t buf_len = (size_t)(sizeof(mutable_json) - 1);

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_ParseWithLengthOpts(mutable_json, buf_len, &parse_end, 1);
    wrapper = cJSON_CreateObject();
    name_item = cJSON_GetObjectItem(parsed, "name");
    name_value = cJSON_GetStringValue(name_item);
    greeting = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(wrapper, "from_parsed", greeting);
    null_item = cJSON_AddNullToObject(wrapper, "missing");

    // step 3: Operate / Validate
    parsed_len = (size_t)(parse_end - mutable_json);
    dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(wrapper, "original", dup_parsed);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(wrapper);
    // API sequence test completed successfully
    return 66;
}