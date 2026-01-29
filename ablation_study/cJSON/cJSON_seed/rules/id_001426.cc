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
//<ID> 1426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed_a = NULL;
    cJSON *parsed_b = NULL;
    cJSON *beta = NULL;
    cJSON *dup_beta = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    char temp_buf[64];

    // step 2: Setup / Configure
    parsed_a = cJSON_Parse("{\"alpha\":1,\"beta\":{\"inner\":true}}");
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", parsed_a);
    parsed_b = cJSON_Parse("{\"alpha\":2,\"beta\":{\"inner\":false}}");
    beta = cJSON_GetObjectItem(parsed_a, "beta");
    dup_beta = cJSON_Duplicate(beta, 1);
    cJSON_AddItemToObject(root, "dup_beta", dup_beta);

    // step 3: Operate / Validate
    replacement = cJSON_GetObjectItem(parsed_b, "beta");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "dup_beta", cJSON_Duplicate(replacement, 1));
    printed = cJSON_Print(root);
    memset(temp_buf, 0, sizeof(temp_buf));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed_b);
    // API sequence test completed successfully
    return 66;
}