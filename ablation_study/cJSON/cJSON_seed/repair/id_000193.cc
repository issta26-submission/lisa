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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s_alpha = cJSON_CreateString("alpha_value");
    cJSON_AddItemToObject(root, "alpha", s_alpha);
    cJSON_AddStringToObject(root, "beta", "beta_value");

    // step 3: Operate and Validate
    cJSON *got_alpha = cJSON_GetObjectItem(root, "alpha");
    cJSON *got_beta = cJSON_GetObjectItem(root, "beta");
    const char *val_alpha = cJSON_GetStringValue(got_alpha);
    const char *val_beta = cJSON_GetStringValue(got_beta);
    size_t len_a = strlen(val_alpha);
    size_t len_b = strlen(val_beta);
    size_t total = len_a + len_b + 2;
    char *combined = (char *)cJSON_malloc(total);
    memset(combined, 0, total);
    memcpy(combined, val_alpha, len_a);
    combined[len_a] = '_';
    memcpy(combined + len_a + 1, val_beta, len_b);
    cJSON_AddStringToObject(root, "combined", combined);
    cJSON_free(combined);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}