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
//<ID> 1482
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
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *parsed_true = NULL;
    cJSON *parsed_null = NULL;
    cJSON_bool equal_true = 0;
    cJSON_bool equal_null = 0;
    char json_buf[] = "{\n  \"enabled\" : true ,\n  \"optional\" : null \n}\n";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "optional", null_item);

    // step 3: Operate / Validate
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    parsed_true = cJSON_GetObjectItem(parsed, "enabled");
    parsed_null = cJSON_GetObjectItem(parsed, "optional");
    equal_true = cJSON_Compare(true_item, parsed_true, 1);
    equal_null = cJSON_Compare(null_item, parsed_null, 1);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)equal_true;
    (void)equal_null;

    return 66;
}