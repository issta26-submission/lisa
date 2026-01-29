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
//<ID> 1594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *null_item = NULL;
    cJSON *false_item = NULL;
    char *out = NULL;
    cJSON_bool was_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);
    null_item = cJSON_CreateNull();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 3: Operate / Validate
    was_false = cJSON_IsFalse(false_item);
    cJSON_DeleteItemFromObject(root, "empty");
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}