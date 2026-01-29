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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"alpha\":1.5,\"beta\":2.5}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *constructed = cJSON_CreateObject();
    cJSON *alpha_item = NULL;
    cJSON *beta_item = NULL;
    double alpha_val = 0.0;
    double beta_val = 0.0;
    cJSON_bool equal = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    alpha_item = cJSON_GetObjectItem(root, "alpha");
    beta_item = cJSON_GetObjectItem(root, "beta");
    cJSON_AddItemReferenceToObject(constructed, "alpha_ref", alpha_item);
    cJSON_AddItemReferenceToObject(constructed, "beta_ref", beta_item);

    // step 3: Operate / Validate
    alpha_val = cJSON_GetNumberValue(alpha_item);
    beta_val = cJSON_GetNumberValue(beta_item);
    equal = cJSON_Compare(root, dup, 1);
    printed = cJSON_PrintUnformatted(constructed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(constructed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}