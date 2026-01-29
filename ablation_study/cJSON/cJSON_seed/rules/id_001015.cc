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
//<ID> 1015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darr = NULL;
    cJSON *raw_item = NULL;
    cJSON *status_item = NULL;
    cJSON *replacement = NULL;
    cJSON *got_status = NULL;
    char *printed = NULL;
    double numbers[3] = {1.1, 2.2, 3.3};
    int count = 3;
    cJSON_bool was_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darr = cJSON_CreateDoubleArray(numbers, count);
    cJSON_AddItemToObject(root, "measurements", darr);
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);
    status_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", status_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    replacement = cJSON_CreateFalse();
    got_status = cJSON_GetObjectItem(root, "status");
    cJSON_ReplaceItemViaPointer(root, got_status, replacement);
    got_status = cJSON_GetObjectItem(root, "status");
    was_true = cJSON_IsTrue(got_status);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}