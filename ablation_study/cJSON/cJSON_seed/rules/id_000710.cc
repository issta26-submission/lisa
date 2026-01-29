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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = NULL;
    cJSON *threshold_item = NULL;
    cJSON *enabled_item = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    double threshold_value = 0.0;
    cJSON_bool enabled_flag = 0;

    // step 2: Setup / Configure
    settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 42.5);
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate / Validate
    threshold_item = cJSON_GetObjectItem(settings, "threshold");
    threshold_value = cJSON_GetNumberValue(threshold_item);
    enabled_item = cJSON_GetObjectItem(settings, "enabled");
    enabled_flag = cJSON_IsTrue(enabled_item);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(buffer, printed ? printed : "", printed_len);
    buffer[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}