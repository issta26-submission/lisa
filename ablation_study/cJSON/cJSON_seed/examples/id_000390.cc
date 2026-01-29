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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers_src[3] = {1.0, 2.5, 4.0};
    cJSON *numbers = cJSON_CreateDoubleArray(numbers_src, 3);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double original = cJSON_GetNumberValue(first_item);
    double updated = cJSON_SetNumberHelper(first_item, original + 3.14);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "orig=%.2f|upd=%.2f|flag=%d", original, updated, (int)cJSON_IsTrue(flag));
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}