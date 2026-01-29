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
//<ID> 1489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *parsed = NULL;
    cJSON *inner = NULL;
    cJSON_bool true_check = 0;
    cJSON_bool null_check = 0;
    cJSON_bool inner_check = 0;
    char json[] = " { \"embedded\" : true , \"empty\" : null } ";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "opt", null_item);
    cJSON_Minify(json);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    true_check = cJSON_IsTrue(true_item);
    null_check = cJSON_IsNull(null_item);
    inner = cJSON_GetObjectItem(parsed, "embedded");
    inner_check = cJSON_IsTrue(inner);
    (void)true_check;
    (void)null_check;
    (void)inner_check;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}