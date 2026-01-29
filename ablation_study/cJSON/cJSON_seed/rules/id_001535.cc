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
//<ID> 1535
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
    cJSON *dup = NULL;
    cJSON *num = NULL;
    cJSON *double_gamma = NULL;
    cJSON *beta = NULL;
    cJSON *gamma = NULL;
    const char *parse_end = NULL;
    char json[] = "{ \"alpha\": 1, \"beta\": { \"gamma\": 3 } }";
    size_t len = sizeof(json) - 1;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));
    double gamma_val = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "imported_copy", dup);
    beta = cJSON_GetObjectItem(parsed, "beta");
    gamma = cJSON_GetObjectItem(beta, "gamma");
    gamma_val = cJSON_GetNumberValue(gamma);
    double_gamma = cJSON_CreateNumber(gamma_val * 2.0);
    cJSON_AddItemToObject(root, "double_gamma", double_gamma);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "answer");
    cJSON_DeleteItemFromObject(root, "imported_copy");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}