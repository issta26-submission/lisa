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
//<ID> 1913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(cfg, "label", label);
    cJSON_SetValuestring(label, "updated");
    cJSON *count = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(cfg, "count", count);
    cJSON *newcount = cJSON_CreateNumber(200.0);
    cJSON_ReplaceItemInObject(cfg, "count", newcount);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    double got = cJSON_GetNumberValue(cJSON_GetObjectItem(cfg, "count"));
    char *labelStr = cJSON_GetStringValue(label);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (((int)got) % 10));
    buffer[2] = (char)('0' + ((labelStr ? (unsigned char)labelStr[0] : (unsigned char)'0') % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}