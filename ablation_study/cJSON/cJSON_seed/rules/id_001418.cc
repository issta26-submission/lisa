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
//<ID> 1418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed_default = NULL;
    cJSON *parsed_minified = NULL;
    char *mutable_json = NULL;
    char *printed = NULL;
    const char *raw_json = "{ \"alpha\": 123, \"beta\": [ true, false, null ], \"gamma\": \"text\" }";
    size_t raw_len = 0;
    size_t min_len = 0;
    int prebuffer = 128;
    cJSON_bool fmt = 1;

    // step 2: Setup / Configure
    raw_len = (size_t)strlen(raw_json);
    mutable_json = (char *)cJSON_malloc(raw_len + 1);
    memcpy(mutable_json, raw_json, raw_len + 1);
    cJSON_Minify(mutable_json);
    min_len = (size_t)strlen(mutable_json);
    parsed_default = cJSON_Parse(raw_json);
    parsed_minified = cJSON_ParseWithLength(mutable_json, min_len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed_default", parsed_default);
    cJSON_AddItemToObject(root, "parsed_minified", parsed_minified);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, prebuffer, fmt);
    cJSON_Minify(printed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(mutable_json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}