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
//<ID> 1483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *inner = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON_bool added_root_true = 0;
    cJSON_bool added_root_null = 0;
    cJSON_bool added_inner_true = 0;
    char json_buf[64] = "{  \"opt\"  :  null  ,  \"flag\"  :  true  }";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    null_item = cJSON_CreateNull();
    added_root_true = cJSON_AddItemToObject(root, "flag", true_item);
    added_root_null = cJSON_AddItemToObject(root, "opt", null_item);
    added_inner_true = cJSON_AddItemToObject(inner, "nested_flag", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "child", inner);

    // step 3: Operate / Validate
    cJSON_Minify(json_buf);
    (void)json_buf;
    (void)added_root_true;
    (void)added_root_null;
    (void)added_inner_true;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}