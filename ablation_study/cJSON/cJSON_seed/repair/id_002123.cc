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
//<ID> 2123
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *s_item = cJSON_CreateString("s1");
    cJSON_AddItemToArray(arr, s_item);
    cJSON *label_prop = cJSON_AddStringToObject(root, "label", "root_label");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *num_item = cJSON_GetArrayItem(items, 1);
    double extracted = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "extracted", extracted);
    cJSON *label_item = cJSON_GetObjectItemCaseSensitive(root, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = label_str ? label_str[0] : '\0';
    buffer[1] = (char)('0' + ((int)extracted % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}