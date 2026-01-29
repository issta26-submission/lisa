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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    double *pval = (double *)cJSON_malloc(sizeof(double));
    *pval = 3.14159;
    cJSON *pi_item = cJSON_CreateNumber(*pval);
    cJSON *sum_item = NULL;
    cJSON *retrieved_pi = NULL;
    cJSON *retrieved_version = NULL;
    double pi_val = 0.0;
    double sum_val = 0.0;
    const char *version_str = NULL;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    version_str = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version_str);

    // step 3: Operate / Validate
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    sum_val = pi_val + 1.0;
    sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "pi_plus_one", sum_item);
    retrieved_version = cJSON_GetObjectItem(root, "version");
    is_str = cJSON_IsString(retrieved_version);
    (void)is_str;

    // step 4: Cleanup
    cJSON_free(pval);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}