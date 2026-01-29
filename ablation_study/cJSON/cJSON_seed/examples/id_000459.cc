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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_Parse("[]");

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON *num2 = cJSON_CreateNumber(2.71828);
    cJSON_bool added1 = cJSON_AddItemToArray(root, num1);
    cJSON_bool added2 = cJSON_AddItemToArray(root, num2);

    // step 3: Operate & Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 1));
    double total = v0 + v1;
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s|sum=%.5f|added=%d,%d", version ? version : "unknown", total, (int)added1, (int)added2);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    (void)status_buf;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}