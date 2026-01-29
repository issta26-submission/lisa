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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *readings = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *label = cJSON_CreateString("sensorA");
    cJSON_AddItemToArray(readings, n1);
    cJSON_AddItemToArray(readings, n2);
    cJSON_AddItemToObject(root, "readings", readings);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate & Validate
    cJSON_SetNumberHelper(n2, 25.75);
    cJSON *first = cJSON_GetArrayItem(readings, 0);
    cJSON *second = cJSON_GetArrayItem(readings, 1);
    double vfirst = cJSON_GetNumberValue(first);
    double vsecond = cJSON_GetNumberValue(second);
    double total = vfirst + vsecond;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    char *slabel = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(slabel);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}