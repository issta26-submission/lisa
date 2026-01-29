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
//<ID> 2380
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
    cJSON *num_threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(inner, "threshold", num_threshold);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *inner_found = cJSON_GetObjectItem(root, "inner");
    cJSON *thr_found = cJSON_GetObjectItem(inner_found, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_found);
    cJSON_bool has_active = cJSON_HasObjectItem(root, "active");
    cJSON *plus_one = cJSON_CreateNumber(thr_val + 1.0);
    cJSON_AddItemToObject(root, "threshold_plus_one", plus_one);
    cJSON *read_back = cJSON_GetObjectItem(root, "threshold_plus_one");
    double read_val = cJSON_GetNumberValue(read_back);

    // step 4: Validate and Cleanup
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)thr_val;
    buffer[2] = (char)(int)read_val;
    buffer[3] = (char)(int)has_active;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}