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
//<ID> 1159
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
    cJSON *copied_name = NULL;
    const char *parse_end = NULL;
    char json[] = "{ \"name\" : \"Alice\", \"items\": [ 1, 2, 3 ], \"active\" : true }";
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_Minify(json);
    parsed = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    root = cJSON_CreateObject();
    name_item = cJSON_GetObjectItem(parsed, "name");
    copied_name = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate / Validate
    cJSON_AddItemToObject(root, "from_parsed", parsed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}