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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *created_num = cJSON_CreateNumber(7.5);
    cJSON *parsed = NULL;
    cJSON *p_alpha = NULL;
    cJSON *p_beta = NULL;
    cJSON *retrieved_created = NULL;
    double alpha_val = 0.0;
    double beta_val = 0.0;
    double created_val = 0.0;
    double sum = 0.0;
    const char *json = "{\"alpha\":10,\"beta\":3.5}";
    size_t json_len = sizeof("{\"alpha\":10,\"beta\":3.5}") - 1;
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON_AddNumberToObject(root, "explicit", 2.25);
    parsed = cJSON_ParseWithLength(json, json_len);

    // step 3: Operate / Validate
    p_alpha = cJSON_GetObjectItem(parsed, "alpha");
    alpha_val = cJSON_GetNumberValue(p_alpha);
    p_beta = cJSON_GetObjectItem(parsed, "beta");
    beta_val = cJSON_GetNumberValue(p_beta);
    retrieved_created = cJSON_GetObjectItem(root, "created");
    created_val = cJSON_GetNumberValue(retrieved_created);
    sum = alpha_val + beta_val + created_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    /* use scratch to make a visible side-effect of the computed sum */
    scratch[0] = (char)('0' + ((int)sum % 10));
    scratch[1] = (char)('\n');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}