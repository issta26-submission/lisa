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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToArray(data, s2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "scale", n1);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddTrueToObject(root, "enabled");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(data);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double computed_val = (double)count * scale;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}