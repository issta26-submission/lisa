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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":{\"x\":1,\"y\":2},\"beta\":[10,20]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *refs = cJSON_CreateArray();

    // step 2: Configure
    cJSON *alpha = cJSON_GetObjectItemCaseSensitive(root, "alpha");
    cJSON *beta = cJSON_GetObjectItemCaseSensitive(root, "beta");
    cJSON *beta_first = cJSON_GetArrayItem(beta, 0);
    cJSON_AddItemReferenceToArray(refs, alpha);
    cJSON_AddItemReferenceToArray(refs, beta_first);

    // step 3: Operate and Validate
    cJSON_AddItemToObject(root, "refs", refs);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}