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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw = "{\"width\":800,\"height\":600}";
    cJSON *root = cJSON_Parse(raw);
    cJSON *pi_item = cJSON_CreateNumber(3.14159);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "depth", 32.0);
    cJSON_AddItemToObject(root, "pi", pi_item);

    // step 3: Operate and Validate
    char *compact = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    cJSON *width_item = cJSON_GetObjectItem(root, "width");
    double width_v = cJSON_GetNumberValue(width_item);
    cJSON *height_item = cJSON_GetObjectItem(root, "height");
    double height_v = cJSON_GetNumberValue(height_item);
    double area = width_v * height_v;
    cJSON_AddNumberToObject(root, "area", area);
    cJSON *root2 = cJSON_Parse(compact);
    cJSON_bool identical = cJSON_Compare(root, root2, 1);
    cJSON_AddNumberToObject(root, "identical", (double)identical);

    // step 4: Cleanup
    cJSON_free(compact);
    cJSON_free(pretty);
    cJSON_Delete(root2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}