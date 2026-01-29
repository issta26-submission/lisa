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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":1.5,\"b\":2}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(a_item);

    // step 2: Configure
    cJSON *marker = cJSON_AddNullToObject(root, "marker");
    cJSON *new_num = cJSON_CreateNumber(a_val + 4.5);
    cJSON_AddItemToObject(root, "c", new_num);

    // step 3: Operate
    cJSON *c_item = cJSON_GetObjectItem(root, "c");
    double c_val = cJSON_GetNumberValue(c_item);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    (void)a_val;
    (void)c_val;
    (void)marker;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}