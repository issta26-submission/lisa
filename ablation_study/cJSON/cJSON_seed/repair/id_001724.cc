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
//<ID> 1724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(config, "name", "fuzzer");
    cJSON *count_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "count", count_item);
    cJSON_AddNullToObject(config, "optional");

    // step 2: Configure
    double updated = cJSON_SetNumberHelper(count_item, 42.5);
    (void)updated;
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    buffer[1] = (char)('F' + (flag_true * ('T' - 'F')));
    cJSON *got_count_item = cJSON_GetObjectItem(config, "count");
    double got_count = cJSON_GetNumberValue(got_count_item);
    int digit = ((int)got_count % 10 + 10) % 10;
    buffer[2] = (char)('0' + digit);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}