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
//<ID> 2381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *num_inner = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(inner, "value", num_inner);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *num_thresh = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "threshold", num_thresh);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    double thr_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "threshold"));
    double inner_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "inner"), "value"));
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)thr_val;
    buffer[2] = (char)(int)inner_val;
    buffer[3] = (char)(int)has_enabled;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}