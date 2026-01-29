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
//<ID> 1358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *n_pi = cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *n_count = cJSON_AddNumberToObject(root, "count", 10.0);

    // step 2: Configure
    cJSON *replacement_count = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "count", replacement_count);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    double pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)pi_val % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)count_val % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}