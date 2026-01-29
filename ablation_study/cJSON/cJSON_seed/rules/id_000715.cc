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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = NULL;
    cJSON *threshold_item = NULL;
    cJSON *enabled_item = NULL;
    double threshold_value = 0.0;
    cJSON_bool enabled_flag = 0;
    char *printed = NULL;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 3.14159);
    cJSON_AddTrueToObject(settings, "enabled");
    threshold_item = cJSON_GetObjectItem(settings, "threshold");
    enabled_item = cJSON_GetObjectItem(settings, "enabled");

    // step 3: Operate / Validate
    threshold_value = cJSON_GetNumberValue(threshold_item);
    enabled_flag = cJSON_IsTrue(enabled_item);
    printed = cJSON_Print(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}