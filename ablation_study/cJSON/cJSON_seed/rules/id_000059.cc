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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"alpha\":\"one\",\"num\":10}";
    size_t buffer_length = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, buffer_length, &parse_end, 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"replaced\":true}");
    cJSON *found_before = NULL;
    cJSON *found_after = NULL;
    cJSON *status_item = NULL;
    cJSON *echo_item = NULL;
    const char *str_val = NULL;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    added = cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    found_before = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    replaced = cJSON_ReplaceItemViaPointer(root, found_before, raw_item);
    found_after = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    str_val = cJSON_GetStringValue(found_after);
    status_item = cJSON_CreateNumber((double)(added + replaced));
    cJSON_AddItemToObject(root, "status", status_item);
    echo_item = cJSON_CreateString(str_val ? str_val : "");
    cJSON_AddItemToObject(root, "echo", echo_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}