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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n0 = cJSON_CreateNumber(4.0);
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *label = cJSON_CreateString("demo");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version_major", (double)version[0]);
    cJSON_AddNumberToObject(root, "base_sum", 11.0);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *scaled = cJSON_CreateNumber(second_val * 3.0);
    cJSON_ReplaceItemInArray(arr, 0, scaled);
    cJSON *replaced = cJSON_GetArrayItem(arr, 0);
    double replaced_val = cJSON_GetNumberValue(replaced);
    char *out = cJSON_PrintBuffered(root, 256, 1);
    char buf[16];
    memset(buf, 0, sizeof(buf));
    buf[0] = (char)('0' + ((int)replaced_val % 10));
    buf[1] = out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}