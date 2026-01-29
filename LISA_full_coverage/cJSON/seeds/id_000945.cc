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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON *n3 = cJSON_CreateNumber(30.75);
    cJSON_InsertItemInArray(array, 0, n1);
    cJSON_InsertItemInArray(array, 1, n2);
    cJSON_InsertItemInArray(array, 1, n3);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate
    cJSON *mid = cJSON_GetArrayItem(array, 1);
    double midval = cJSON_GetNumberValue(mid);
    cJSON *aug = cJSON_CreateNumber(midval + 100.0);
    cJSON_AddItemToObject(root, "augmented", aug);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}