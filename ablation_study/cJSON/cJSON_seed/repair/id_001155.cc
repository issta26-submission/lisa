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
//<ID> 1155
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, bfalse);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 1.5);
    cJSON_AddStringToObject(root, "title", "example");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    int count_ref = cJSON_GetArraySize(items_ref);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double computed_value = (double)count_ref * scale + 0.5;
    cJSON *computed = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}