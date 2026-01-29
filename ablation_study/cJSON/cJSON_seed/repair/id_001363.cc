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
//<ID> 1363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON *label_item = cJSON_AddStringToObject(root, "label", "example_label");

    // step 3: Operate and Validate
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    const char *label_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)count_val % 10));
    ((char *)scratch)[2] = label_val ? label_val[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}