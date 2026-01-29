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
//<ID> 1355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "a", 1.0);
    cJSON_AddNumberToObject(root, "b", 2.5);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "answer", 42.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "a", replacement);

    // step 3: Operate and Validate
    cJSON *item_a = cJSON_GetObjectItem(root, "a");
    cJSON *item_b = cJSON_GetObjectItem(root, "b");
    double val_a = cJSON_GetNumberValue(item_a);
    double val_b = cJSON_GetNumberValue(item_b);
    double sum = val_a + val_b;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (int)val_a % 10);
    ((char *)scratch)[2] = (char)('0' + (int)val_b % 10);
    ((char *)scratch)[3] = (char)('0' + (int)sum % 10);
    ((char *)scratch)[4] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}