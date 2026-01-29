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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json[] = "{ \"flagA\" : false, \"nested\" : { \"x\" : 1 } }";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = (cJSON *)0;
    cJSON *cs_item = (cJSON *)0;
    cJSON *added_flag = (cJSON *)0;
    cJSON *flagA_item = (cJSON *)0;
    cJSON_bool flagA_is_false = 0;

    // step 2: Initialize - minify and parse JSON
    cJSON_Minify(json);
    root = cJSON_ParseWithLength(json, json_len);

    // step 3: Configure - add items using CS and bool helper
    cs_item = cJSON_CreateString("inner_value");
    cJSON_AddItemToObjectCS(root, "cs_key", cs_item);
    added_flag = cJSON_AddBoolToObject(root, "added_flag", 1);

    // step 4: Operate / Validate - inspect existing flag and record result
    flagA_item = cJSON_GetObjectItem(root, "flagA");
    flagA_is_false = cJSON_IsFalse(flagA_item);
    cJSON_AddBoolToObject(root, "flagA_was_false", flagA_is_false);

    // step 5: Cleanup - free all cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}