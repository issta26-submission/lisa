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
//<ID> 1485
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
    cJSON *parsed_flag = NULL;
    cJSON *parsed_opt = NULL;
    cJSON_bool parsed_flag_is_true = 0;
    cJSON_bool parsed_opt_is_null = 0;
    char raw_json[] = " { \"flag\" : true , \"opt\" : null } ";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "opt", null_item);

    // step 3: Operate / Validate
    cJSON_Minify(raw_json);
    parsed = cJSON_Parse(raw_json);
    parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    parsed_opt = cJSON_GetObjectItem(parsed, "opt");
    parsed_flag_is_true = cJSON_IsTrue(parsed_flag);
    parsed_opt_is_null = cJSON_IsNull(parsed_opt);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parsed_flag_is_true;
    (void)parsed_opt_is_null;
    return 66;
}