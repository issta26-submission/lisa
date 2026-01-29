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
//<ID> 1539
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
    cJSON *alpha = NULL;
    cJSON *beta = NULL;
    cJSON *newnum = NULL;
    cJSON *newnum_dup = NULL;
    cJSON *retrieved_dup = NULL;
    double beta_val = 0.0;
    double created_val = 0.0;
    char json[] = "{ \"alpha\": { \"beta\": 123.5 }, \"gamma\": 456 }";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    alpha = cJSON_GetObjectItem(parsed, "alpha");
    beta = cJSON_GetObjectItem(alpha, "beta");
    beta_val = cJSON_GetNumberValue(beta);
    created_val = beta_val * 2.0;
    newnum = cJSON_CreateNumber(created_val);
    cJSON_AddItemToObject(alpha, "newnum", newnum);
    newnum_dup = cJSON_Duplicate(newnum, 1);
    cJSON_AddItemToObject(root, "newnum_dup", newnum_dup);
    cJSON_DeleteItemFromObjectCaseSensitive(alpha, "newnum");
    retrieved_dup = cJSON_GetObjectItem(root, "newnum_dup");
    double dup_val = cJSON_GetNumberValue(retrieved_dup);
    (void)dup_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}