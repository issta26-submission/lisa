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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 3: Operate & Validate
    cJSON *retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool missing_is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "missing_key"));
    cJSON *n3 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(retrieved_numbers, n3);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "count=%d|null=%d", cJSON_GetArraySize(retrieved_numbers), (int)missing_is_null);
    cJSON_AddStringToObject(root, "status", status_buf);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}