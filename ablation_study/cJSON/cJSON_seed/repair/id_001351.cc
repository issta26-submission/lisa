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
//<ID> 1351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "alpha", 3.14);
    cJSON_AddNumberToObject(root, "beta", 2.71);
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *n_item = cJSON_AddNumberToObject(child, "n", 10.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    double val_alpha_before = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "alpha"));
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON_bool replaced = cJSON_ReplaceItemInObject(child, "n", replacement);
    double val_n_after = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "n"));
    double val_beta = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "beta"));
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)val_alpha_before % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)val_n_after % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[4] = (char)('0' + ((int)val_beta % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}