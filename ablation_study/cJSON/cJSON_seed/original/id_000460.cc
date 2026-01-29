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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"alpha\":1.5,\"beta\":2.5}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *alpha_item = cJSON_GetObjectItem(root, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);

    // step 2: Configure
    cJSON *marker = cJSON_AddNullToObject(root, "marker");
    char empty_buf[16];
    memset(empty_buf, 0, sizeof(empty_buf));
    cJSON *empty_str = cJSON_CreateString(empty_buf);
    cJSON_AddItemToObject(root, "empty", empty_str);
    cJSON *plus_num = cJSON_CreateNumber(alpha_val + 4.0);
    cJSON_AddItemToObject(root, "gamma", plus_num);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *gamma_item = cJSON_GetObjectItem(root, "gamma");
    double gamma_val = cJSON_GetNumberValue(gamma_item);
    (void)gamma_val;
    (void)marker;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}