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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *beta = NULL;
    cJSON *inner = NULL;
    cJSON *num_item = NULL;
    cJSON *replacement = NULL;
    cJSON *detached = NULL;
    cJSON *true_item = NULL;
    char *out = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool added = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse("{\"alpha\":123, \"beta\":{\"inner\":7}}");
    beta = cJSON_GetObjectItem(root, "beta");
    num_item = cJSON_CreateNumber(42.0);
    added = cJSON_AddItemToObject(root, "added_num", num_item);
    true_item = cJSON_AddTrueToObject(beta, "flag");

    // step 3: Operate / Validate
    inner = cJSON_GetObjectItem(beta, "inner");
    detached = cJSON_DetachItemViaPointer(beta, inner);
    added = cJSON_AddItemToObject(root, "moved_inner", detached);
    replacement = cJSON_CreateString("replaced_value");
    replaced = cJSON_ReplaceItemViaPointer(root, num_item, replacement);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}