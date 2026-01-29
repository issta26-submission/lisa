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
//<ID> 1438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *template_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(template_obj, "v", 7.0);
    cJSON_AddItemToObject(root, "template", template_obj);
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "threshold", 0.75);
    cJSON_AddNumberToObject(root, "extra_count", 2.0);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(template_obj, 1);
    cJSON_AddItemToArray(array, dup);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double vernum = cJSON_GetNumberValue(ver_item);
    cJSON_bool was_number = cJSON_IsNumber(ver_item);
    cJSON_AddNumberToObject(root, "next_version", vernum + 1.0);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)was_number;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}