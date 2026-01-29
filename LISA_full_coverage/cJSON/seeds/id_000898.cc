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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dnums[3] = {1.5, 2.5, -3.0};
    float fnums[2] = {4.0f, 5.5f};
    cJSON *arrD = cJSON_CreateDoubleArray(dnums, 3);
    cJSON *arrF = cJSON_CreateFloatArray(fnums, 2);
    cJSON_AddItemToObject(root, "doubleArray", arrD);
    cJSON_AddItemToObject(root, "floatArray", arrF);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "creator", "tester");

    // step 3: Operate
    cJSON *firstD = cJSON_GetArrayItem(arrD, 0);
    double v1 = cJSON_GetNumberValue(firstD);
    cJSON *secondF = cJSON_GetArrayItem(arrF, 1);
    double v2 = cJSON_GetNumberValue(secondF);
    double sum = v1 + v2;
    cJSON_AddNumberToObject(meta, "sum", sum);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_DeleteItemFromObject(root, "floatArray");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}