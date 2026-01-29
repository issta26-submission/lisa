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
//<ID> 1481
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
    cJSON *parsed_root = NULL;
    cJSON *dup_true = NULL;
    cJSON_bool equal = 0;
    char mutable_json[] = "{\"key\": true, \"opt\": null}";
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "key", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "opt", null_item);
    cJSON_Minify(mutable_json);
    parsed_root = cJSON_Parse(mutable_json);
    dup_true = cJSON_Duplicate(cJSON_GetObjectItem(root, "key"), 1);
    cJSON_AddItemToObject(root, "key_copy", dup_true);

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, parsed_root, 1);
    (void)scratch;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(parsed_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}